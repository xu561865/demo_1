//
//  MFileUtils.h
//  legend
//
//  Created by Xu Jinyang on 13-3-14.
//
//

#ifndef __mlib__MFileUtilsIOS__
#define __mlib__MFileUtilsIOS__

#include "MFileUtils.h"

MLIB_NS_BEGIN

class MFileUtilsIOS : public MFileUtils
{
public:
    virtual bool copyFilesToWriteablePath(std::string srcRelaPath, std::string dstRelaPath);
    virtual std::string getAbsPath(MFileType_t type);
    virtual void clearDirectory(std::string path);
};

MLIB_NS_END

#endif /* defined(__mlib__MFileUtilsIOS__) */
