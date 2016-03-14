//
//  MFileUtils.cpp
//  legend
//
//  Created by Xu Jinyang on 13-3-14.
//
//

#include "MFileUtils.h"
#include "mlib.h"
#include "json.h"
#include "mlib_ccext.h"
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <algorithm>

USING_NS_CC;
using namespace std;

MLIB_NS_BEGIN

const std::string MFileUtils::EVENT_FILES_UPDATED = "event_files_updated";
const std::string MFileUtils::EVENT_FILES_UPDATE_FAILED = "event_files_update_failed";
const std::string MFileUtils::EVENT_FILE_DOWNLOADED = "event_files_downloaded";

MFileUtils::MFileUtils()
{
//    _reqFileList = nullptr;
    _counterUpdated = 0;
}

bool MFileUtils::setPathAndSyncFiles(cocos2d::CCDictionary *resMap, bool needSync)
{
    std::vector<std::string> searchPaths;
    searchPaths.push_back(getAbsPath(kMFileTypeResource));
    
    if (needSync)
    {
        FILE_UTILS->clearDirectories(kMFileTypeResource, resMap->allKeys());
    }
    
    CCDictElement *pElement = nullptr;
    CCDICT_FOREACH(resMap, pElement)
    {
        std::string dstPath = ((CCString *)pElement->getObject())->getCString();
        
        searchPaths.push_back(getAbsPath(kMFileTypeResource) + dstPath);
        searchPaths.push_back(pElement->getStrKey());
    }
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    
    return true;
}

std::string MFileUtils::getFullPath(std::string path)
{
    return CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str());
}

std::string MFileUtils::getFullPath(std::string path, MFileType_t type)
{
    return getAbsPath(type) + path;
}

void MFileUtils::updateFiles(std::string fileListUrl, std::string baseUrl, bool isInBackground)
{
    auto req = MHttpRequest::Request(fileListUrl);
    req->onSuccess([this, baseUrl, isInBackground](MHttpRequest *req) {
//        _reqFileList = nullptr;
        
        FILE_UTILS->resolveFileListData(req->response()->responseData(), baseUrl, isInBackground);
    });
    req->onError([this](MHttpRequest *req) {
//        _reqFileList = nullptr;
        
        M_DEBUG("update error: " << req->url());
        FILE_UTILS->dispatchEvent(EVENT_FILES_UPDATE_FAILED);
        req->response()->isErrorHandled() = true;
    });
    req->send();
    
//    _reqFileList = req;
}

void MFileUtils::resolveFileListData(std::string data, std::string baseUrl, bool isInBackground)
{
    if (data.size() == 0)
    {
        dispatchEvent(EVENT_FILES_UPDATED);
        return;
    }
    
    std::vector<std::string> urllist;
    
    Json::Reader reader;
    Json::Value val;
    if (reader.parse(data, val))
    {
        if (val.type() == Json::arrayValue)
        {
            Json::UInt size = val.size();
            for (Json::UInt i=0; i < size; ++i)
            {
                std::string filePath = val[i]["name"].asString();
                std::string sig = val[i]["sig"].asString();
                
                std::string sigFile = getFileSignature(filePath);
                
                if (sigFile != sig
                    && !(isInBackground && sigFile.empty()))
                {
                    urllist.push_back(filePath);
                    M_INFO("will download " << filePath);
                    M_INFO("sig: " << sig << "  will be: " << getFileSignature(filePath) );
                }
            }
        }
    }
    else
    {
        dispatchEvent(EVENT_FILES_UPDATE_FAILED);
        return;
    }
    
    
    if (urllist.size() == 0)
    {
        dispatchEvent(EVENT_FILES_UPDATED);
        return;
    }
    
    
    for (auto r : _requests)
    {
        if (r != nullptr)
        {
            r->cancel();
        }
    }
    
    _requests.clear();
    _counterUpdated = 0;
    mlib::MHttpRequest * req = nullptr;
    
    for (auto i: urllist)
    {
        req = MHttpRequest::Request(baseUrl + i);
        req->timeoutInSeconds(3600);
        req->networkTimeout(600);
        if (isInBackground)
        {
            req->priority(MHttpRequest::LOW);
        }
        
        req->context() << i;
        _requests.push_back(req);
        req->addEventListener(MHttpRequest::EVENT_FINISHED, [this](MEvent * evt) {
            const MHttpRequest * req = (const MHttpRequest *)evt->target();
            
            _counterUpdated++;
            
            if (req->isSuccess())
            {
                MEvent evt(EVENT_FILE_DOWNLOADED);
                evt.buffer() << (uint32_t)_requests.size()
                << ((_counterUpdated < _requests.size()) ? (_counterUpdated+1) : _counterUpdated);
                FILE_UTILS->dispatchEvent(evt);
                
                // save file
                std::string filePath;
                req->context().seekBegin();
                req->context() >> filePath;
                FILE_UTILS->saveFile(req->response()->responseData(), filePath, kMFileTypeResource);
                
                if (_counterUpdated == _requests.size())
                {
                    // do end
                    _requests.clear();
                    FILE_UTILS->dispatchEvent(EVENT_FILES_UPDATED);
                    return;
                }
                else
                {
                    auto iter = std::find(_requests.begin(), _requests.end(), req);
                    if (iter != _requests.end())
                    {
                        *iter = nullptr;
                    }
                }
            }
            else
            {
                _requests.clear();
                FILE_UTILS->dispatchEvent(EVENT_FILES_UPDATE_FAILED);

                return;
            }
        });
    }
    
    MEvent evt(EVENT_FILE_DOWNLOADED);
    evt.buffer() << (uint32_t)_requests.size() << (0+1);
    FILE_UTILS->dispatchEvent(evt);
    
    for (auto r : _requests)
    {
        r->send();
    }
}

bool MFileUtils::saveFile(std::string data, std::string relaPath, MFileType_t type)
{
    std::string filePathName = getAbsPath(type) + relaPath;
    
//    M_DEBUG("savefile: " << filePathName);
    
    FILE *file = fopen(filePathName.c_str(), "w");
    if (file == nullptr)
    {
        if (ENOENT == errno)
        {
            std::string::size_type pos = filePathName.find_last_of('/');
            std::string dirPath = filePathName.substr(0, pos+1);
            
            std::string::size_type lastPos = 0;
            while(1)
            {
                pos = dirPath.find('/', lastPos);
                if (pos == std::string::npos)
                {
                    break;
                }
                lastPos = pos+1;
                if (pos == 0)
                {
                    continue;
                }

                std::string path = dirPath.substr(0, pos);
                if (access(path.c_str(), F_OK) != 0)
                {
                    mode_t processMask = umask(0);
                    int ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
                    umask(processMask);
                    if (ret != 0 && (errno != EEXIST))
                    {
                        M_ERROR("mkdir " << path << " error No. " << errno << "  ret: " << ret);
                        return false;
                    }
                }
            }

            file = fopen(filePathName.c_str(), "w");
        }
    }
    
    if (file == nullptr)
    {
        // error
        M_INFO("saveFile error No.: " << errno);
        return false;
    }
    else
    {
        fwrite(data.data(), sizeof(const char), data.size(), file);
        fclose(file);
        return true;
    }
}

std::string MFileUtils::getFileSignature(std::string relaPath)
{
    unsigned long size = 0;
    
    if (this->isFileExist(relaPath))
    {
        unsigned char *data = CCFileUtils::sharedFileUtils()->getFileData(relaPath.c_str(), "r", &size);
        
        if (data != nullptr)
        {
            std::string md5 = md5_string(std::string((const char *)data, size));
            delete [] data;
            return md5.substr(0,4);
        }
    }
    
    return "";
}

bool MFileUtils::isFileExist(std::string relaPath)
{
    return CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(relaPath.c_str()));
}

//void MFileUtils::clearDirectory(MFileType_t type)
//{
//    clearDirectory(getAbsPath(type));
//}

void MFileUtils::clearDirectories(MFileType_t type, cocos2d::CCArray *arrResPath)
{
    std::string path = getAbsPath(type);
    
    CCObject *obj = nullptr;
    CCARRAY_FOREACH(arrResPath, obj)
    {
        CCString *resPath = (CCString *)obj;
        clearDirectory(path + resPath->getCString());
    }
}

float MFileUtils::getUpdateProgress()
{
    if (_requests.empty())
    {
        return 0;
    }
    
    float total = _requests.size();
    float prog = _counterUpdated / total;
    
    float downloading = 0;
    double dlNow = 0;
    double dlTotal = 0;
    for (auto req : _requests)
    {
        if (req != nullptr)
        {
//            M_DEBUG("req: " << (unsigned)req << " downloadNow: " << req->downloadNow() << " dlTotal: " << req->downloadTotal());
            
            if (req->prog() > 0)
            {
                downloading++;
                dlNow += req->downloadNow();
                dlTotal += req->downloadTotal();
            }
        }
    }
    
//    M_DEBUG("dlnow: " << dlNow << " dltotal: " << dlTotal);
    
    if (downloading > 0)
    {
        prog += dlNow / dlTotal * downloading / total;
    }
    
    return prog;
}

MLIB_NS_END
