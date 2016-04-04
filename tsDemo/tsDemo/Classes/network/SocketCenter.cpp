#include "SocketCenter.h"
#include "SocketErrorHandler.h"
//#include "LMemory.h"

#include <sstream>


SocketCenter * SocketCenter::SharedInstance()
{
    static SocketCenter inst;
    return &inst;
}

SocketRequest * SocketCenter::createRequest(bool isSigned, bool isBackground)
{
    SocketRequest * req = SocketRequest::Request(host(), port());
    
    req->isBackground(isBackground);
    
    req->addEventListener(SocketRequest::EVENT_FINISHED, MLIB_EVENT_CALLBACK(&SocketCenter::preLogicRequestHandler), this, INT_MIN); // highest priority
    req->addEventListener(SocketRequest::EVENT_FINISHED, MLIB_EVENT_CALLBACK(&SocketCenter::postLogicRequestHandler), this, INT_MAX); // lowest priority
    
    return req;
}

void SocketCenter::preLogicRequestHandler(mlib::MEvent *evt)
{
    auto req = dynamic_cast<const SocketRequest *>(evt->target());

    auto json = req->parsedResponse()->jsonValue();
    
 
    if (req->parsedResponse()->returnCode() != SocketErrorHandler::ERROR_NETWORK)
    {

    }
    
    if (req->parsedResponse()->returnCode() == SocketErrorHandler::ERR_SERVER_MESSAGE)
    {
        auto msg = json["message"].asString(),
        title = json["title"].asString();
        auto behavior = json["behavior"].asUInt();
        
        SocketErrorHandler::showServerMessage(msg, title, behavior);
        evt->cancel();
    }
    else if (req->parsedResponse()->returnCode() == SocketErrorHandler::ERR_CLIENT_NEEDS_UPDATE)
    {
        auto version = json["version"].asString();
        
        evt->cancel();
    }
}


void SocketCenter::postLogicRequestHandler(mlib::MEvent *evt)
{
    auto req = dynamic_cast<const SocketRequest *>(evt->target());
    
    if (req->isSuccess())
    {
        
    }
    else 
    {
        M_ERROR("response error: " << req->parsedResponse()->responseData());
        if (!req->response()->isErrorHandled())
        {
            auto rc = req->parsedResponse()->returnCode();
            auto json = req->parsedResponse()->jsonValue();
            SocketErrorHandler::handlerError(rc, json, req->isBackground());
        }
    }
}




SocketRequest * SocketCenter::login(Json::Value& value)
{
    auto req = this->createRequest(false);

    req->setParameter(value);
    
    auto handler = [req] (mlib::MEvent * evt) {
        if (req->isSuccess())
        {
            Json::Value root = req->parsedResponse()->jsonValue();
            Json::Value data = root["ResRegMsg"];
            int ret = data["ResState"].asInt();   //0: 成功  1:server error   2:已注册
            CCLog("%d", ret);

        }
    };
    
    req->addEventListener(SocketRequest::EVENT_FINISHED, handler);
    
    return req;
}
