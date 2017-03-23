#include "ModelUser.h"

#include "Memory.h"


ModelUser::ModelUser()
{
    
}

ModelUser::~ModelUser()
{
    
}

ModelUser * ModelUser::createWithJson(Json::Value json)
{
    ModelUser *user = ModelUser::create();
    user->parseJson(json);
    
    return user;
}

void ModelUser::parseJson(Json::Value json)
{
    this->userId(json["user_id"].asUInt());
    this->nickname(json["nickname"].asString());

    this->level(json["level"].asUInt());
    this->exp(json["exp"].asUInt());
    this->coin(json["coin"].asInt());
    this->cash(json["cash"].asInt());
    this->vipTime(json["vip_time"].asUInt());

}