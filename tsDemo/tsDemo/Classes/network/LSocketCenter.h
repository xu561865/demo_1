#ifndef lsocketcenter_h
#define lsocketcenter_h

#include "mlib.h"

#include "LSocketRequest.h"
#include "Json.h"
#include <string>

class LSocketCenter
{
public:
    static LSocketCenter * SharedInstance();
    MLIB_DECLARE_PROPERTY(std::string, host);
    MLIB_DECLARE_PROPERTY(int, port);
    
public:
    
    LSocketRequest * login(Json::Value&);
    
protected:
    LSocketRequest * createRequest(bool isSigned = true, bool isBackground = true);
    void preLogicRequestHandler(mlib::MEvent * evt);
    void postLogicRequestHandler(mlib::MEvent * evt);
    

};



#endif // lsocketcenter_h
