//
//  MFileDownloader.cpp
//  legend
//
//  Created by Xu Jinyang on 6/13/13.
//
//

#include "MFileDownloader.h"
#include "MAppManager.h"

using namespace mlib;
using namespace std;

MFileDownloader::MFileDownloader()
{
    
}

MFileDownloader::~MFileDownloader()
{
    for (auto i : _requests)
    {
        i->cancel();
//        M_DEBUG("req cancelled: " << (long)i);
    }
}

bool MFileDownloader::isFileExist(std::string file)
{
    return FILE_UTILS->isFileExist(getLocalFilePath(file));
}

std::string MFileDownloader::getLocalFilePath(std::string file)
{
    string::size_type pos;
    if (string::npos == (pos = file.find("://")))
    {
        return file;
    }
    else
    {
        string filePath = file.substr(pos + strlen("://"));
        filePath = mlib::string_replace(filePath, "/", "_");
        filePath = "http/" + filePath;
        return filePath;
    }
}

std::string MFileDownloader::getUrlForFile(std::string file)
{
    if (string::npos == file.find("://"))
    {
        return (APP_MANAGER->resBaseUrl() + file);
    }
    else
    {
        return file;
    }
}


void MFileDownloader::download(std::vector<std::string> urls, mlib::MFileType_t fileType)
{
    if (!_requests.empty())
    {
        M_ERROR("download should only be called once!");
        return;
    }
    
    _urls = urls;
    
    mlib::MHttpRequest *req = nullptr;
    
    _requests.clear();
    _fileDownloadCounter = 0;
    _downloadFailed = false;
    for (auto i: _urls)
    {
        req = MHttpRequest::Request(getUrlForFile(i));
        req->priority(MHttpRequest::NORMAL);
        
        req->context() << getLocalFilePath(i);
        _requests.insert(req);
        
        req->addEventListener(MHttpRequest::EVENT_FINISHED, [this, fileType](MEvent * evt) {
            MHttpRequest * req = (MHttpRequest *)evt->target();
            
            _fileDownloadCounter++;
            
            _requests.erase(req);
            if (req->isSuccess())
            {
                // save file
                std::string filePath;
                req->context().seekBegin();
                req->context() >> filePath;
                FILE_UTILS->saveFile(req->response()->responseData(), filePath, fileType);
                
                if (_fileDownloadCounter == _urls.size())
                {
                    // do end
                    this->dispatchEvent(EVENT_FILE_DOWNLOADED);
                    M_ASSERT(_requests.size() == 0);
                    delete this;
                    return;
                }
            }
            else
            {
                if (!_downloadFailed)
                {
                    this->dispatchEvent(EVENT_FILE_DOWNLOAD_FAILED);
                    delete this;
                    return;
                }
                _downloadFailed = true;
            }
        });
    }
    
    for (auto r:_requests)
    {
        r->send();
    }
}
