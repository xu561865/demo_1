#ifndef lsocketcenter_h
#define lsocketcenter_h

#include "mlib.h"

#include "LSocketRequest.h"
#include "Json.h"

class LSocketCenter
{
public:
    static LSocketCenter * SharedInstance();
    
    MLIB_DECLARE_PROPERTY(std::string, api);
    
public:
    
    LSocketRequest * login(Json::Value&);
    
protected:
    LSocketRequest * createRequest(bool isSigned = true, bool isBackground = false);
    void preLogicRequestHandler(mlib::MEvent * evt);
    void postLogicRequestHandler(mlib::MEvent * evt);
    

};



#endif // lsocketcenter_h
