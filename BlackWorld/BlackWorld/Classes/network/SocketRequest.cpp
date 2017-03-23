#include "SocketRequest.h"
//#include "LMemory.h"
#include "SocketErrorHandler.h"

using namespace mlib;

#pragma mark LSocketRequest

SocketRequest::SocketRequest(const std::string& host, const int port) : MSocketRequest::MSocketRequest(host, port)
{
    _timeoutInSeconds = 5;
}

MSocketResponse * SocketRequest::createResponse(unsigned short statusCode, const char * data, size_t size)
{
    runInMainThread([&](){
        if (!isBackground())
        {
//            MScreenManager::closePopup(_loadingView);
        }
    });
    
    SocketResponse * resp = new SocketResponse(statusCode, data, size);
    
    return resp;
}

SocketResponse * SocketRequest::parsedResponse() const
{
    return dynamic_cast<SocketResponse *>(_response);
}

void SocketRequest::send()
{
    if(this->isBackground())
    {
        this->priority(NORMAL);
    }
    else
    {
        this->priority(HIGH);
    }
    
    MSocketRequest::send();
    
    if (!this->isBackground())
    {
        M_DEBUG("loadingview popup");
    }
}



#pragma mark LSocketResponse

SocketResponse::SocketResponse(unsigned short statusCode, const char * data, size_t size) : MSocketResponse(statusCode, data, size)
{
    mlib::runInMainThread([this, statusCode] () {
        switch (statusCode) {
            case OK:
            {
                Json::Reader reader;
                CCLog("responseData : %s", this->responseData().c_str());
                if (reader.parse(this->responseData(), _jsonValue))
                {
                    if (!_jsonValue.isObject())
                    {
                        _returnCode = SocketErrorHandler::ERROR_PARSE_JSON;
                        CCLog("ERROR_PARSE_JSON_NOT_OBJECT");
                    }
                    else
                    {
                        _returnCode = 0;
                    }
                }
                else
                {
                    _returnCode = SocketErrorHandler::ERROR_PARSE_JSON;
                    CCLog("ERROR_PARSE_JSON");
                }
            }break;
                
            case ERROR:
            {
                
            }break;
                
            default:
            {
                _returnCode = SocketErrorHandler::ERROR_NETWORK;
            }break;
        }
    });
}

SocketResponse::~SocketResponse()
{
    
}

bool SocketResponse::isValid()
{
    return (_returnCode == 0);
}
