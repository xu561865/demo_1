#include "SocketManager.h"


static SocketManager* instance = NULL;

SocketManager::SocketManager()
{
    
}

SocketManager* SocketManager::getInstance()
{
    if(instance == NULL)
    {
        instance = new SocketManager();
    }
    
    return instance;
}

void SocketManager::startSocket()
{
//    _socket = new SocketClient("192.168.1.111",11009,1,1);
    _socket = new SocketClient("182.92.129.73",3563,1,1);
    _socket->start();
}

void SocketManager::sendMessage(const char* data, int commandId)
{
    Message *msg=_socket->constructMessage(data, commandId);
    _socket->sendMessage_(msg, false);
}




