//
//  MAppManager.h
//  legend
//
//  Created by Xu Jinyang on 3/21/13.
//
//

#ifndef __legend__MAppManager__
#define __legend__MAppManager__

#include <iostream>
#include "mlib_helpers.h"

class MAppManager
{
public:
    MAppManager() {}
    virtual ~MAppManager() {}
    
    static MAppManager * sharedManager();
    
    MLIB_DECLARE_PROPERTY(std::string, remoteConfigURL);
    MLIB_DECLARE_PROPERTY(std::string, resBaseUrl);
    MLIB_DECLARE_PROPERTY(std::string, specBaseUrl);
    MLIB_DECLARE_PROPERTY(std::string, resFileList);
    MLIB_DECLARE_PROPERTY(std::string, specFileList);
    MLIB_DECLARE_PROPERTY(std::string, specVersion);
    
    MLIB_DECLARE_PROPERTY(std::string, appStoreUrl);
};

#define APP_MANAGER (MAppManager::sharedManager())

#endif /* defined(__legend__MAppManager__) */
