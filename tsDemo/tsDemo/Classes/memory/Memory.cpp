#include "Memory.h"
#include "MFileUtils.h"
#include "cocos2d.h"
#include "json.h"

#include <random>
#include <algorithm>

USING_NS_CC;


#define  MAX_CHAT 30


std::vector<std::string> LMemory::g_tips = {
    "角色升级会提升领导力上限和包裹容量上限。",
    "为装备镶嵌合适的符石，能大幅度提升您的实力。",
    "只有队长卡牌的队长技能，才能对小队产生作用。",
    "羁绊组合中的卡牌同时上阵，会触发相应的属性加成。",
    "您的挑战排名越高，每次挑战获得的勋章数量就越多。",
    "挑战商店中，可以用勋章兑换符石。",
    "向忍者村捐献铜币或者金币，可以提升忍者村贡献。",
    "忍者村商店中，可以用忍者村贡献兑换高级装备。",
    "四块同名的符石可以合成一块更高级的符石。",
    "一般情况下，防守型的卡牌应该放在前排，为其他卡牌抵挡伤害。",
    "普通卡牌每4回合的第2回合释放主动技能，“+1”“+2”“+3”卡牌每3回合的第1回合释放主动技能。",
    "要想把忍者进阶到5星，需要提供“等级证书”。",
    "历练副本中所有关卡均获得S级评价，可以获得金币奖励。",
    "您可以让您的忍者卡牌学习新的队长技能。",
    "输入同服邀请码，双方均有奖励获得。",
    "每天可以领取每日登录奖励，连续登录天数越多领到越多。",
    "连续保护大名3天，可以获得丰厚的奖励。",
    "每天可以免费为尾兽刷新一次战吼。",
    "集齐召唤卷轴可以召唤出尾兽，帮助您一起战斗。",
    "每天18点参加“晓”的入侵活动，可以获得大量的铜币。",
    "卡牌升级时吞噬满级卡牌，可以提供双倍经验。",
    "分解尾兽会全额返还升级该尾兽所消耗的能量。",
    "摇钱树有几率暴击，获得十倍奖励。",
};

LMemory * LMemory::SharedInstance()
{
    static LMemory inst;
    return &inst;
}

LMemory::LMemory()
{
    reset();
}

LMemory::~LMemory()
{
    
}

void LMemory::reset()
{

}

void LMemory::saveBasic()
{
    CCUserDefault * def = CCUserDefault::sharedUserDefault();
    
//    Json::Value val(Json::ValueType::objectValue);
//    val["accountId"] = _accountId;
//    val["password"] = _passwordHashed;
//    val["lastServerId"] = _lastServerId;
//    val["musicOn"] = _musicOn;
//    val["effectOn"] = _effectOn;
//
//    
//    Json::Value val2(Json::ValueType::arrayValue);
//    for (auto tr:_pendingTransactions)
//    {
//        Json::Value val3(Json::ValueType::objectValue);
//        val3["p"] = tr.productId;
//        val3["t"] = tr.transactionId;
//        val3["r"] = tr.receipt;
//
//        val2[val2.size()] = val3;
//    }
//    val["pendingTransactions"] = val2;
    
    
//    def->setStringForKey("lmemory_basic", SSTR(val));
    def->flush();
}

bool LMemory::loadBasic()
{
    CCUserDefault * def = CCUserDefault::sharedUserDefault();
    
    std::string saved = def->getStringForKey("lmemory_basic");
    Json::Value val;
    Json::Reader reader;
    if (!reader.parse(saved, val))
    {
        M_WARNING("error reading user default");
        return false;
    }
    
//    _accountId = val["accountId"].asUInt();
//    _passwordHashed = val["password"].asString();
//    _lastServerId = val["lastServerId"].asUInt();
//    _musicOn = val["musicOn"].asBool();
//    _effectOn = val["effectOn"].asBool();
//    _weibo_name = val["weiboName"].asString();
//    _qq_name = val["qqName"].asString();
//    _facebook_name =val["facebookName"].asString();
//   
//    _weibo_id =  val["weiboId"].asString();
//    _facebook_id = val["facebookId"].asString();
//    _qq_id = val["qqId"].asString();
//    
//    _pendingTransactions.clear();
//    auto val2 = val["pendingTransactions"];
//    if (val2.isArray())
//    {
//        for (auto v : val2)
//        {
//            MIAPTransaction tr;
//            tr.productId = v["p"].asString();
//            tr.transactionId = v["t"].asString();
//            tr.receipt = v["r"].asString();
//            
//            _pendingTransactions.push_back(tr);
//        }
//    }

//    _userId = 0;
    

    return true;
}

void LMemory::save()
{
    CCUserDefault * def = CCUserDefault::sharedUserDefault();
    
//    Json::Value val(Json::ValueType::objectValue);
//    val["userId"] = _userId;
//    val["token"] = _token;
//    val["tokenExpireAt"] = _tokenExpireAt;
//    val["isNicknameSettingPrompted"] = _isNicknameSettingPrompted;
//    val["shouldShowCardDetail"] = _shouldShowCardDetail;
//    val["firstDailyLoginUse"] = _firstDailyLoginUse;
//    val["firstBlessingUse"] = _firstBlessingUse;
//    val["speedForBattle"] = _speedForBattle;
//    val["isAutoMove"] = _isAutoMove;
//    val["isFirstEnergyNotEnough"] = _isFirstEnergyNotEnough;
//    val["popUpSevenDayViewTime"] = _popUpSevenDayViewTime;
//    val["lastPopupEvents"] = _lastPopupEvents;
//    val["lastEventsHash"] = _lastEventsHash;
//    val["maxChatId"] = _maxChatId;
//    val["maxChatIdRead"] = _maxChatIdRead;
//    val["maxAppIdRead"] = _maxAppIdRead;
//    val["isDuelCountDown"] = _isDuelCountDown;
//    val["isFirstEnterPartnerView"] = _isFirstEnterPartnerView;
//    val["mountBattleStatus"] = _mountBattleStatus;
//    val["hasMountGuide"] = _hasMountGuide;
//
//    Json::Value mopup(Json::ValueType::arrayValue);
//
//    for (auto iter : _mopupStagesVec)
//    {
//        Json::Value valMop(Json::ValueType::objectValue);
//        valMop["c"] = iter.campaignId;
//        valMop["s"] = iter.stageIndx;
//        valMop["t"] = iter.type;
//        
//        mopup[mopup.size()] = valMop;
//    }
//    val["mopupStagesVec"] = mopup;
//
//    Json::Value val2(Json::ValueType::arrayValue);
//    for (auto iter : _activitiesPassed)
//    {
//        Json::Value val3(Json::ValueType::objectValue);
//        val3["c"] = iter.campaignId;
//        val3["s"] = iter.stageId;
//        
//        val2[val2.size()] = val3;
//    }
//    val["activitiesPassed"] = val2;
//    
//    
//    Json::Value val3(Json::ValueType::arrayValue);
//    for (auto iter : _activitiesFirstOpenId)
//    {
//        Json::Value val4(Json::ValueType::objectValue);
//        val4["c"] = iter;
//        val3[val3.size()] = val4;
//    }
//    val["activitiesFirstOpenId"] = val3;
//    //save guild chat messages
//    saveGuildChatMessages(val);
//
//    std::string strKey = SSTR("lmemory_" << _lastServerId);
//    def->setStringForKey(strKey.c_str(), SSTR(val));
    def->flush();
}

bool LMemory::load()
{
    CCUserDefault * def = CCUserDefault::sharedUserDefault();
    
//    std::string saved = def->getStringForKey(SSTR("lmemory_" << _lastServerId).c_str());
//    Json::Value val;
//    Json::Reader reader;
//    if (!reader.parse(saved, val))
//    {
//        M_WARNING("error reading user default");
//        return false;
//    }
//    
//    _userId = val["userId"].asUInt();
//    _token = val["token"].asString();
//    _tokenExpireAt = val["tokenExpireAt"].asUInt();
//    _isNicknameSettingPrompted = val["isNicknameSettingPrompted"].asBool();
//    _shouldShowCardDetail = val["shouldShowCardDetail"].asBool();
//    _firstDailyLoginUse = val["firstDailyLoginUse"].asBool();
//    _firstBlessingUse = val["firstBlessingUse"].asBool();
//
//    _speedForBattle = val["speedForBattle"].asDouble();
//    _isAutoMove = val["isAutoMove"].asBool();
//    _isFirstEnergyNotEnough = val["isFirstEnergyNotEnough"].asBool();
//    _popUpSevenDayViewTime = val["popUpSevenDayViewTime"].asUInt();
//    _lastPopupEvents = val["lastPopupEvents"].asUInt();
//    _lastEventsHash = val["lastEventsHash"].asString();
//    _maxChatId = val["maxChatId"].asUInt();
//    _maxChatIdRead = val["maxChatIdRead"].asUInt();
//    _maxAppIdRead = val["maxAppIdRead"].asUInt();
//    _isDuelCountDown = val["isDuelCountDown"].asBool();
//    _isFirstEnterPartnerView = val["isFirstEnterPartnerView"].asBool();
//    _mountBattleStatus = val["mountBattleStatus"].asUInt();
//    _hasMountGuide = val["hasMountGuide"].asBool();
//    
//    _activitiesPassed.clear();
//    auto val2 = val["activitiesPassed"];
//    if (val2.isArray())
//    {
//        for (auto iter : val2)
//        {
//            LActivityPassed tmp;
//            tmp.campaignId = iter["c"].asUInt();
//            tmp.stageId = iter["s"].asUInt();
//            
//            _activitiesPassed.push_back(tmp);
//        }
//    }
//    
//    _activitiesFirstOpenId.clear();
//    auto val3 = val["activitiesFirstOpenId"];
//    if (val3.isArray())
//    {
//        for (auto iter : val3)
//        {
//            _activitiesFirstOpenId.push_back(iter["c"].asUInt());
//        }
//    }
//    
//    auto messageVal = val["guildchatmessages"];
//    loadGuildChatMessages(messageVal);
//    if(def->getStringForKey("inited_mopup_data") == SSTR(1))
//    {
//        auto mopup = val["mopupStagesVec"];
//        if (!mopup.empty())
//        {
//            _mopupStagesVec.clear();
//
//            for (auto iter : mopup) {
//                LMopupStageRecord tmp;
//                tmp.campaignId = iter["c"].asUInt();
//                tmp.stageIndx = iter["s"].asUInt();
//                tmp.type = iter["t"].asUInt();
//                _mopupStagesVec.push_back(tmp);
//            }
//        }
//        def->setStringForKey("inited_mopup_data", SSTR(1));
//
//    }

    return true;
}

bool LMemory::clear()
{
//    CCUserDefault * def = CCUserDefault::sharedUserDefault();
//    def->setStringForKey("lmemory_basic", "");
//    
//    for (uint32_t i = 1; i < 100; i++)
//    {
//        std::string key = SSTR("lmemory_" << i);
//        if ("false" != def->getStringForKey(key.c_str(), "false"))
//        {
//            def->setStringForKey(key.c_str(), "");
//        }
//    }
//    def->setStringForKey("inited_mopup_data", "");
//
//    def->flush();
    return true;
}

std::string LMemory::getCurAPI()
{
    ModelServer *info = getCurServer();
    if (info != nullptr)
    {
        return info->api();
    }
    
    M_ERROR("no api url");
    return "";
}

ModelServer * LMemory::getCurServer()
{
//    uint32_t serverId = 0;
//    if (_lastServerId == 0)
//    {
//        if (recommendedServerId() == 0)
//        {
//            return (LServerInfo *)_servers->lastObject();
//        }
//        else
//        {
//            serverId = recommendedServerId();
//        }
//    }
//    else
//    {
//        serverId = _lastServerId;
//    }
//    
//    CCObject *obj;
//    CCARRAY_FOREACH(_servers, obj)
//    {
//        LServerInfo *info = (LServerInfo *)obj;
//        if (info->serverId() == serverId)
//        {
//            return info;
//        }
//    }
    
    return nullptr;
}