#include "CombinePage.h"
#include "LayerLogin.h"
#include "hcbag.h"
#include "SocketManager.h"
#include "CustomPop.h"
#include "headPage.h"
#include "BottomPage.h"

SEL_MenuHandler CombinePage::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_1", CombinePage::openbag);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_2", CombinePage::openbag);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_3", CombinePage::openbag);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_4", CombinePage::openbag);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_combine", CombinePage::fasong);
    
    return NULL;
}

extension::SEL_CCControlHandler CombinePage::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool CombinePage::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_plus_1", CCSprite*, pPlus[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_plus_2", CCSprite*, pPlus[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_plus_3", CCSprite*, pPlus[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pic_plus_4", CCSprite*, pPlus[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_1", CCMenuItemImage*, pBtn[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_2", CCMenuItemImage*, pBtn[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_3", CCMenuItemImage*, pBtn[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_4", CCMenuItemImage*, pBtn[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_combine", CCMenuItemImage*, m_hc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ttf_money", CCLabelTTF*, gold);
    
    return false;
}

bool CombinePage::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    return true;
}

void CombinePage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    //加载角色信息
    CCNode* pHeadPage = HeadPage::getNode(this);
    pHeadPage->setPosition(ccp(0, size.height-100));
    this->addChild(pHeadPage,1);
    
    //加载底部按钮
    CCNode* pBottomPage = BottomPage::getNode(this);
    pBottomPage->setPosition(ccp(0, 25));
    this->addChild(pBottomPage, 2);
    
    for(int i = 0; i < 4; i++)
    {
        CCActionInterval* fadein = CCFadeIn::create(0.5);
        CCActionInterval* fadeout = CCFadeOut::create(0.5);
        CCActionInterval* action = CCSequence::create(fadein, fadeout, NULL);
        pPlus[i]->runAction(CCRepeatForever::create(action));
    }
    
    m_hc->setVisible(false);
}

CCScene* CombinePage::scene()
{
    CCScene *pScene = CCScene::create();
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("CombinePage", CombinePageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/CombinePage.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pScene;
}

void CombinePage::onExit()
{
    CCLayer::onExit();
}

CombinePage::~CombinePage()
{
    for(int i = 0; i < 4; i++)
    {
        pPlus[i]->stopAllActions();
    }
    
    P_dictionary->removeAllObjects();
    P_dictionary->release();
    pf_dic->removeAllObjects();
    pf_dic->release();
}

bool CombinePage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    pf_dic=CCDictionary::create();
    pf_dic->retain();
    flag=true;
    P_dictionary = CCDictionary::create();
    P_dictionary->retain();
    num = 0;
    state =1;
    leng = 10*140;
    isxz = false;
    tpy = 2116;
    tableView = NULL;
    isa = false;
    m_index =0;
    isgx = false;
    m_cell =NULL;
    m_pf =NULL;
    tmpitemdata =NULL;
    
    size = CCDirector::sharedDirector()->getWinSize();

    return true;
}

void CombinePage::sethcdata(hcitemdata *item)
{
    configGoodsItem* goodsitem = CData::getCData()->getConfigOfGoods(item->tempid);
    
    char itemicon[40]="";
    
    if(goodsitem->icon >=6000 && goodsitem->icon<= 6056)
    {
        sprintf(itemicon, "qs_%d.png",goodsitem->icon);
        
        for(int i = 0; i < 4; i++)
        {
            sp[i] = CCSprite::create(itemicon);
            sp[i]->setScale(0.19);
        }
    }
    else
    {
        sprintf(itemicon, "item_%d.png",goodsitem->icon);
        for(int i = 0; i < 4; i++)
        {
            sp[i] = CCSprite::create(itemicon);
        }
    }
    
    sprintf(itemicon, "item_%d.png",goodsitem->icon);
    
    for(int i = 0; i < 4; i++)
    {
        pBtn[i]->addChild(sp[i]);
    }
    
    tmpitemdata = item;
    
    char g[10] = "";
    sprintf(g, "%d", goodsitem->comprice);
    gold->setString(g);
    
    m_hc->setEnabled(true);
}

void CombinePage::recData()
{
    Message *revMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(2109);
    CData::getCData()->m_dictionary->removeObjectForKey(2109);
    if(revMsg)
    {
        this->unschedule(schedule_selector(CombinePage::recData));
        load->removeFromParent();
        char * data=revMsg->data;
        
        hcbag* bag = hcbag::create();
        bag->hc = this;
        //bag->setPosition(ccp(size.width/2, size.height/2));
        CCDirector::sharedDirector()->getRunningScene()->addChild((CCNode*)bag);
        //bag->setPosition(ccp(0, 0));
        bag->setData(data);
        
        //CData::getCData()->setZGBagInfo(data);
        //this->initJsonData();
    }
}

void CombinePage::clearData()
{
    tmpitemdata = NULL;
    m_hc->setEnabled(false);
    for(int i = 0; i < 4; i++)
    {
        pBtn[i]->removeChild(sp[i]);
    }

    gold->setString("0");
}

void CombinePage::openbag(CCObject* pSender)
{
    clearData();
    
    load=Loading::create();
    load->setPosition(ccp(0, 0));
    this->addChild(load,9999);
   
    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    std::string json_file_zg=writer.write(sendMessage);
    
    SocketManager::getInstance()->sendMessage(json_file_zg.c_str(), 2109);
    this->schedule(schedule_selector(CombinePage::recData),0.2);
}

void CombinePage::goBack()
{
    CData::getCData()->setyinxiao();
    //this->setVisible(true);
    m_CK->removeFromParent();
    this->removeChild(bac);
    //UI->setVisible(true);
}

void CombinePage::dongzuo()
{
    CData::getCData()->setyinxiao();

    //if(isxz)
    {
        //Peifang * pf = (Peifang*)P_dictionary->objectForKey(m_index);
        hcpf * pf=(hcpf *)pf_dic->objectForKey(m_index);
        fsqq(pf->idenity);//tempid
        
        CCSprite* spite1 = (CCSprite*)this->getChildByTag(1001);
        CCSprite* spite2 = (CCSprite*)this->getChildByTag(1002);
        CCSprite* spite3 = (CCSprite*)this->getChildByTag(1003);
        CCPoint poi = spite3->getPosition();
        //淡进
        CCFadeOut *playfadeout=CCFadeOut::create(0.3f);
        //淡出
        //CCFadeIn *playerfadein=CCFadeIn::create(0.5f);
        CCCallFunc * fun2=CCCallFunc::create(this, callfunc_selector(CombinePage::fasonghecheng));
        CCFiniteTimeAction *playerAct=CCSequence::create(playfadeout,fun2,NULL);
        spite1->runAction(playerAct);
        
        CCFadeOut *playfadeout1=CCFadeOut::create(0.3f);
        //CCFadeIn *playerfadein=CCFadeIn::create(0.5f);
        CCCallFunc * fun=CCCallFunc::create(this, callfunc_selector(CombinePage::fasonghecheng));
        CCFiniteTimeAction *playerAct1=CCSequence::create(playfadeout1,fun,NULL);
        spite2->runAction(playerAct1);
        
        CCActionInterval * moveon=CCMoveTo::create(0.4f, poi);
        spite1->runAction(moveon);
        CCActionInterval * moveon1=CCMoveTo::create(0.4f, poi);
        spite2->runAction(moveon1);
    }
    //isxz = false;
}

void CombinePage::fasonghecheng()
{
    this->removeChildByTag(1001);
    this->removeChildByTag(1002);
    this->removeChildByTag(1003);
    this->removeChildByTag(1004);
    this->removeChildByTag(1005);
    this->removeChildByTag(1006);
    this->removeChildByTag(1007);
    this->removeChildByTag(1008);
    this->removeChildByTag(1009);
    this->removeChildByTag(1010);
    this->removeChildByTag(1011);

    switch (state)
    {
        case 1:
            CCLOG("合成武将");
            break;
        case 2:
            CCLOG("合成宝石");
            break;
        case 3:
            CCLOG("合成技能");
            break;
        
        default:
            break;
    }
}

//发送请求
void CombinePage::fasong()
{
    if(tmpitemdata == NULL)
    {
        CustomPop::show("请选择合成材料");
        return;
    }
    
    load=Loading::create();
    load->setPosition(ccp(0, 0));
    this->addChild(load,9999);
    
    int characterID = CData::getCData()->getCharactorId();
    Json::FastWriter  writer;
    Json::Value person;
    
    person["characterId"]=characterID;
    person["tempid"]=tmpitemdata->tempid;
    
    std::string  json_file=writer.write(person);
    SocketManager::getInstance()->sendMessage(json_file.c_str(), tpy);
    this->schedule(schedule_selector(CombinePage::getdata));
}

//获得数据
void CombinePage::getdata()
{
    Message* revMsg = (Message*)CData::getCData()->m_dictionary->objectForKey(tpy);
    CData::getCData()->m_dictionary->removeObjectForKey(tpy);
    
    if(revMsg)
    {
        this->unschedule(schedule_selector(CombinePage::getdata));
        this->removeChild(load);
        char * ditu=revMsg->data;
        
        //解析数据
        if(ditu)
        {
            CCLOG("得到物品的信息:%s",ditu);
            
            jsondata(ditu);
        }
        
        //this->schedule(schedule_selector(CHecheng::getdata));
    }
}

//解析道具
void CombinePage::jsondata(char * data)
{
    Json::Reader read;
    Json::Value value;
    if(read.parse(data, value))
    {
        Json::Value m_data= value["data"];
        
        bool  ischenggong = value["result"].asBool();
        CCLog("ischenggong=%d",ischenggong);
        if(ischenggong == true)
        {
            CustomPop::show("合成成功！");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("refresh_zhuangtai");
            
            Json::Value mapinfos = m_data["itemlist"];
            int len  = mapinfos.size();
            
            CCLog("配方个数为=%d",len);
            if(len !=0)
            {

            }
            num += len;
        }
        else
        {
            std::string out = value["message"].asCString();
            //这里输出错误原因：out
            CustomPop::show(out.c_str());
            //const char* txt = out.c_str();
        }
    }
    clearData();
}

void CombinePage::jieximoban(Peifang * pf)
{
    Json::Reader reader;
    Json::Value root;
    Json::Value items;
    
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("item.json");
    CCString* str = CCString::createWithContentsOfFile(path.c_str());
    
    if(reader.parse(str->getCString(), root))
    {
        if(pf->tempid!=0)
        {
            char inttostr[100]="";
            sprintf(inttostr, "%d",pf->tempid);
            items=root[inttostr];
            pf->name = items["name"].asString();
            pf->icon = items["icon"].asInt();
            pf->id   = items["id"].asInt();
            pf->levelRequire   = items["levelRequire"].asInt();
            pf->baseQuality    = items["baseQuality"].asInt();
            pf->buyingRateCoin = items["buyingRateCoin"].asInt();
            pf->help           = items["description"].asString();
            pf->hp = items["baseHpAdditional"].asInt();
            pf->gj = items["basePhysicalAttack"].asInt();
            pf->fy = items["basePhysicalDefense"].asInt();
            pf->shb = items["baseDodgeAdditional"].asInt();
            pf->bj = items["baseCritAdditional"].asInt();
            pf->isxz = false;
        }
    }
}

void CombinePage::getjsonname(Hcdata* hc,std::string &str,std::string &str1)
{
    Json::Reader reader;
    Json::Value root;
    Json::Value items;
    Json::Value items1;
    
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("item.json");
    CCString* strtt = CCString::createWithContentsOfFile(path.c_str());
    
    if(reader.parse(strtt->getCString(), root))
    {
        char inttostr[100]="";
        sprintf(inttostr, "%d",hc->itemA);
        char inttostr1[100]="";
        sprintf(inttostr1, "%d",hc->itemB);
        items=root[inttostr];
        items1=root[inttostr1];
        str1=items1["name"].asString();
        str = items["name"].asString();
        
        CCLog("%s",str.c_str());
    }
}

void CombinePage::getjsonpic(Hcdata* hc,int &tmpId,int &temId2)
{
    Json::Reader reader;
    Json::Value root;
    Json::Value items;
    Json::Value items1;
    
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("item.json");
    CCString* str = CCString::createWithContentsOfFile(path.c_str());
    
    if(reader.parse(str->getCString(), root))
    {
        char tempid[100]="";
        sprintf(tempid, "%d",hc->itemA);
        char temid1[100]="";
        sprintf(temid1, "%d",hc->itemB);
        items=root[tempid];
        items1=root[temid1];
        tmpId=items["icon"].asInt();
        temId2=items1["icon"].asInt();
    }
}

void CombinePage::getitempic(Hcdata* hc,int &tmpId)
{
    Json::Reader reader;
    Json::Value root;
    Json::Value items;
    Json::Value items1;
    
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("item.json");
    CCString* str = CCString::createWithContentsOfFile(path.c_str());

    if(reader.parse(str->getCString(), root))
    {
        char tempid[100]="";
        sprintf(tempid, "%d",hc->itemBound);

        items=root[tempid];
        tmpId=items["icon"].asInt();
    }
}

void CombinePage::addtupian(Hcdata* hc)
{
    this->removeChildByTag(1001);
    this->removeChildByTag(1002);
    this->removeChildByTag(1003);
    this->removeChildByTag(1004);
    this->removeChildByTag(1005);
    //this->removeChildByTag(1006);
    this->removeChildByTag(1007);
    this->removeChildByTag(1008);
    this->removeChildByTag(1009);
    this->removeChildByTag(1010);
    this->removeChildByTag(1011);
    std::string nameA;
    std::string nameB;
    
    getjsonname(hc,nameA,nameB);
  
    int iconA = 0;
    int iconB = 0;
    int itemicons;
    char iconsA[100]="";
    char iconsB[100]="";
    getitempic(hc, itemicons);
    getjsonpic(hc, iconA, iconB);
    if(state==2)
    {
        sprintf(iconsA, "item_%d.png",iconA);
        sprintf(iconsB, "item_%d.png",iconB);
    }
    else if(state==1)
    {
        sprintf(iconsA, "item_%d.png",iconA);
        sprintf(iconsB, "item_%d.png",iconB);
    }
    else if(state==3)
    {
        sprintf(iconsA, "item_%d.png",iconA);
        sprintf(iconsB, "item_%d.png",iconB);
    }
    CCSprite* zuo = CCSprite::create(iconsA);//dj_08.png
    zuo->setPosition(ccp(127,675));
    this->addChild(zuo,1,1001);
    if(hc->itemAcnt<hc->itemAGoal)
    {
        zuo->setColor(ccc3(100, 100, 100));
    }
    
    char itemicon[100]="";
    
    sprintf(itemicon, "item_%d.png",itemicons);
    CCSprite* you = CCSprite::create(iconsB);
    you->setPosition(ccp(512,675));
    this->addChild(you,1,1002);
    if(hc->itemBcnt<hc->itemBGoal)
    {
        you->setColor(ccc3(100, 100, 100));
    }
    
    CCSprite* zhong = CCSprite::create(itemicon);//dj_16.png
    zhong->setPosition(ccp(320,753));
    this->addChild(zhong,1,1003);
    
    CCPoint zuopio = zuo->getPosition();
    CCSize zuosize = zuo->getContentSize();
    
    char zuozong1[100]="";
    sprintf(zuozong1, "%d",hc->itemAGoal);
    CCLabelAtlas * zuoshuzi1 = CCLabelAtlas::create(zuozong1, "shuzi.png", 11.2, 16, '0');
    //zuoshuzi1->setAnchorPoint(ccp(1,0.5));
    zuoshuzi1->setPosition(ccp(zuopio.x+zuosize.width/2-40,zuopio.y-zuosize.height/2+15));
    this->addChild(zuoshuzi1,1,1004);
    
    CCSprite* xiexian = CCSprite::create("xiexian.png");
    xiexian->setPosition(ccp(zuopio.x,zuopio.y-zuosize.height/2+20));
    this->addChild(xiexian,1,1005);
    
    char zuozong2[100]="";
    sprintf(zuozong2, "%d",hc->itemAcnt);
    CCLabelAtlas * zuoshuzi2 = CCLabelAtlas::create(zuozong2, "shuzi1.png", 11.2, 16, '0');
    //zuoshuzi2->setAnchorPoint(ccp(0,0.5));
    zuoshuzi2->setPosition(ccp(zuopio.x-zuosize.width/2+25,zuopio.y-zuosize.height/2+15));
    this->addChild(zuoshuzi2,1,1006);
    
    CCPoint youpio = you->getPosition();
    CCSize yousize = you->getContentSize();
    
    char youzong1[100]="";
    sprintf(youzong1, "%d",hc->itemBGoal);
    CCLabelAtlas * youshuzi1 = CCLabelAtlas::create(youzong1, "shuzi.png",  11.2, 16, '0');
    //youshuzi1->setAnchorPoint(ccp(1,0.5));
    youshuzi1->setPosition(ccp(youpio.x+yousize.width/2-40,youpio.y-yousize.height/2+15));
    this->addChild(youshuzi1,1,1007);
    
    CCSprite* xiexian1 = CCSprite::create("xiexian.png");
    xiexian1->setPosition(ccp(youpio.x,youpio.y-yousize.height/2+20));
    this->addChild(xiexian1,1,1008);
    
    char youzong2[100]="";
    sprintf(youzong2, "%d",hc->itemBcnt);
    CCLabelAtlas * youshuzi2 = CCLabelAtlas::create(youzong2, "shuzi1.png",  11.2, 16, '0');
    //youshuzi2->setAnchorPoint(ccp(0,0.5));
    youshuzi2->setPosition(ccp(youpio.x-yousize.width/2+25,youpio.y-yousize.height/2+15));
    this->addChild(youshuzi2,1,1009);
    
    CCLabelTTF * zuoname = CCLabelTTF::create(nameA.c_str(), "Helvetica", 20);
    CCLog("%s",nameA.c_str());
    //zuoshuzi2->setAnchorPoint(ccp(0,0.5));
    zuoname->setPosition(ccp(zuopio.x,zuopio.y+80));
    this->addChild(zuoname,1,1010);
    
    CCLabelTTF * youname = CCLabelTTF::create(nameB.c_str(), "Helvetica", 20);
    CCLog("%s",nameB.c_str());
    //zuoshuzi2->setAnchorPoint(ccp(0,0.5));
    youname->setPosition(ccp(youpio.x,youpio.y+80));
    this->addChild(youname,1,1011);
    this->removeChildByTag(00000);
}

void CombinePage::fasongHCID(hcpf *pf)
{
    this->removeChildByTag(00000);
    Loading *loding = Loading::create();
    this->addChild(loding,100,00000);
    
    int characterID = CData::getCData()->getCharactorId();
    Json::FastWriter  writer;
    Json::Value person;
    
    person["characterId"]=characterID;
    CCLog("%d",pf->idenity);
    person["tempid"]=pf->idenity;
    CCLog("请求的tempid= %d",pf->idenity);
    std::string  json_file=writer.write(person);

    SocketManager::getInstance()->sendMessage(json_file.c_str(), 2115);
    
    this->schedule(schedule_selector(CombinePage::getHCdata));
}

void CombinePage::getHCdata()
{
    Message* revMsg = (Message*)CData::getCData()->m_dictionary->objectForKey(2115);
    CData::getCData()->m_dictionary->removeObjectForKey(2115);
    
    if(revMsg)
    {
        this->unschedule(schedule_selector(CombinePage::getHCdata));
        char * ditu=revMsg->data;
        //解析数据
        if(ditu)
        {
            CCLOG("合成所需要的物品信息:%s",ditu);
            this->removeChildByTag(00000);
            jasonHCdata(ditu);
        }
        else
        {
            this->schedule(schedule_selector(CombinePage::getHCdata));
        }
    }
}

void CombinePage::jasonHCdata(char * data)
{
    Json::Reader read;
    Json::Value value;
    if(read.parse(data, value))
    {
        bool  ischenggong = value["result"].asBool();
        CCLog("ischenggong=%d",ischenggong);
        if(ischenggong == true)
        {
            Json::Value mapinfos = value["data"];
            
            Hcdata *hc = new Hcdata();
            hc->autorelease();
            hc->coinrequired = mapinfos["coinrequired"].asInt();
            hc->itemA        = mapinfos["itemA"].asInt();
            hc->itemAcnt     = mapinfos["itemAcnt"].asInt();
            hc->itemAGoal    = mapinfos["itemAGoal"].asInt();
            hc->itemB        = mapinfos["itemB"].asInt();
            hc->itemBcnt     = mapinfos["itemBcnt"].asInt();
            hc->itemBGoal    = mapinfos["itemBGoal"].asInt();
            hc->itemBound    = mapinfos["itemBound"].asInt();

            addtupian(hc);
        }
        else
        {
            std::string out = value["message"].asCString();
            //CCLog("out = %s",out.c_str());
            //这里输出错误原因：out
        }
    }
}

void CombinePage::fsqq(int tempid)
{
    this->removeChildByTag(00000);
    Loading *loding = Loading::create();
    this->addChild(loding,100,00000);
    
    int characterID = CData::getCData()->getCharactorId();
    Json::FastWriter  writer;
    Json::Value person;
    
    person["characterId"]=characterID;
    person["tempid"]=tempid;    
    std::string  json_file=writer.write(person);
        
    SocketManager::getInstance()->sendMessage(json_file.c_str(), 2116);
    
    this->schedule(schedule_selector(CombinePage::getjieguo));
}

void CombinePage::getjieguo()
{
    Message* revMsg = (Message*)CData::getCData()->m_dictionary->objectForKey(2116);
    CData::getCData()->m_dictionary->removeObjectForKey(2116);
    
    if(revMsg)
    {
        this->unschedule(schedule_selector(CombinePage::getjieguo));
        char * ditu=revMsg->data;
        //解析数据
        if(ditu)
        {
            CCLOG("得到物品的信息:%s",ditu);
            
            jsonjieguo(ditu);
        }
        else
        {
            this->schedule(schedule_selector(CombinePage::getjieguo));
        }
    }
}

void CombinePage::jsonjieguo(char* data)
{
    this->removeChildByTag(00000);
    
    Json::Reader read;
    Json::Value value;
    if(read.parse(data, value))
    {
        bool  ischenggong = value["result"].asBool();
        CCLog("%d",ischenggong);
        std::string out = value["message"].asCString();
        
        if(ischenggong)
        {
            addchenggong();
        }
        else
        {
            addshibai();
        }
    }
}

//添加成功动画  运用的是大图 plist
void CombinePage::addchenggong()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chenggong.plist");
    //用一个序列帧
    CCSpriteFrame *pFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cg_001.png");
    //创建精灵
    CCSprite* sprite= CCSprite::createWithSpriteFrame(pFrame);
    //创建数组
    CCArray * array=CCArray::create();
    for(int i =1;i<8;i++)
    {
        //循环的将所有动作从大图中抽离出来
        CCSpriteFrame *spf=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("cg_00%d.png",i)->getCString());
        
        array->addObject(spf);
    }
    //创建动作
    CCActionInterval * dz=CCAnimate::create(CCAnimation::createWithSpriteFrames(array,0.3f));
    //创建 动作完成后的回调事件
    CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(CombinePage::bfshibai));
    //执行所有动作的顺序
    CCFiniteTimeAction *Action=CCSequence::create(dz,func,NULL);
    
    //运行动作
    sprite->runAction(Action);
    sprite->setPosition(ccp(size.width/2, size.height-250));
    sprite->setTag(201);
    this->addChild(sprite,10);
}

void CombinePage::bfchenggong()
{
    
}

void CombinePage::addshibai()
{
    //加载整个plist   shibai.plist
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shibai.plist");
    //用一个序列帧
    CCSpriteFrame *pFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sb_001.png");
    CCSprite* sprite= CCSprite::createWithSpriteFrame(pFrame);
    CCArray * array=CCArray::create();
    for(int i =1;i<7;i++)
    {
        CCSpriteFrame *spf=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("sb_00%d.png",i)->getCString());
        
        array->addObject(spf);
    }
    
    CCActionInterval * dz=CCAnimate::create(CCAnimation::createWithSpriteFrames(array,0.2f));
    
    CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(CombinePage::bfshibai));
    CCFiniteTimeAction *Action=CCSequence::create(dz,func,NULL);
    
    sprite->runAction(Action);
    sprite->setPosition(ccp(size.width/2, size.height-250));
    sprite->setTag(201);
    this->addChild(sprite,10);
}

void CombinePage::bfshibai()
{
    this->removeChildByTag(201);
}