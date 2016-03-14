//
//  LConstants.h
//  legend
//
//  Created by Xu Jinyang on 4/7/13.
//
//

#ifndef legend_LConstants_h
#define legend_LConstants_h

typedef enum
{
    kEffectPhysical = 1,
    kEffectMagical = 2,
    kEffectHeal = 3,
    kEffectMagicalAndHeal = 4,
    kEffectMoraleBoost = 11,
    kEffectMoraleDestroy = 12,
    kEffectGuard = 21,
    kEffectCurse = 22,
    kEffectProsper = 31,
    kEffectToxic = 32,
    kEffectBless = 41,
    kEffectSlow = 42,
    kEffectkEffectChaos = 51,
    kEffectRecover = 61,
} EffectType_t;

typedef enum
{
    kExtraEffectInvalid = 0,
    kExtraEffectVertigo = 1,    //眩晕
    kExtraEffectDivineShield = 2,    //圣盾
    kExtraEffectSuckBlood = 4,  //吸血
    kExtraEffectMonthly = 5,    //月读
    kExtraEffectTodetect = 6,    //侦破
} ExtraEffectType_t;

typedef enum
{
    kEffectModNone = 0,
    kEffectModPositive = 1,
    kEffectModNegative = 2,
} EffectModType_t;

typedef enum
{
    kDataTypeMain = 1,
    kDataTypeEquip = 2,
    kDataTypeProp = 3,
    kDataTypeCash = 11,
    kDataTypeCoin = 12,
    kDataTypeExp = 13,
    kDataTypeBadge = 14,
    kDataTypeEnergy = 15,
    kDataTypeSpirit = 16,
    kDataTypeVip= 17,
    kDataTypeSpiritSoul = 18,
    kDataTypeMount = 19,
    kDataTypeBraveBadge = 20,
    kDataTypeItem = 99,
    kDataTypeQuest = 100,
    kUserTypeProp = 1001
} DataType_t;

typedef enum
{
    kCostTypeCash = 1,
    kCostTypeCoin = 2,
    kCostTypeFriendshipPoint = 3,
    kCostTypeBadge = 4,
    kCostTypeContribution = 5,
    kCostTypeBravebadge = 6,
    kCostTypeTicket = 101,    //传送石 //by zhangcheng : 6 改为 101
} CostType_t;

typedef enum
{
    kGuildRoleNormal = 0,
    kGuildRoleFounder = 10,
} GuildRole_t;

typedef enum
{
    kGuildTechGuild = 1,
    kGuildTechShop = 2,
} GuildTech_t;

typedef enum
{
    kAppStatusPending = 0,
    kAppStatusApproved = 1,
    kAppStatusIgnored = 2,
} GuildApplicationStatus_t;

typedef enum
{
    kGuildEventFound = 1,
    kGuildEventJoin = 2,
    kGuildEventQuit = 3,
    kGuildEventOwnerTransfer = 4,
    kGuildEventLevelUp = 5,
    kGuildEventShopLevelUp = 6,
    kGuildEventImpeachmentNotify1 = 7,
    kGuildEventImpeachmentNotify2 = 8,
    kGuildEventImpeachmentDone = 9,
    kGuildEventBoughtBadge = 10,
    kGuildEventGetFreeBadge = 11,
    kGuildEventOpenBadge = 12,

} GuildEventType_t;

typedef enum {
    kShopShelfOffShelf = 0,
    
    kShopShelfNormal = 1,
    kShopShelfArena = 2,
    kShopShelfGuild = 4,
    kShopShelfCardPack = 8,
    kShopShelfGems = 16,
    kShopShelfFriendPoint = 32,
    kShopShelfVip = 64,
    kShopShelfBraveArena = 256,
} ShopShelf_t;

typedef enum {
    kQuestTypeLevel = 1,
    kQuestTypeDepositTotal = 2,
    kQuestTypeLoginContinuous = 3,
    kQuestTypeCampaign = 4,
    kQuestTypeCampaignElite = 5,
    kQuestTypeArenaRank = 6,
    kQuestTypeArenaDailyReward = 7,
    kQuestTypeGuildMemberSize = 14,
    kQuestTypeGuildLevel = 15,
    kQuestTypeMainUsed = 8,
    kQuestTypeEquipUsed = 9,
    kQuestTypeMounted = 10,
    kQuestTypeFused = 11,
    kQuestTypeGuildjoined = 12,
    kQuestTypeInviteLow = 16,
    kQuestTypeInviteMid = 17,
    kQuestTypeShop = 20,
    kQuestTypeWeaponGrowth = 21,
    kQuestTypeArmorGrowth = 22,
    kQuestTypeNecklaceGrowth = 23,
    kQuestTypeFuseGemLevel = 24,
    kQuestTypeFuseGemID = 25,
    kQuestTypeRegist = 26,
    kQuestTypeVIP = 27,
    kQuestTypeCardGet = 28,
    kQuestTypeFairyLandFloor = 29,
    kQuestTypeMountTrain = 30,
    kQuestTypeEquipStar3Get = 31,
    kQuestTypeEquipStar4Get = 32,
    kQuestTypeEquipStar5Get = 33,

} QuestType_t;

typedef enum {
    kBroadcastTypePlainText = 1,
    kBroadcastTypeNewUser = 2,
    kBroadcastTypeCardFromItem = 3,
    kBroadcastTypePropFromItem = 4,
    kBroadcastTypeCardFromCampaign = 5,
    kBroadcastTypeEvolve = 6,
    kBroadcastTypeEnhance = 7,
    kBroadcastTypeGemFuse = 8,
    
    kBroadcastTypeProtectTinaThreeDay = 9,
    kBroadcastTypeFuseFiveStarSpirit = 10,
    kBroadcastTypeSkillWorldBossCoin = 11,
    kBroadcastTypeLevyCoin = 12,
    kBroadcastTypeSeverFast = 13,
    kBroadcastTypeProtectTinaPropLevel = 14,
    kBroadcastTypeProtectTinaCardMaxLevel = 15,
    kBroadcastTypeLearnCaptainFluence = 16,
    kBroadcastTypeDuelWon = 17,
    kBroadcastTypeLegendCardSummoned = 18,
    kBroadcastTypeLegendSpiritSummoned = 19,
    kBroadcastTypeLegendElectChallenger = 20,
    kBroadcastTypeLegendPassOne = 21,
    kBroadcastTypeLegendPassTwo = 22,
    kBroadcastTypeLegendPassAll = 23,
    kBroadcastTypeLegendCardFromCamp = 24,
    kBroadcastTypeGemBatchFuseOne = 25,
    kBroadcastTypeGemBatchFuseMore = 26,
    kBroadcastTypeMountTrain = 27,
    kBroadcastTypeMountObtain = 28,
    kBroadcastTypeMountObtainActivity = 29,
    kBroadcastTypeCardObtain = 30,
    kBroadcastTypeCardCompose = 31,
    kBroadcastTypeCardObtainFromStore = 32,
    kBroadcastTypeBuyHalfPriceItem = 33,
    kBroadcastTypePVPRoundWon = 34,
    kBroadcastTypePVPChampion = 35,
} BroadcastType_t;

typedef enum {
    kMailTypeSystemPlainText = 1,
    
    kMailTypeArenaWon = 10,
    kMailTypeArenaLost = 11,
    kMailTypeArenaLostAndDrop = 12,
    
    kMailTypeGuildPlainText = 20,
    kMailTypeGuildInvitation = 21,
    kMailTypeGuildFired = 22,
    kMailTypeDiscoverBossReward = 23,
    kMailTypeBossRank = 24,
    kMailTypeBetweenFriendMessage = 30,
    KMailTypeLegendElectChallenger = 40,
    KMailTypeLegendCheer = 41,
    KMailTypeLegendChallenge = 42,
    KMailTypeLegendPassRound = 43,
    KMailTypeLegendPassZero = 44,
    KMailTypeLegendPassRoundForCheer = 45,
    KMailTypeLegendPassZeroForCheer = 46,
    KMailTypeLegendNotFight = 47,
    KMailTypeLegendNotFightForCheer = 48,
    kMailTypeGuildImpeachmentNotify1 = 49,
    kMailTypeGuildImpeachmentNotify2 = 50,
    kMailTypeGuildImpeachmentDone = 51,
    kMailTypeCompetitionCampaignReward = 52,
    kMailTypeCompetitionBadgeReward = 53,
    kMailTypeCompetitionLevelupReward = 54,
    kMailTypePVPKnockOutMatch = 55, //参与跨服争霸淘汰赛邮件
    kMailTypePVPTop64Match = 56,    //跨服争霸64强邮件
    kMailTypePVPPresentFlower = 57, //跨服争霸给冠军献花奖励
    kMailTypeVersionUpgrade1dot3Normal = 58,  //升级版本1.3 补偿邮件 普通补偿邮件
    kMailTypeVersionUpgrade1dot3Deposit14 = 59,  //升级版本1.3 补偿邮件 特殊补偿邮件 对 完成过累计充值14的用户
    kMailTypeVersionUpgrade1dot3Deposit15 = 60,  //升级版本1.3 补偿邮件 特殊补偿邮件 对 完成过累计充值15的用户
    kMailTypeVersionUpgrade1dot3Deposit16 = 61,  //升级版本1.3 补偿邮件 特殊补偿邮件 对 完成过累计充值16的用户
    kMailTypeCompetitionDepositReward = 62,
} MailType_t;

typedef enum {
    kProfessionTypeAttack = (1 << 0),
    kProfessionTypeDefend = (1 << 1),
    kProfessionTypeTreat = (1 << 2),
    kProfessionTypeBalance = (1 << 3),
    
    kProfessionTypeUniversal = (kProfessionTypeAttack | kProfessionTypeDefend | kProfessionTypeTreat | kProfessionTypeBalance)
} ProfessionType_t;

typedef enum {
    kNewbieFlagFirstFight = (1 << 0),
    kNewbieFlagEnhance = (1 << 1),
    kNewbieFlagSecondFight = (1 << 2),
    //kNewbieFlagEvolve = (1 << 3),
    kNewbieFlagShop = (1 << 4),
    kNewbieFlagBand = (1 << 5),
    kNewbieFlagEquip = (1 << 6),
    kNewbieFlagArena = (1 << 7),
    //kNewbieFlagGuild = (1 << 8),
    //kNewbieFlagEliteCamp = (1 << 9),
    //kNewbieFlagGem = (1 << 10),
    //kNewbieFlagCampElite = (1 << 11),
    //kNewbieFlagCampActivity = (1 << 12),
    kNewbieFlagSevenDayLogin = (1 << 13),
    kNewbieFlagSpiritOpen = (1 << 14),
    kNewbieFlagThirdFight = (1 << 15),
    kNewbieFlagDuelBegin = (1 << 16),
    kNewbieFlagDuelFirstFight = (1 << 17),
    kNewbieFlagActivityMysteriousForest = (1 << 18), // 活动战役:神秘的丛林
//    kNewbieFlagFreeForFirstCampaignElite_1_1 = (1 << 19), // 第一次打精英1-1，不消耗体力
    kNewbieFlagWonFirstDuel = (1 << 20), // 第一次5牌对决胜利
} NewbieFlag_t;


// (0删除,1发送,2已读 兼容老用户)
typedef enum {
    kMailStatusDelete = 0, // 彻底删除
    kMailStatusSended = (1 << 0), // 已发送（接收）
    kMailStatusRead = (1 << 1), // 已读
    kMailStatusDeleteSended = (1 << 2), // 删除 自己发送的邮件
    kMailStatusDeleteReceived= (1 << 3), // 删除 接收到的邮件
} MailStatus_t;


typedef enum
{
    kMusicMain = 0,
    kMusicBattle = 1,
    kMusicCampaign = 2,
    kMusic5Card = 3,
    kMusicFairyland = 4,
    kMusicLegendGod = 5,
    kMusicLegendCall = 6,
    kMusicOther = 7,
}AudioMusic_t;

typedef enum
{
    kUnprotect = 0,
    kProtect = 1,

}Protect_t;

typedef enum
{
    kGetReward = 0,
    kUnGetReward = 1,
}GetGift_t;

typedef enum
{
    kEffectNoPlay = -1,
    kEffectButtonDown,
    kEffectBattleWin,
    kEffectBattleDropDownWin,
    kEffectBattleLose,
    kEffectBattleNinjaForward,
    kEffectBattleMoveNormal,
    kEffectBattleCardDie,
    kEffectBattleResurrection,
    kEffectEvolveToTarget,
    kEffectEvolveMaterialDisappear,
    kEffectGemFuseStart,
    kEffectGemFuseFlow,
    kEffectGemFuseEnd,
    kEffectGemDrill,
    kEffectGemMount,
    kEffectBuildingUnlock,
    kEffectCountDown,
    kEffectShuffle1,
    kEffectShuffle2,
    kEffectTurnover,
    kEffectReward,
    kEffectBanner,
    kEffectOther,
    kEffectGemUnmount,
    
    kEffectLegendCallBullet,
    kEffectLegendCallCenter,
    
    kEffectSpiritPsychicSurgery,
    kEffectSpiritEnergyFly,
    kEffectSpiritEnergyGot,
    
    kEffectActivityFirstOpen,
    kEffectEliteFirstOpen,
    kEffectCampaignNewStart,
    kEffectCampaignNewEnd,
    kEffectCardDetailPage,
    kEffectAlertTips,
    kEffectCardDroopDrag,
    kEffectFragmentFuse,
    kEffectCardBreakDownExploding,
    kEffectCardBreakDownFly,
    kEffectCardBreakDownCollect,
    kEffectReceiveAward,

    kEffectProtectTina1,
    kEffectProtectTina2,
    kEffectProtectTina3,
    kEffectProtectTina4,
    kEffectProtectTina5,
    kEffectAngelBless1,
    kEffectAngelBless2,
    
    kEffectGuildCreate,
    kEffectGuildDonate,
    kEffectFuseSpirit,
    kEffectGuildUpgradeCash,
    kEffectGuildUpgradeTech,
    kEffectFragmentExplore,
    kEffectFragmentObtain,
    kEffectDailyLoginFlip,
    kEffectGuildUpgradeFail,
    kEffectUnlock,
    
    kEffectShowBattleCry,
    kEffectRefreshBattleCry,
    kEffectAbility,
    kEffectProtectTinaActionStop,
    
    kEffectLevyCoin,
    kEffectLevyFireworks,
    
    kEffectPartner,
    kEffectObtainNewCard,
    kEffectRefreshInfluence,
    kEffectStudyInfluence,
    
    kEffectEnhanceMaterialRotating,
    kEffectEnhanceMaterialBullet,
    kEffectEnhanceTarget,
    kEffectEnhanceQuick,
    
    kEffectLuckyBoxOpen,
    kEffectLuckyBoxCardFly,
    kEffectLuckyBoxSoulCard,
    
    kEffectEnhanceEquip,
    kEffectFuseEquipMaterial,
    
}AudioEffect_t;

typedef enum
{
    KRankS = 1,
    KRankA = 11,
    KRankB = 12,
    KRankC = 13,
    KRankD = 14,
    KRankN = 15
}RankType_t;

typedef enum
{
    EnableClick = 1,
    DisableClick = 2
}ClickType_t;

typedef enum
{
    KCellNormal = 1,
    KCellActivity,
    KCellElite,
    KCellGray,
    kCellRed,
}StageCellBgType_t;

typedef enum
{
    KOrange = 1,
    KDark = 11,
}RankBgType_t;

typedef enum
{
    kRegist = 0,
    kFaceBook = 1,
    kQQ = 2,
    kWeiBo = 3,
    kLogin = 4,
    kPP = 5,
}ThirdPartyName_t;

// for event ui list
typedef enum
{
    kEvtIdx_DailyActive = 0,//每日活跃
    kEvtIdx_DailyLogin,//每日翻牌
    kEvtIdx_Ramen,//一乐拉面
    kEvtIdx_ProtectDaimyo,//保护大名
    kEvtIdx_CardsDuel,//五牌对决
    kEvtIdx_Quest,//任务
    kEvtIdx_MoneyTree,//摇钱树
    
    kEvtIdx_COUNT // must at end
}EventIdx_t;

// for activity ui list
typedef enum
{
    kActIdx_ActDepositFirst = 0,//活动期间首充
    kActIdx_ActDepositTotal,//活动期间累计充值
    kActIdx_ActDepositDaily,//活动期间连续每日充值
    kActIdx_ActConsumeTotal,//活动期间累计消费
    kActIdx_ActConsumeDaily,//活动期间每日消费
    kActIdx_ActBuyItem,//活动期间购买指定物品
    kActIdx_CampaignRank,//历练进度排名
    kActIdx_BadgeRank,//勋章数量排名
    kActIdx_PlayerLevelRank,//玩家等级排名
    kActIdx_MysteriousBox,//神秘宝箱（从"事件"中移过来的）DiamondBox
    kActIdx_LevelRush,//冲级活动
    kActIdx_ItemExchange,//物品兑换活动
    kActIdx_GrowthFund,//成长基金
    kActIdx_MonthCard,//月卡（从"事件"中移过来的）
    kActIdx_DepositTotal,//累计充值（从"事件"中移过来的）
    kActIdx_InvitationFriend,//好友邀请（从"事件"中移过来的）
    kActIdx_InvitationCode,//输邀请码（从"事件"中移过来的）
    kActIdx_ActDailyAccumulativeDeposit,//活动期间每日累计充值活动
    kActIdx_ActFirstBuySpecifiedPackage,//活动期间首次购买指定套餐活动
    kActIdx_ActDailyAccumulativeSpending,//活动期间每日累计消费活动
    kActIdx_Welfare,//奖励发放
    kActIdx_DepositRank,   //玩家充值排名
    kActIdx_ShareToFacebook,    //分享到facebook
    
    kActIdx_COUNT // must at end
}ActivityIdx_t;

//operational activity
typedef enum {
    kActTypeAll = 0,    //所有活动
    kActTypeFirstRecharge = 1,  //活动期间首充
    kActTypeTotalDeposit = 2,   //活动期间累计充值
    kActTypeDailyDeposit = 3,    //活动期间连续每日充值
    kActTypeContinuousDeposit = 4,
    kActTypeDailyAccumulativeDeposit = 5,                      //活动期间每日累计充值活动
    kActTypeFirstBuySpecifiedPackage = 6,                      //活动期间首次购买指定套餐活动
    kActTypeTotalSpending = 11,     //活动期间累计消费
    kActTypeDailySpending = 12,     //活动期间每日消费
    kActTypeBuyItem = 13,           //活动期间购买指定物品
    kActTypeDailyAccumulativeSpending = 14,                    //活动期间每日累计消费活动
    kActTypeOpenAllCampActivities = 21, //活动副本全开
    kActTypeDoubleCampActivities = 22,  //活动副本次数翻倍
    kActTypeOpenSpecifiedInstanceZones = 23,                                //活动指定副本开放
    kActTypeDoubleCampElites = 24,  //精英副本次数翻倍
    kActTypeDoubleBossdrop = 25,		//指定副   本boss双倍掉落
    kActTypeDiamondBox = 31,        //神秘宝箱
    kActTypeEnergyPriceChange = 41,    //体力购买价格变化
} ActivityType_t;

typedef enum
{
    kPVPStatusInvalid = 0,              //无效状态
    kPVPStatusCanNotParticipate = 1,    //不能参与状态
    kPVPStatusPrepare = 2,              //准备状态
    kPVPStatusSignUp = 3,               //报名状态
    kPVPStatusDoNotParticipate = 4,     //未参与状态
    kPVPStatusShowMatchWon = 5,         //比赛结束展示胜利状态
    kPVPStatusShowMatchLose = 6,        //比赛结束展示失败状态
    kPVPStatusWaitMatchBegin = 7,       //比赛等待开战状态
    kPVPStatusShowCampion = 8,          //冠军展示状态
}PVPStatus_t;

typedef enum
{
    kOpenDefault = 0,
    kOpenGuild = 1,
    kOpenGem = 2,
    kOpenArena = 3,
    kClickAltar = 4,
}ConversationOpen_t;

typedef enum
{
    kModEnhance = 0,
    kModEvolve = 1,
    kModFuseView = 2,
    kModInfluence = 3,
}MainSceneGrowth_t;

typedef enum
{
    kTypeTwo,
    kTypeFour,
    kTypeSix,
} FragmetnType_t;


enum class LabelType
{
    NORMAL,
    SPECIAL,
    DESC,
    TITLE,
    BUTTON,
    EDIT,
    BROADCAST,
};


typedef enum
{
    kCardLvBG_Gray = 0,
    kCardLvBG_Brown = 1,
    kCardLvBG_Blue = 2,
}CardLvBG_t;


enum class BlessStatus
{
    OK, // 可接受祝福（吃面）
    COUNTING_DOWN, // 正在倒计时
    FULL, // 领取的祝福已达上限
    DATE_CHANGE, // 日期改变
};


enum class TaskState
{
    DOING,
    COMPLETED,
    REWARD_RECEIVED,
};


typedef enum
{
    kListSclSts_NotFull = 0,
    kListSclSts_FullAtCenter,
    kListSclSts_FullAtStart,
    kListSclSts_FullAtEnd,
}ListScrollStatus_t;

typedef enum
{
    kGrowthFund = 1,
    kGrowthServer = 2
}GrowthFundType_t;


typedef enum
{
    kForwardType_Switch = 0,
    kForwardType_Push = 1,
    kForwardType_Pop = 2,
}ForwardType_t;


typedef enum
{
    kForwardModule_Campaign = 0,//战役
    kForwardModule_Arena = 1,//竞技场
    kForwardModule_Events = 2,//事件
    kForwardModule_Activity = 3,//活动
    kForwardModule_WorldBoss = 4,//世界BOSS
    kForwardModule_Growth = 5,//成长（升级、进化、队长技能）
    kForwardModule_Friend = 6,//好友
    kForwardModule_Spirit = 7,//英灵
    kForwardModule_Mount = 8,//坐骑
    kForwardModule_BuyEnergy = 9,//购买体力
    kForwardModule_Deposit = 10,//充值
    kForwardModule_BuyVIP = 11,//VIP购买
}ForwardModule_t;

//装备副属性
typedef enum
{
    kEquipSubAttr_Null = 0,//无
    kEquipSubAttr_PAtk = 1,//破甲
    kEquipSubAttr_PDef = 2,//护甲
    kEquipSubAttr_Hit = 3,//命中
    kEquipSubAttr_Dodge = 4,//闪避
    kEquipSubAttr_Critical = 5,//暴击
    kEquipSubAttr_Toughness = 6,//韧性
    kEquipSubAttr_Heal = 7,//被治疗

}EquipSubAttrType_t;

typedef enum
{
    kModEquipEnhance = 0,
    kModEquipEvolve = 1,
    kModEquipGem = 2,
}MainSceneEquipGrowth_t;

// brave arena
// see detail on server
typedef enum
{
    kBraveArenaSelfStatus_Invalid = 0, //无效数据
    kBraveArenaSelfStatus_Ready = 1, //数据正常
}BraveArenaSelfStatus_t;

typedef enum
{
    kBraveArenaCardStatus_Invalid = 0, //无效状态
    kBraveArenaCardStatus_Ready = 1, //可战斗
    kBraveArenaCardStatus_Death = 2, //死亡
}BraveArenaCardStatus_t;

typedef enum
{
    kBraveArenaOpponentStatus_Invalid = 0, //无效状态
    kBraveArenaOpponentStatus_Lock = 1, //锁定
    kBraveArenaOpponentStatus_Ready = 2, //可攻击
    kBraveArenaOpponentStatus_Death = 3, //击杀
}BraveArenaOpponentStatus_t;

typedef enum
{
    kImgStateNormal = 0,
    kImgStateDisable = 1,
    kImgStateHighLight = 2,
    kImgStateEliteDisable = 3,  //精英关卡无次数状态
    kImgStateEliteHightLight = 4,   //精英关卡无次数选中状态
}kImgState_t;

//for spirit
#define TWO_BOX_FUSE 51
#define FOUR_BOX_FUSE 52
#define SIX_BOX_FUSE 53

//activity
//#define ACTIVITY_GEM_ID 204  // delete in naruto

// 卡牌详情闪烁特效星级
#define STAR_EFF_MIN_LEVEL     5

//prop
#define GEM_FAMILY_MAGICSOULSTONE 64
#define GEM_FAMILY_MAGICSPARSTONE 65
#define PROP_FAMILY_FRAGMENT_MAIN 33
#define PROP_FAMILY_FRAGMENT_EQUIP 34
#define PROP_FAMILY_NOT_SHOW 41 //by zhangcheng : TODO1.3 这个是干啥用的
#define PROP_FAMILY_REEL_JUNIOR 51
#define PROP_FAMILY_REEL_MIDDLE 52
#define PROP_FAMILY_REEL_HIGH 53
#define PROP_FAMILY_EQUIP_MATERIAL 41
#define PROP_FAMILY_BOX_NORMAL 71
#define PROP_FAMILY_BOX_ADVANCED 72

//宝石
#define GEM_FAMILY_MIN 21
#define GEM_FAMILY_MAX 28
#define GEM_FAMILY_DAMAGE 21
#define GEM_FAMILY_ATK 21 // alias
#define GEM_FAMILY_PDEF 22
#define GEM_FAMILY_MDEF 23
#define GEM_FAMILY_HP 24
#define GEM_FAMILY_CRITICAL 25
#define GEM_FAMILY_DODGE 26
#define GEM_FAMILY_TOUGHNESS 27
#define GEM_FAMILY_HIT 28

// for mount
#define MOUNT_STAR_MAXED 5

//
#define  FRIENDPOINT_GOT 5
#define  FRIENDPOINT_GOT_WITH_FRIEND 10
//campaign
#define FIRST_CAMPAIGN 100
#define FIRST_STAGE 1
#define SECOND_STAGE 2
#define STAGE_NORMAL 1
#define STAGE_ELITE  2
#define STAGE_HERO   3
#define CARD_TYPE_INVALID 0
#define CARD_TYPE_HERO 1
#define CARD_TYPE_NPC 2
#define CARD_TYPE_MAIN (CARD_TYPE_HERO | CARD_TYPE_NPC)
#define CARD_TYPE_WEAPON 4
#define CARD_TYPE_ARMOR 8
#define CARD_TYPE_NECKLACE 16
#define CARD_TYPE_EQUIP (CARD_TYPE_WEAPON | CARD_TYPE_ARMOR | CARD_TYPE_NECKLACE )
#define CARD_TYPE_PROP 32
#define CARD_TYPE_MOUNT 64
#define PASS_ELITE_CAMPAIGN_DROP_EQUIP_CARD 30574
#define FRAME_CARD_WHITE_SMALL "empty_white_s.png"
#define FRAME_CARD_GREEN_SMALL "empty_green_s.png"
#define FRAME_CARD_BLUE_SMALL "empty_blue_s.png"
#define FRAME_CARD_PURPLE_SMALL "empty_purple_s.png"
#define FRAME_CARD_ORANGE_SMALL "empty_orange_s.png"

#define FRAME_CARD_BG_EMPTY_SMALL "card_bg_blue.png"
#define FRAME_CARD_BG_WHITE_SMALL "card_bg_white.png"
#define FRAME_CARD_BG_GREEN_SMALL "card_bg_green.png"
#define FRAME_CARD_BG_BLUE_SMALL "card_bg_blue.png"
#define FRAME_CARD_BG_PURPLE_SMALL "card_bg_purple.png"
#define FRAME_CARD_BG_ORANGE_SMALL "card_bg_orange.png"
#define FRAME_CARD_BG_RED_SMALL "card_bg_red.png"

#define FRAME_CARD_WHITE_MEDIUM "frame_white_m.png"
#define FRAME_CARD_GREEN_MEDIUM "frame_green_m.png"
#define FRAME_CARD_BLUE_MEDIUM "frame_blue_m.png"
#define FRAME_CARD_PURPLE_MEDIUM "frame_purple_m.png"
#define FRAME_CARD_ORANGE_MEDIUM "frame_orange_m.png"
#define FRAME_CARD_EMPTY_MEDIUM "card_empty_m.png"

#define FRAME_CARD_WHITE_LARGE "frame_white_l.png"
#define FRAME_CARD_GREEN_LARGE "frame_green_l.png"
#define FRAME_CARD_BLUE_LARGE "frame_blue_l.png"
#define FRAME_CARD_PURPLE_LARGE "frame_purple_l.png"
#define FRAME_CARD_ORANGE_LARGE "frame_orange_l.png"

#define BG_CARD_WHITE_LARGE "bg_white_l.png"
#define BG_CARD_GREEN_LARGE "bg_green_l.png"
#define BG_CARD_BLUE_LARGE "bg_blue_l.png"
#define BG_CARD_PURPLE_LARGE "bg_purple_l.png"
#define BG_CARD_ORANGE_LARGE "bg_orange_l.png"

#define BG_CARD_WHITE_MEDIUM "card_bg_white_m.png"
#define BG_CARD_GREEN_MEDIUM "card_bg_green_m.png"
#define BG_CARD_BLUE_MEDIUM "card_bg_blue_m.png"
#define BG_CARD_PURPLE_MEDIUM "card_bg_purple_m.png"
#define BG_CARD_ORANGE_MEDIUM "card_bg_orange_m.png"

// equip border
#define FRAME_EQUIP_WHITE_BORDER "icon_status_white.png"
#define FRAME_EQUIP_GREEN_BORDER "icon_status_green.png"
#define FRAME_EQUIP_BLUE_BORDER "icon_status_blue.png"
#define FRAME_EQUIP_PURPLE_BORDER "icon_status_purple.png"
#define FRAME_EQUIP_ORANGE_BORDER "icon_status_orange.png"

#define COLOR_WHITE cocos2d::ccc3(255, 255, 255)
#define COLOR_GREEN cocos2d::ccc3(0, 255, 0)
#define COLOR_BLUE cocos2d::ccc3(70, 200, 235)
#define COLOR_PURPLE cocos2d::ccc3(220, 35, 220)
#define COLOR_ORANGE cocos2d::ccc3(220, 125, 35)
#define COLOR_RED cocos2d::ccc3(255, 19, 19)
#define COLOR_YELLOW cocos2d::ccc3(220, 220, 35)
#define COLOR_GRAY cocos2d::ccc3(190, 190, 190)
#define COLOR_BROWN cocos2d::ccc3(88, 36, 0)

//ui color
#define COLOR_UI_1 cocos2d::ccc3(255, 248, 210)
#define COLOR_UI_2 cocos2d::ccc3(255, 121, 0)
#define COLOR_UI_3 cocos2d::ccc3(134, 255, 148)
#define COLOR_UI_4 cocos2d::ccc3(255, 19, 19)
#define COLOR_UI_5 cocos2d::ccc3(169, 130, 85)
#define COLOR_UI_6 cocos2d::ccc3(0, 0, 0)   //for stroke
#define COLOR_UI_7 cocos2d::ccc3(0, 0, 0)
#define COLOR_UI_8 cocos2d::ccc3(73, 136, 255)
#define COLOR_UI_9 cocos2d::ccc3(252, 223, 59)
#define COLOR_UI_10 cocos2d::ccc3(217, 205, 170)
#define COLOR_UI_11 cocos2d::ccc3(85, 182, 203)
#define COLOR_UI_12 cocos2d::ccc3(255, 253, 219)
#define COLOR_UI_13 cocos2d::ccc3(160, 131, 89)
#define COLOR_UI_14 cocos2d::ccc3(225, 187, 112)
#define COLOR_UI_15 cocos2d::ccc3(213, 213, 213)
#define COLOR_UI_16 cocos2d::ccc3(98, 98, 98)
#define COLOR_UI_17 cocos2d::ccc3(89, 103, 117)
#define COLOR_UI_18 cocos2d::ccc3(114, 75, 17)
#define COLOR_UI_19 cocos2d::ccc3(255, 255, 255)
#define COLOR_UI_20 cocos2d::ccc3(1, 73, 126)
#define COLOR_UI_21 cocos2d::ccc3(255, 0, 0)
#define COLOR_UI_22 cocos2d::ccc3(0, 174, 255)


// new ui color
#define COLOR_UI_NEW_1 cocos2d::ccc3(255, 255, 255) // white
#define COLOR_UI_NEW_2 cocos2d::ccc3(158, 158, 158) // gray
#define COLOR_UI_NEW_3 cocos2d::ccc3(0, 0, 0)
#define COLOR_UI_NEW_4 cocos2d::ccc3(100, 255, 118)  // green
#define COLOR_UI_NEW_5 cocos2d::ccc3(255, 12, 12)  // red
#define COLOR_UI_NEW_6 cocos2d::ccc3(138, 248, 255) // blue
#define COLOR_UI_NEW_7 cocos2d::ccc3(33, 85, 163)
#define COLOR_UI_NEW_8 cocos2d::ccc3(88, 36, 0)
#define COLOR_UI_NEW_9 cocos2d::ccc3(255, 229, 39)  // yellow
#define COLOR_UI_NEW_10 cocos2d::ccc3(70, 70, 70)
#define COLOR_UI_NEW_11 cocos2d::ccc3(220, 125, 35)
#define COLOR_UI_NEW_12 cocos2d::ccc3(254, 227, 175)
#define COLOR_UI_NEW_13 cocos2d::ccc3(225, 107, 255)  // purple
#define COLOR_UI_NEW_14 cocos2d::ccc3(255, 166, 51)  // orange
#define COLOR_UI_NEW_15 cocos2d::ccc3(255, 233, 194)

#define COLOR_UI_NEW_WHITE      COLOR_UI_NEW_1
#define COLOR_UI_NEW_GRAY       COLOR_UI_NEW_2
#define COLOR_UI_NEW_GREEN      COLOR_UI_NEW_4
#define COLOR_UI_NEW_RED        COLOR_UI_NEW_5
#define COLOR_UI_NEW_BLUE       COLOR_UI_NEW_6
#define COLOR_UI_NEW_YELLOW     COLOR_UI_NEW_9
#define COLOR_UI_NEW_PURPLE     COLOR_UI_NEW_13
#define COLOR_UI_NEW_ORANGE     COLOR_UI_NEW_14

#define COLOR_UI_NEW_LEGEND COLOR_UI_NEW_YELLOW
#define COLOR_UI_NEW_STROKE_LEGEND COLOR_UI_NEW_3
#define COLOR_UI_NEW_STROKE_NORMAL COLOR_UI_NEW_3
#define COLOR_UI_NEW_STROKE_SPECIAL COLOR_UI_NEW_7
#define NEW_STROKE_SIZE 2

#define COLOR_UI_BROADCAST_ITEM cocos2d::ccc3(35, 221, 233)

#define COLOR_UI_LEVEL_LIMIT cocos2d::ccc3(192,56,48)

#define COLOR_UI_STROKE cocos2d::ccc3(255, 121, 0)


#define GUILD_CASH_COLOR ccc3(255,229,39)
#define GUILD_COIN_COLOR ccc3(255,255,255)

#define GEM_RANGE_MIN 21
#define GEM_RANGE_MAX 28
#define EQUIP_MAX_LEVEL 30
#define EQUIP_HAVE_SLOT_MIN_STAR  3 // 装备可以打第一个宝石孔的最低等级要求

#define PROPS_NUM_A_ROW 5
#define FRAME_RATE_DEFAULT 24
//(24 * 24 * CCDirector::sharedDirector()->getSecondsPerFrame())
#define BADGE_NUM_A_ROW 5

#define PATH_IMAGE "images/"

#define CG_LOGO "logo.mp4"
#define CG_START "startCG.mp4"

#define INHERIT_LIST_VISIBLE_FLOOR 1


#define GUILD_TITLE_MOVE_SPPED 0.5


#define FRAGMENT_ID_1 51
#define FRAGMENT_ID_2 52
#define FRAGMENT_ID_3 53

#define LUCKY_SHOW_0 0
#define LUCKY_SHOW_1 2
#define LUCKY_SHOW_2 3
#define LUCKY_SHOW_3 4 //魂匣紫卡

#define MAX_NICKNAME_LENGTH 18
#define MAX_GUILD_NAME_LENGTH 20

#define BRAVE_ARENA_MAP_TOTAL_PAGE      3
#define BRAVE_ARENA_MAP_COUNT_PER_PAGE      5

#define DURATION_OF_MOVEMENT (2)
#define DURATION_OF_ACTION_MOVEMENT (1 + 20/30.f)

#endif
