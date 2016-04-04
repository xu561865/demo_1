#include "ModuleServer.h"


USING_NS_CC_EXT;

ModuleServer * ModuleServer::serverInfoFromDict(CCDictionary *dict)
{
    ModuleServer *obj = ModuleServer::create();
    obj->serverId(dict->valueForKey("serverId")->uintValue());
    obj->serverName(dict->valueForKey("serverName")->getCString());
    obj->url(dict->valueForKey("url")->getCString());
    
    auto serverStatus = dict->valueForKey("serverStatus");
    if (serverStatus->length() <= 0)
    {
        obj->serverStatus(kServerStatus_Normal);
    }
    else
    {
        obj->serverStatus(serverStatus->uintValue());
    }
    
    return obj;
}

std::string ModuleServer::api()
{
    return url() + "index.php";
}

std::string ModuleServer::configUrl()
{
    return url() + "server_configs.json";
}

bool ModuleServer::isBusy()
{
    return serverStatus() == kServerStatus_Busy;
}

