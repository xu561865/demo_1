//
//  MFileUtils.h
//  legend
//
//  Created by Xu Jinyang on 13-3-14.
//
//

#ifndef __mlib__MFileUtils__
#define __mlib__MFileUtils__

#include <iostream>
#include <string>
#include "mlib_helpers.h"
#include "cocos2d.h"
#include "mlib.h"

MLIB_NS_BEGIN

typedef enum {
    kMFileTypeResource = 1,
    kMFileTypeCache = 2,
} MFileType_t;

class MFileUtils : public MEventDispatcher
{
public:
    static const std::string EVENT_FILES_UPDATED;
    static const std::string EVENT_FILES_UPDATE_FAILED;
    static const std::string EVENT_FILE_DOWNLOADED;
    
public:
    MFileUtils();
    virtual ~MFileUtils() {};
    
public:
    static MFileUtils * sharedFileUtils();
    
    bool setPathAndSyncFiles(cocos2d::CCDictionary *resMap, bool needSync);
    std::string getFullPath(std::string path);
    std::string getFullPath(std::string path, MFileType_t type);
    void updateFiles(std::string fileListUrl, std::string baseUrl, bool isInBackground = false);
    
//    virtual bool copyFilesToWriteablePath(std::string srcRelaPath, std::string dstRelaPath) = 0;
    virtual std::string getAbsPath(MFileType_t type) = 0;
    
    bool saveFile(std::string data, std::string relaPath, MFileType_t type);
    
    std::string getFileSignature(std::string relaPath);
    bool isFileExist(std::string relaPath);
    
//    void clearDirectory(MFileType_t type);
    virtual void clearDirectory(std::string path) = 0;
    
    void clearDirectories(MFileType_t type, cocos2d::CCArray *arrResPath);
    
    float getUpdateProgress();
    
private:
    // 背景模式只会下载所有本地已有文件里需要更新的
    void resolveFileListData(std::string data, std::string baseUrl, bool isInBackground);
    
//    MHttpRequest *_reqFileList;
    std::vector<MHttpRequest *> _requests;
    uint32_t _counterUpdated;
};

#define FILE_UTILS (mlib::MFileUtils::sharedFileUtils())

MLIB_NS_END

#endif /* defined(__mlib__MFileUtils__) */
