//
//  MFileUtils.cpp
//  legend
//
//  Created by Xu Jinyang on 13-3-14.
//
//

#include "MFileUtils.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

USING_NS_CC;

MLIB_NS_BEGIN

class MFileUtilsAndroid : public MFileUtils
{
public:
    virtual std::string getAbsPath(MFileType_t type);
    virtual void clearDirectory(std::string path);
};

static MFileUtils *FileUtilsInstance = nullptr;
MFileUtils * MFileUtils::sharedFileUtils()
{
    if (FileUtilsInstance == NULL)
    {
        FileUtilsInstance = new MFileUtilsAndroid();
    }
    return FileUtilsInstance;
}

std::string MFileUtilsAndroid::getAbsPath(MFileType_t type)
{
    std::string dir = "";
    switch(type)
    {
        case kMFileTypeResource:
        {
            dir = "resources";
            break;
        }
        default:
        {
            dir = "cache";
            break;
        }
    }

    std::string p = CCFileUtils::sharedFileUtils()->getWritablePath() + dir + "/";
    return CCFileUtils::sharedFileUtils()->getWritablePath() + dir + "/";
}

void MFileUtilsAndroid::clearDirectory(std::string path)
{
    deleteFilesJNI(path.c_str());
}

MLIB_NS_END
