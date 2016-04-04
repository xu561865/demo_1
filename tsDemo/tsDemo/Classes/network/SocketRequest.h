#ifndef SOCKET_REQUEST_H
#define SOCKET_REQUEST_H

#include "mlib_socket.h"
#include "json.h"
#include "mlib_ccext.h"

class SocketResponse : public mlib::MSocketResponse
{
public:
    
    SocketResponse(unsigned short statusCode, const char * data, size_t size);
    SocketResponse(const SocketResponse&) = delete;
    ~SocketResponse();
    
    virtual bool isValid();
    
    MLIB_DECLARE_READONLY_PROPERTY(unsigned short, returnCode);
    MLIB_DECLARE_READONLY_PROPERTY(Json::Value, jsonValue);
};


class SocketRequest : public mlib::MSocketRequest
{
public:
    static SocketRequest * Request(const std::string& host, const int& port)
    {
        return new SocketRequest(host, port);
    }
    
    SocketResponse * parsedResponse() const;
    
    virtual void send();
    
    MLIB_DECLARE_PROPERTY(bool, isBackground);
    
protected:
    SocketRequest(const std::string& host, const int port);
    virtual mlib::MSocketResponse * createResponse(unsigned short statusCode, const char * data, size_t size);
    
};


#endif // SOCKET_REQUEST_H
