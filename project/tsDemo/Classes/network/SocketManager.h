#ifndef __client1__SocketManager__
#define __client1__SocketManager__


#include "cocos2d.h"
#include "message.h"
#include "SocketClient.h"
#include "Json.h"

USING_NS_CC;

class SocketManager
{
public:
    static SocketManager* getInstance();
    void startSocket();
    void sendMessage(const char* data,int commandId);
    void sendMessage(Json::Value, int commandId);
    void sendMessage(std::string);
    
private:
    SocketManager();
    ~SocketManager();
    
    SocketClient* _socket;
};


#endif /* defined(__client1__SocketManager__) */
