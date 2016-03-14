//
//  MFileUtils.cpp
//  legend
//
//  Created by Xu Jinyang on 13-3-14.
//
//

#include "MFileUtilsIOS.h"
#import <Foundation/Foundation.h>

USING_NS_CC;

MLIB_NS_BEGIN

static MFileUtils *FileUtilsInstance = nullptr;
MFileUtils * MFileUtils::sharedFileUtils()
{
    if (FileUtilsInstance == NULL)
    {
        FileUtilsInstance = new MFileUtilsIOS();
    }
    return FileUtilsInstance;
}


std::string MFileUtilsIOS::getAbsPath(MFileType_t type)
{
    NSSearchPathDirectory dir = NSCachesDirectory;
    switch (type) {
        case kMFileTypeResource:
            dir = NSDocumentDirectory;
            break;
        case kMFileTypeCache:
            dir = NSCachesDirectory;
            break;
            
        default:
            break;
    }
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(dir, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
}

bool MFileUtilsIOS::copyFilesToWriteablePath(std::string srcRelaPath, std::string dstRelaPath)
{
    NSFileManager * fm = [NSFileManager defaultManager];
    NSError *err = nil;
    NSBundle *bundle = [NSBundle mainBundle];
    
    NSString *srcPath = [NSString stringWithFormat:@"%@/%@", bundle.resourcePath, [NSString stringWithUTF8String:srcRelaPath.c_str()]];
    std::string strDstPath = getAbsPath(kMFileTypeResource) + dstRelaPath;
    NSString *dstPath = [NSString stringWithUTF8String:strDstPath.c_str()];
    if ([fm fileExistsAtPath:dstPath])
    {
        [fm removeItemAtPath:dstPath error:&err];
        if (err) return false;
    }
    
    NSString * dstParent = [dstPath stringByDeletingLastPathComponent];
    err = nil;
    [fm createDirectoryAtPath:dstParent withIntermediateDirectories:YES attributes:nil error:&err];
    if (err) return false;
    
    err = nil;
    [fm copyItemAtPath:srcPath toPath:dstPath error:&err];
    if (err) return false;
    
    return true;
}

void MFileUtilsIOS::clearDirectory(std::string path)
{
    NSFileManager *fileMgr = [[[NSFileManager alloc] init] autorelease];
    NSError *error = nil;
    NSString *dir = [NSString stringWithUTF8String:path.c_str()];
    NSArray *directoryContents = [fileMgr contentsOfDirectoryAtPath:dir error:&error];
    if (error == nil) {
        for (NSString *path in directoryContents) {
            NSString *fullPath = [dir stringByAppendingPathComponent:path];
            BOOL removeSuccess = [fileMgr removeItemAtPath:fullPath error:&error];
            if (!removeSuccess) {
                // Error handling
                M_ERROR("Error occurred in MFileUtilsIOS::clearDirectory");
            }
        }
    } else {
        // Error handling
        M_ERROR("Error occurred in MFileUtilsIOS::clearDirectory");
    }
}

MLIB_NS_END
