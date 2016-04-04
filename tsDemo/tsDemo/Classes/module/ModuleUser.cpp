#include "ModuleUser.h"

#include "Memory.h"


ModuleUser::ModuleUser()
{
    
}

ModuleUser::~ModuleUser()
{
    
}

ModuleUser * ModuleUser::createWithJson(Json::Value json)
{
    ModuleUser *user = ModuleUser::create();
    user->parseJson(json);
    
    return user;
}

void ModuleUser::parseJson(Json::Value json)
{
    this->userId(json["user_id"].asUInt());
    this->nickname(json["nickname"].asString());

    this->level(json["level"].asUInt());
    this->exp(json["exp"].asUInt());
    this->coin(json["coin"].asInt());
    this->cash(json["cash"].asInt());
    this->vipTime(json["vip_time"].asUInt());

}