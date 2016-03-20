//
//  MFileDownloader.h
//  legend
//
//  Created by Xu Jinyang on 6/13/13.
//
//

#ifndef __legend__MFileDownloader__
#define __legend__MFileDownloader__

#include <iostream>
#include "mlib_helpers.h"
#include "mlib.h"
#include "MFileUtils.h"

static std::string EVENT_FILE_DOWNLOADED = "event_file_downloaded";
static std::string EVENT_FILE_DOWNLOAD_FAILED = "event_file_download_failed";

MLIB_NS_BEGIN

class MFileDownloader : public mlib::MEventDispatcher
{
public:
    MFileDownloader();
    ~MFileDownloader();
    
    static bool isFileExist(std::string file);
    static std::string getLocalFilePath(std::string file);

    // should only be called once
    void download(std::vector<std::string> urls, mlib::MFileType_t fileType = kMFileTypeResource);
    
private:
    std::set<mlib::MHttpRequest *> _requests;
    std::vector<std::string> _urls;
    uint32_t _fileDownloadCounter;
    bool _downloadFailed;
    
    std::string getUrlForFile(std::string file);
};

MLIB_NS_END

#endif /* defined(__legend__MFileDownloader__) */
