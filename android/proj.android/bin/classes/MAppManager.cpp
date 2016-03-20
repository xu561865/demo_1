//
//  MAppManager.cpp
//  legend
//
//  Created by Xu Jinyang on 3/21/13.
//
//

#include "MAppManager.h"

static MAppManager *AppManagerInstance = nullptr;
MAppManager * MAppManager::sharedManager()
{
    if (AppManagerInstance == NULL)
    {
        AppManagerInstance = new MAppManager();
        AppManagerInstance->remoteConfigURL("http://localhost/app.xml");
    }
    return AppManagerInstance;
}
