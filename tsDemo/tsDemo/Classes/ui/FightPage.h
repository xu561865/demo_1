#ifndef __Client__FireWork__
#define __Client__FireWork__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "json.h"
#include "skillList.h"
#include "LodingLayer.h"

USING_NS_CC;
using namespace std;

class fireinfo:public CCObject
{
public:
    fireinfo()
    {
        personId=0;
        *buff=0;
    }
    int personId;
    int buff[5];
};

class skill:public CCObject
{
public:
    int releaseEffect;
    int bearEffect;
    int throwEffectId;
    
};

class FightPage
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
    {
        return NULL;
    }
    
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
    {
        return NULL;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_buf_1", CCSprite*, this->buff1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_buf_2", CCSprite*, this->buff2);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_buf_3", CCSprite*, this->buff3);
        
        return false;
    }
    
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue)
    {
        return true;
    }
    
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);

    CREATE_FUNC(FightPage);
    ~FightPage();
    virtual bool init();
    virtual void onExit();
    static CCScene * scene();
    
    CCSprite * enemyIcon;
    CCDictionary * jn_dic;
    CCLabelTTF * enemyName1;
    CCLabelTTF * playerName1;
    CCSprite * playerIcon;
    CCLabelTTF * label1;
    CCLabelTTF * label2;
    CCLabelTTF * label3;
    CCDictionary * ene_dic;
    CCDictionary * player_dic;
    CCSprite * chatbg;
    CCLabelTTF * print;
    CCSprite *eneB1;
    CCSprite * normalAttack;
    CCLabelAtlas * reduB;
    CCLabelAtlas * reduB1;
    CCSprite * tz;
    CCSprite * buff1;
    CCSprite * buff2;
    CCSprite * buff3;
    CCSprite *explosion;
    CCSprite * jiantou;
    CCSprite *tx;//受伤特效
    CCSprite * texiao;//技能特效
    CCSprite * mz;//命中特效
    CCSprite *label;
    CCSprite * enemy;
    CCSprite * player;
    extension::CCScale9Sprite * m_pNextBG;
    CCLabelTTF * chatBounder;
    CCSprite * updown;
    CCSize size;
    
    int e;
    int max;
    int enemytag;
    const char *namee;
    
    Json::Value _json;
    Json::Value stepData;
    int roundss;
    int _chaCurrentPower;
    int _eneCurrentPower;
    int skillnumbers1[20];
    int direction;
    int audionum;
    Loading *load;
    double percent1;
    
    CCDictionary * chat_dic;
    skillList * sk;
    skill * skk;
    int ea;
    int chaIdd;
    int chaBattleId;
    bool chengzhan;
    bool first;
    int rounds;//回合数
    int pos;//敌我双方武将所在九宫格位置
    int kaishi;//标记
    int battleresult;//战役结果
    int changeHp;//改变的血量
    int currHp;//当前血量
    int eneBID;//敌人ID
    int totalHp;//血量上限
    int _actionId;//动作ID
    int _star;//星级
    int _skillId;//技能ID;
    int _eneBattId[9];//多
    int _eneChaid[9]; //个
    int _eneChanHp[9];//敌
    int _eneCurrHp[9];//人
    int _eneTotHp[9]; //数
    int _eneActId[9]; //组
    int _chaBattleId;
    int _chaId;
    int _length;
    int j;
    int t;
    
    void initFireData();
    void cardData(char * json);//解析json
    void attackData(char *json);
    void debuff(int eneBid, int changeHp);
    void playStarting(int eneBattleId, int chaBattleID);//播放开始动画
    void doAttack();
    void playSkill();//播放招数动画
    void playSkillName();
    void whethertwice();
    void throwtwice();
    void attacked();//被攻击动画
    void changeBlood();//改变血量
    void MeasureEneDead(CCObject * eneTag);//判断敌人是否死亡
    void checkOut();
    
    void moreActionAttack(int * eneBattId, int * eneChaid,int * eneChanHp,int * eneCurrHp,int  * eneTotHp,int  * eneActId,int chaBattleId,int chaId,int length,int skillId);//多重攻击
    void moreattactnext();
    void moreAttackName();
    void moreAttackTitle();//攻击动画
    void moreAttacked();
    void MoreAttack();//循环多重攻击
    void removeAction();
    void removeit();
    void zhenping(CCNode * sender);
    skill * returnskills(int skillId);
    void SimpleAttack();//循环单一攻击
    void buffbegin(int bufftype);
};

class FightPageLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(FightPageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(FightPage);
};

#endif /* defined(__Client__FireWork__) */
