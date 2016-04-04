#ifndef SOCKET_CENTER_H
#define SOCKET_CENTER_H

#include "mlib.h"

#include "SocketRequest.h"
#include "Json.h"
#include <string>

class SocketCenter
{
public:
    static SocketCenter * SharedInstance();
    MLIB_DECLARE_PROPERTY(std::string, host);
    MLIB_DECLARE_PROPERTY(int, port);
    
public:
    
    SocketRequest * login(Json::Value&);
    
protected:
    SocketRequest * createRequest(bool isSigned = true, bool isBackground = true);
    void preLogicRequestHandler(mlib::MEvent * evt);
    void postLogicRequestHandler(mlib::MEvent * evt);
    

};



#endif // SOCKET_CENTER_H
