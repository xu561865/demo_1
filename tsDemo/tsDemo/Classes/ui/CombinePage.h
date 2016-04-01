#ifndef __client1__CHecheng__
#define __client1__CHecheng__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LSocketCenter.h"
#include "message.h"
#include "CData.h"
#include "LayerLogin.h"
#include "LodingLayer.h"
#include "hcitemdata.h"

USING_NS_CC;

class Peifang:public CCObject
{
public:
    Peifang(){tempid =0;icon=0;id=0;name="";levelRequire=0;baseQuality=0;buyingRateCoin=0;help = "";hp=0;gj=0;fy=0;shb=0;bj=0;};
    int tempid;             //配方的模板id
    int icon;               //图标
    int id;                 //配方的id
    string name;            //配方的名字
    int levelRequire;       //等级
    int baseQuality;        //品质
    int buyingRateCoin;     //价格
    string help;            //描述
    int hp;                 //加血
    int gj;                 //攻击
    int fy;                 //防御
    int shb;                //闪避
    int bj;                 //暴击
    bool isxz;
    
};

class hcpf:public CCObject
{
public:
    hcpf()
    {
        jiage=0;
        name="";
        pz=0;
        icon=0;
        idenity=0;
        des="";
        isxz=false;
    }
    int jiage;
    string name;
    int pz;
    int icon;
    int idenity;
    string des;
    bool isxz;
};

class Hcdata:public CCObject
{
public:
    int coinrequired;		//合成所需的钱
    int itemA;              //物品1的ID
    int itemAcnt;			//物品1当前的数量
    int itemAGoal;          //物品1总共需要的数量
    int itemB;				//物品2的ID
    int itemBcnt;           //物品2当前的数量
    int itemBGoal;          //物品2总共需要的数量
    int itemBound;          //合成的物品ID
};

class CombinePage
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
{
    CCSprite* pPlus[4];
    CCLabelTTF* gold;
    CCMenuItemImage* pBtn[4];
    
    CCDictionary * pf_dic;
    bool flag;
    int state;
    int num;        //细胞个数
    int leng;
    CCSize size;
    bool isxz;
    int tpy;
    CCDictionary* P_dictionary;
    extension::CCTableView* tableView;
    bool isa;
    int m_index;
    bool isgx;
    CCMenu * m_CK;
    extension::CCTableViewCell* m_cell;
    CCSprite * bac;
    hcpf * m_pf;
    
    CCSprite* sp[4];
    
    hcitemdata * tmpitemdata;
    CCMenuItemImage * m_hc;
    
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);
    
    CREATE_FUNC(CombinePage);
    static CCScene* scene();
    virtual bool init();
    virtual void onExit();
    ~CombinePage();
    
    void menuItemCallbackBuy(CCObject *pSender);
    void removeFadeIn();
    
    void fasonghecheng();
    void dongzuo();
    //进入界面则发送发送请求
    void fasong();
    void getdata();
    void jsondata(char* data);
    //void jsondata(ifstream ifs);
    void jieximoban(Peifang * pf);
    
    void openbag(CCObject* pSender);
    void goBack();//返回
    void recData();
    void sethcdata(hcitemdata* item);
    
    void addtupian(Hcdata* hc);
    void clearData();
    
    //发送合成物品信息
    void  fasongHCID(hcpf* pf);
    //得到合成所需要的数据
    void  getHCdata();
    //解析得到的合成数据
    void  jasonHCdata(char * data);
    void  addchenggong();
    void  bfchenggong();
    void  addshibai();
    void  bfshibai();
    //发送合成请求
    void  fsqq(int tempid);
    //得到得到合成成功与否的结果
    void  getjieguo();
    //解析结果
    void  jsonjieguo(char* data);
    void getitempic(Hcdata* hc,int &tmpId);
    void getjsonpic(Hcdata* hc,int &tmpId,int  &temId2);
    void getjsonname(Hcdata* hc,std::string &str,std::string &str1);
    bool istouch;
    
    Loading * load;
    SocketClient * sclient;
    
};

class CombinePageLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CombinePageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CombinePage);
};

#endif /* defined(__client1__CHecheng__) */
