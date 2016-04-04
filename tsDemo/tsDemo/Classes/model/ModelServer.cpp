#include "ModelServer.h"


USING_NS_CC_EXT;

ModelServer * ModelServer::serverInfoFromDict(CCDictionary *dict)
{
    ModelServer *obj = ModelServer::create();
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

std::string ModelServer::api()
{
    return url() + "index.php";
}

std::string ModelServer::configUrl()
{
    return url() + "server_configs.json";
}

bool ModelServer::isBusy()
{
    return serverStatus() == kServerStatus_Busy;
}

