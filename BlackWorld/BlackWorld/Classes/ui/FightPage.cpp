#include "FightPage.h"
#include "json.h"
#include "TeXiao.h"
#include "GameOver.h"
#include "AnimatePacker.h"
#include "Singleton.h"
#include "SocketManager.h"
#include "CustomPop.h"

#define Time 2.5f

static int moreene[9]={0};
static int skillnumbers[20]={0};

void FightPage::onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader)
{
    initFireData();
}

FightPage::~FightPage()
{
    ene_dic->release();
    player_dic->release();
    jn_dic->release();
    chat_dic->release();
    if (sk)
    {
        delete sk;
    }
}

void FightPage::onExit()
{
    AnimatePacker::getInstance()->freeAnimations ("qishouTX.xml");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName ("result_attacked.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName ("jn_name.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName ("begin1.plist");
    
    CCLayer::onExit();
}

CCScene* FightPage::scene()
{
    CCScene *pScene = CCScene::create();
    
    extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("FightPage", FightPageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/FightPage.ccbi", pScene);
    node->setTag(902);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pScene;
}

bool FightPage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    
    size=CCDirector::sharedDirector()->getWinSize();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("bg2.mp3");
    
    return true;
}

void FightPage::initFireData()
{   
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("jn_name.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("begin1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("result_attacked.plist");
    
    label3=CCLabelTTF::create("", "Marker Felt", 30, CCSizeMake(200,40), kCCTextAlignmentLeft);
    label3->setColor(ccc3(0, 0, 255));
    label3->setPosition(ccp(520, 420));
    this->addChild(label3,5);
    
    ene_dic=CCDictionary::create();
    ene_dic->retain();
    player_dic=CCDictionary::create();
    player_dic->retain();
    jn_dic=CCDictionary::create();
    jn_dic->retain();
    chat_dic=CCDictionary::create();
    chat_dic->retain();
    
    //起手动作
    AnimatePacker::getInstance()->loadAnimations("qishouTX.xml");
    
    setTouchEnabled(true);
    
    first=false;
    kaishi=0;
    sk=new skillList();
    
    rounds=0;
    
    char * data2=CData::getCData()->getdata();
    Json::Reader read;
    Json::Value root;
    Json::Value data;
    Json::Value stepdata;
    int skill;
    int eee=0;
    if(read.parse(data2, root))
    {
        data=root["data"];
        stepdata=data["stepData"];
        for (int i=0; i<stepdata.size(); i++)
        {
            skill=stepdata[i]["skill"].asInt();
            if(skill!=610101)
            {
                for(int j=0;j<20;j++)
                {
                    if(skillnumbers[j]==skill)
                    {
                        break;
                    }
                    else
                    {
                        if(skillnumbers[j]==0)
                        {
                            skillnumbers[eee]=skill;
                            eee++;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    for(int i=0;i<20;i++)
    {
        if(skillnumbers[i]!=0)
        {
            const char * texiaoname=sk->skills(skillnumbers[i]);
            char names[40]="";
            sprintf(names, "%s.xml",texiaoname);
            AnimatePacker::getInstance()->loadAnimations(names);
        }
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bg2.mp3", true);
    
    this->cardData(data2);
    this->attackData(data2);
}

void FightPage::cardData(char *json)
{
    //此处将server端发送的总json数据进行解析
    Json::Reader read;
    Json::Value value;
    if (read.parse(json, value))
    {
        std::string out=value["message"].asCString();
        Json::Value data=value["data"];
        Json::Value setData=data["setData"];
        CData::getCData()->setFireJson(setData);
        
        _json=setData;
        int coin;
        coin=setData["star"].asInt();
        
        Json::Value startValue=data["startData"];
        Json::Value array;
        
        const char * name;
        int nengliang;
        int battleId;
        int pinzhi;
        int level;
        float chaCurrentHp;
        float chaTotalHp;
        int icon;
        int chaId;
        char tag[2];
        for (int i=0; i<startValue.size(); i++)
        {
            pos=startValue[i]["chaPos"].asInt();
            battleId=startValue[i]["chaBattleId"].asInt();
            direction=startValue[i]["chaDirection"].asInt();
            level=startValue[i]["chaLevel"].asInt();
            icon=startValue[i]["chaIcon"].asInt();
            pinzhi=startValue[i]["difficulty"].asInt();
            name=startValue[i]["chaName"].asCString();
            nengliang=startValue[i]["chaCurrentPower"].asInt();
            chaTotalHp=startValue[i]["chaTotalHp"].asDouble();
            chaCurrentHp=startValue[i]["chaCurrentHp"].asDouble();
            chaId=startValue[i]["chaId"].asInt();
            
            sprintf(tag, "%d%d",direction,pos);
            cout<<name<<endl;
            if(direction==1)
            {
                CCSprite *player1 = CCSprite::create();
                switch (pinzhi)
                {
                    case 1:
                        player1->initWithFile("common_qs_bkg_1.png");
                        break;
                    case 2:
                        player1->initWithFile("common_qs_bkg_1.png");
                        break;
                    case 3:
                        player1->initWithFile("common_qs_bkg_1.png");
                        break;
                    case 4:
                        player1->initWithFile("common_qs_bkg_1.png");
                        break;
                        
                    default:
                        player1->initWithFile("common_qs_bkg_1.png");
                        break;
                }
                
                char iconname[30]="";
                sprintf(iconname, "qs_%04d.png",icon);
                CCSprite *player111=CCSprite::create(iconname);
                player111->setScale(0.38);
                player111->setAnchorPoint(ccp(0, 0));
                player1->setTag(direction*10+pos);
                player1->addChild(player111);
                
                this->addChild(player1);
                
                CCSprite * xuetiao =CCSprite::create("PK_blood.png");
                xuetiao->setScaleX(chaCurrentHp/chaTotalHp+0.1);
                xuetiao->setAnchorPoint(ccp(0, 1));
                xuetiao->setTag(1000+direction*10+pos);
                xuetiao->setPosition(ccp(2, 5));
                player1->addChild(xuetiao,2);
                player1->setScale(0.6);
                
                if(pos>=1&&pos<=3)
                {
                    player1->setPosition(ccp(size.width/2+(pos-2)*150, size.height/2-70));
                    player111->setPosition(ccp(0, 5));
                }
                
                if (pos>3&&pos<=6)
                {
                    player1->setPosition(ccp(size.width/2+(pos-5)*150, size.height/2-190));
                }
                
                if(pos>6&&pos<=9)
                {
                    player1->setPosition(ccp(size.width/2+(pos-8)*150, size.height/2-310));
                }
            }
            else if(direction==2)
            {
                CCSprite *ene1 = CCSprite::create();
                
                switch (pinzhi)
                {
                    case 1:
                        ene1->initWithFile("common_qs_bkg_1.png");
                        break;
                    case 2:
                        ene1->initWithFile("common_qs_bkg_1.png");
                        break;
                    case 3:
                        ene1->initWithFile("common_qs_bkg_1.png");
                        break;
                    case 4:
                        ene1->initWithFile("common_qs_bkg_1.png");
                        break;
                        
                    default:
                        ene1->initWithFile("common_qs_bkg_1.png");
                        break;
                }
                
                ene1->setScale(0.9);
                char iconname[30]="";
                sprintf(iconname, "qs_%04d.png",icon);
                CCSprite * ene111=CCSprite::create(iconname);
                ene111->setScale(0.38);
                ene1->setTag(direction*10+pos);
                ene1->addChild(ene111);
                this->addChild(ene1);
                
                CCSprite * xuetiao =CCSprite::create("PK_blood.png");
                xuetiao->setAnchorPoint(ccp(0, 1));
                xuetiao->setTag(1000+direction*10+pos);
                xuetiao->setScaleX(chaCurrentHp/chaTotalHp+0.1);
                xuetiao->setPosition(ccp(2, 5));
                ene1->addChild(xuetiao,2);
                ene1->setScale(0.6);
                
                if(pos>=1&&pos<=3)
                {
                    ene1->setPosition(ccp(size.width/2+(pos-2)*150, size.height/2+65));
                    ene111->setAnchorPoint(ccp(0, 0));
                }
                
                if (pos>3&&pos<=6)
                {
                    ene1->setPosition(ccp(size.width/2+(pos-5)*150, size.height/2+190));
                    ene111->setAnchorPoint(ccp(0, 0));
                }
                if(pos>6&&pos<=9)
                {
                    ene1->setPosition(ccp(size.width/2+(pos-8)*150, size.height/2+310));
                    ene111->setAnchorPoint(ccp(0, 0));
                }
            }
        }
    }
}

void FightPage::attackData(char * jsonData)
{
    Json::Reader read;
    Json::Value value;
    int chaBattleId;
    int eneBattleId;
    int eneChaId;
    int eneActionId;
    int eneChangeHP;
    int eneCurrentHp;
    int eneTotalHp;
    int chaId;
    int dir;
    int actionId;
    int skillId;
    int chaCurrentPower;

    Json::Value eneArr;
    if (read.parse(jsonData, value))
    {
        //此处解析攻击json数据
        Json::Value data=value["data"];
        battleresult=data["battleResult"].asInt();
        CData::getCData()->setFireResult(battleresult);
        stepData=data["stepData"];
        
        chaBattleId=stepData[rounds]["chaBattleId"].asInt();
        dir=stepData[rounds]["chaDirection"].asInt();
        eneArr=stepData[rounds]["enemyChaArr"];
        chaId=stepData[rounds]["chaId"].asInt();
        actionId=stepData[rounds]["actionId"].asInt();
        skillId=stepData[rounds]["skill"].asInt();
        chaCurrentPower=stepData[rounds]["chaCurrentPower"].asInt();
        _chaCurrentPower=chaCurrentPower;
        if(eneArr.size()==1)
        {
            eneBattleId=eneArr[rounds]["enemyBattleId"].asInt();
            eneChaId=eneArr[rounds]["enemyChaId"].asInt();
            eneActionId=eneArr[rounds]["enemyActionId"].asInt();
            eneChangeHP=eneArr[rounds]["enemyChangeHp"].asInt();
            eneCurrentHp=eneArr[rounds]["enemyCurrentHp"].asInt();
            eneTotalHp=eneArr[rounds]["enemyTotalHp"].asInt();
            _eneCurrentPower=eneArr[rounds]["enemyCurrentPower"].asInt();
            if(chaBattleId==-1)
            {
                currHp=eneCurrentHp;
                totalHp=eneTotalHp;
                changeHp=eneChangeHP;
                this->debuff(eneBattleId, eneChangeHP);
            }
            else
            {
                _skillId = skillId;
                changeHp = eneChangeHP;
                currHp = eneCurrentHp;
                eneBID = eneBattleId;
                totalHp = eneTotalHp;
                _actionId = actionId;
                this->playStarting(eneBattleId, chaBattleId);
            }
        }
        else if(eneArr.size()>1)
        {
            int eneChaid[eneArr.size()];
            int eneChanHp[eneArr.size()];
            int eneCurrHp[eneArr.size()];
            int eneTotHp[eneArr.size()];
            int eneBattId[eneArr.size()];
            int eneActId[eneArr.size()];
            for(int i=0;i<eneArr.size();i++)
            {
                eneBattleId=eneArr[i]["enemyBattleId"].asInt();
                eneChaId=eneArr[i]["enemyChaId"].asInt();
                eneActionId=eneArr[i]["enemyActionId"].asInt();
                eneChangeHP=eneArr[i]["enemyChangeHp"].asInt();
                eneCurrentHp=eneArr[i]["enemyCurrentHp"].asInt();
                eneTotalHp=eneArr[i]["enemyTotalHp"].asInt();
                eneChaid[i]=eneChaId;
                eneChanHp[i]=eneChangeHP;
                eneCurrHp[i]=eneCurrentHp;
                eneTotHp[i]=eneTotalHp;
                eneActId[i]=eneActionId;
                eneBattId[i]=eneBattleId;
            }
            
            this->moreActionAttack(eneBattId, eneChaid, eneChanHp, eneCurrHp, eneTotHp, eneActId, chaBattleId, chaId,eneArr.size(),skillId);
        }
    }
    
    rounds++;
}

//掉血buff
void FightPage::debuff(int eneBid, int eneChangeHp)
{
    enemy=(CCSprite *)this->getChildByTag(eneBid);
    
    tx=CCSprite::create("attacked_img.png");
    tx->setPosition(ccp(70, 75));
    
    char cHp[10]={0};
    sprintf(cHp, "%d",eneChangeHp);
    
    reduB=CCLabelAtlas::create(cHp, "number.png", 30, 50, '-');
    reduB->setTag(0020);
    reduB->setColor(ccc3(255, 0, 0));
    reduB->setAnchorPoint(ccp(0.5, 0));
    reduB->setPosition(ccp(enemy->getContentSize().width/2, enemy->getContentSize().height/2));
    enemy->addChild(reduB);
    enemy->addChild(tx);
    
    CCActionInterval * set_big=CCScaleTo::create(0.2/Time, 1.5);
    CCActionInterval * set_nor=CCScaleTo::create(0.2/Time, 1);
    CCFadeOut *a_jiaxue_fadeout=CCFadeOut::create(0.5f/Time);
    CCCallFunc * reduceBlood=CCCallFunc::create(this, callfunc_selector(FightPage::changeBlood));
    CCFiniteTimeAction *jianxueaction=CCSequence::create(set_big,set_nor,a_jiaxue_fadeout,reduceBlood,NULL);
    reduB->runAction(jianxueaction);
    
    zhenping(enemy);
    
    eneBID=eneBid;
    _actionId=99;
}

void FightPage::playStarting(int eneBattleId,int chaBattleID)
{
    enemy=(CCSprite *)this->getChildByTag(eneBattleId);
    player=(CCSprite *)this->getChildByTag(chaBattleID);
    chaIdd=chaBattleID;
    //播放开始战斗动画
    if (kaishi==0)
    {
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("01.png");
        explosion = CCSprite::createWithSpriteFrame(pFrame);
        explosion->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(explosion);
        
        CCArray * kneif=CCArray::create();
        for (int i=2;i<=10; i++)
        {
            char strTemp[3];
            sprintf(strTemp, "%02d.png", i);
            kneif->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strTemp));
        }
        
        CCActionInterval * delay=CCDelayTime::create(4/Time);
        CCAnimation * Action=CCAnimation::createWithSpriteFrames(kneif, 0.25f/Time);
        CCActionInterval* act = CCAnimate::create(Action);
        CCFiniteTimeAction *playerAct=CCSequence::create(delay,act,CCCallFunc::create(this, callfunc_selector(FightPage::doAttack)),NULL);
        explosion->runAction(playerAct);
    }
    
    //此处执行攻击者动画
    else if(kaishi==1)
    {
        this->doAttack();
    }
}

//此处执行攻击者动画
void FightPage::doAttack()
{
    if(kaishi==0)
    {
        kaishi=1;
        explosion->removeFromParentAndCleanup(true);
    }
    
    //角色
    if(chaIdd/10==1)
    {
        const char * texiaoname=sk->skills(_skillId);
        if(!strcmp(texiaoname, "sq")||!strcmp(texiaoname, "sl")||!strcmp(texiaoname, "sj")||!strcmp(texiaoname, "sf")||!strcmp(texiaoname, "sr")||!strcmp(texiaoname, "sdjx")||!strcmp(texiaoname, "mrj")||!strcmp(texiaoname, "wlbf"))
        {
            
        }
        else
        {
            
        }
        
        CCActionInterval * moveon=CCMoveTo::create(0.12/Time, ccp(player->getPosition().x, player->getPosition().y+20));
        CCActionInterval * movedown=CCMoveTo::create(0.12/Time, ccp(player->getPosition().x, player->getPosition().y));
        CCFiniteTimeAction *playerAct1=CCSequence::create(moveon,movedown,CCCallFunc::create(this, callfunc_selector(FightPage::playSkill)),NULL);
        player->runAction(playerAct1);
    }
    
    //怪
    else if(chaIdd/10==2)
    {
        CCActionInterval * moveon=CCMoveTo::create(0.12/Time, ccp(player->getPosition().x, player->getPosition().y-20));
        CCActionInterval * movedown=CCMoveTo::create(0.12/Time, ccp(player->getPosition().x, player->getPosition().y));
        CCFiniteTimeAction *playerAct1=CCSequence::create(moveon,movedown,CCCallFunc::create(this, callfunc_selector(FightPage::playSkill)),NULL);
        player->runAction(playerAct1);
    }
}

//放技能
void FightPage::playSkill()
{
    int s1=0;
    s1=rand()%3;
    switch(s1)
    {
        case 0:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("battleAttack1.mp3");
            break;
        case 1:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("battleAttack2.mp3");
            break;
        case 2:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("battleAttack3.mp3");
            break;
        default:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("battleAttack1.mp3");
            break;
    }
    
    if(_actionId==98)
    {
        skill *sk=this->returnskills(_skillId);
        skillList * skk=new skillList();
        const char * namees=skk->releaseName(sk->releaseEffect);
        
        char combine[40]="";
        sprintf(combine, "%s_01.png",namees);
        texiao=CCSprite::createWithSpriteFrameName(combine);
        texiao->setTag(2078);
        texiao->setPosition(ccp(player->getPosition().x, player->getPosition().y));
        texiao->runAction(AnimatePacker::getInstance()->getAnimate(namees));
        this->addChild(texiao,5);
        
        CCActionInterval * moveto=CCMoveTo::create(1/Time, ccp(player->getPosition().x, player->getPosition().y));
        CCFiniteTimeAction *playerAct=CCSequence::create(moveto,CCCallFunc::create(this, callfunc_selector(FightPage::playSkillName)),NULL);
        texiao->runAction(playerAct);
    }
    else if(_actionId==99)
    {
        normalAttack = CCSprite::create("attack_img.png");
        normalAttack->setTag(0012);
        
        if(chaIdd/10==1)
        {
        }
        else if(chaIdd/10==2)
        {
            normalAttack->setRotation(180);
        }
        normalAttack->setPosition(ccp(player->getPosition().x, player->getPosition().y+30));
        this->addChild(normalAttack,5);
        
        CCActionInterval * moveto=CCMoveTo::create(0.4/Time, ccp(player->getPosition().x, player->getPosition().y+30));
        CCFiniteTimeAction *playerAct=CCSequence::create(moveto,CCCallFunc::create(this, callfunc_selector(FightPage::attacked)),NULL);
        normalAttack->runAction(playerAct);
    }
}

//播放技能名称
void FightPage::playSkillName()
{
    this->removeChildByTag(2078,true);
    
    const char * skillname=sk->skillname(_skillId);
    if(!strcmp(skillname, ""))
    {
        CCSpriteFrame *pFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(skillname);
        label=CCSprite::createWithSpriteFrame(pFrame);
        label->setPosition(ccp(size.width/2, size.height/2));
        label->setTag(999);
        this->addChild(label);
        
        CCActionInterval *action=CCMoveTo::create(0.88, ccp(size.width/2, size.height/2));
        CCFadeOut *fadeout=CCFadeOut::create(0.12f);
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::whethertwice));
        CCFiniteTimeAction *allaction=CCSequence::create(action,fadeout,func,NULL);
        label->runAction(allaction);
        this->whethertwice();
    }
    else
    {
        this->whethertwice();
    }
}

void FightPage::whethertwice()
{
    skk=(skill *)this->returnskills(_skillId);
    if(skk->throwEffectId!=0)
    {
        this->throwtwice();
    }
    else
    {
        this->attacked();
    }
}

void FightPage::throwtwice()
{
    enemy=(CCSprite *)this->getChildByTag(eneBID);
    player=(CCSprite *)this->getChildByTag(chaIdd);
    
    const char * texiaoname=sk->skills(_skillId);
    CCParticleSystemQuad * stars;
    if(!strcmp(texiaoname, "tx"))
    {
        stars=CCParticleSystemQuad::create("touxi_1.plist");
        stars->setPosition(player->getPosition());
        stars->setTag(8032);
        this->addChild(stars);
        CCActionInterval * delay=CCDelayTime::create(0.1);
        CCActionInterval * moveto=CCMoveTo::create(0.5/Time, enemy->getPosition());
        
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::attacked));
        CCFiniteTimeAction * final=CCSequence::create(delay,moveto,func,NULL);
        stars->runAction(final);
    }
    else
    {
        char txname[50]="";
        sprintf(txname, "%s.xml",texiaoname);
        
        char combine[50]="";
        sprintf(combine, "%s_01_01.png",texiaoname);
        tz=CCSprite::createWithSpriteFrameName(combine);
        
        char animate[50]="";
        sprintf(animate, "%s_tz",texiaoname);
        tz->runAction(CCRepeatForever::create( AnimatePacker::getInstance()->getAnimate(animate)));
        float y1;
        float y2;
        float x1;
        float x2;
        float angel;
        x1=player->getPosition().x;
        x2=enemy->getPosition().x;
        y1=player->getPosition().y;
        y2=enemy->getPosition().y;
        
        angel=(y1-y2)/(x1-x2);
        angel=atanf(angel)/M_PI*180;
        
        tz->setPosition(ccp(x1, y1+50));
        if(angel<0)
        {
            tz->setRotation(90-angel);
            
        }
        else
        {
            tz->setRotation(90-angel);
        }
        
        tz->setTag(8078);
        this->addChild(tz,2);
        
        CCActionInterval * delay=CCDelayTime::create(0.1);
        CCActionInterval * moveto=CCMoveTo::create(0.5/Time, enemy->getPosition());
        
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::attacked));
        CCFiniteTimeAction * final=CCSequence::create(delay,moveto,func,NULL);
        tz->runAction(final);
    }
}

void FightPage::attacked()
{
    this->removeChildByTag(8032,true);
    this->removeChildByTag(999,true);
    if(_actionId==99)
    {
        normalAttack->stopAllActions();
        this->removeChildByTag(0012,true);
        
        tx=CCSprite::create("attacked_img.png");
        tx->setTag(9099);
        tx->setPosition(ccp(70, 75));
        
        char cHp[10]={0};
        sprintf(cHp, "%d",changeHp);
        reduB=CCLabelAtlas::create(cHp, "number.png", 30, 50, '-');
        reduB->setColor(ccc3(255, 0, 0));
        reduB->setAnchorPoint(ccp(0.5, 0));
        reduB->setPosition(ccp(enemy->getContentSize().width/2, enemy->getContentSize().height/2));
        
        enemy->addChild(reduB);
        enemy->addChild(tx);
        CCActionInterval *a_jianxue=CCMoveTo::create(0.45/Time, ccp(reduB->getPosition().x, reduB->getPosition().y+80));
        CCActionInterval * set_big=CCScaleTo::create(0.4/Time, 1.5);
        CCFadeOut *a_jiaxue_fadeout=CCFadeOut::create(0.5f/Time);
        CCCallFunc * reduceBlood=CCCallFunc::create(this, callfunc_selector(FightPage::changeBlood));
        CCFiniteTimeAction *jianxueaction=CCSequence::create(set_big,a_jianxue,a_jiaxue_fadeout,reduceBlood,NULL);
        reduB->runAction(jianxueaction);
        
        zhenping(enemy);
    }
    else if(_actionId==98)
    {
        this->removeChildByTag(8078,true);
        
        char cHp[10]={0};
        sprintf(cHp, "%d",changeHp);
        reduB1=CCLabelAtlas::create(cHp, "number.png", 30, 50, '-');
        reduB1->setColor(ccc3(255, 0, 0));
        reduB1->setAnchorPoint(ccp(0.5, 0));
        reduB1->setPosition(ccp(enemy->getContentSize().width/2, enemy->getContentSize().height/2));
        enemy->addChild(reduB1);
        
        const char * texiaoname=sk->skills(_skillId);
        char combine[50]="";
        sprintf(combine, "%s_01.png",texiaoname);
        mz=CCSprite::createWithSpriteFrameName(combine);
        mz->runAction(AnimatePacker::getInstance()->getAnimate(texiaoname));
        mz->setTag(9098);
        mz->setPosition(ccp(50, 50));
        enemy->addChild(mz);
        CCActionInterval * delay=CCDelayTime::create(1);
        CCCallFunc * reduceBlood=CCCallFunc::create(this, callfunc_selector(FightPage::changeBlood));
        CCFiniteTimeAction * final=CCSequence::create(delay,reduceBlood,NULL);
        mz->runAction(final);
        
        CCActionInterval * set_big=CCScaleTo::create(0.2/Time, 1.5);
        CCActionInterval * set_nor=CCScaleTo::create(0.2/Time, 1);
        CCFadeOut *a_jiaxue_fadeout=CCFadeOut::create(0.5f/Time);
        CCFiniteTimeAction *jianxueaction=CCSequence::create(set_big,set_nor,a_jiaxue_fadeout,NULL);
        reduB1->runAction(jianxueaction);
        
        zhenping(enemy);
    }
}

void FightPage::changeBlood()
{
    if (_actionId==98)
    {
        enemy->removeChildByTag(9098,true);
        reduB1->removeFromParentAndCleanup(true);
    }
    else if(_actionId==99)
    {
        enemy->removeChildByTag(9099,true);
        reduB->removeFromParentAndCleanup(true);
    }
    
    CCSprite * blood=(CCSprite *)enemy->getChildByTag(1000+eneBID);
    double percent;
    if(currHp+changeHp<=0)
    {
        CCActionInterval * scal=CCScaleTo::create(0.75/Time, 0, 1);
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::MeasureEneDead));
        CCFiniteTimeAction *Action=CCSequence::create(scal,func,NULL);
        blood->runAction(Action);
    }
    else if(currHp+changeHp>0)
    {
        percent=((double)(currHp+changeHp)/(double)totalHp);
        
        CCActionInterval *  scal=CCScaleTo::create(0.75/Time, percent, 1);
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::MeasureEneDead));
        CCFiniteTimeAction *Action=CCSequence::create(scal,func,NULL);
        blood->runAction(Action);
    }
}

void FightPage::MeasureEneDead(CCObject * eneTag)
{
    if (_actionId==98)
    {
        enemy->removeChildByTag(9098,true);
    }
    else if(_actionId==99)
    {
        enemy->removeChildByTag(9099,true);
    }
    if(changeHp+currHp<=0)
    {
        CCActionInterval * fadeout=CCFadeOut::create(0.2);
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::removeit));
        CCFiniteTimeAction * final=CCSequence::create(fadeout,func,NULL);
        enemy->runAction(final);
    }
    else
    {
        this->checkOut();
    }
}

void FightPage::checkOut()
{
    chaBattleId=0;
    int eneBattleId;
    int eneChaId;
    int eneActionId;
    int eneChangeHP;
    int eneCurrentHp;
    int eneTotalHp;
    int chaId;
    int actionID;
    int skillId;
    int chaCurrentPower;
    Json::Value eneArr;
    Json::Value buff;
    chaBattleId=stepData[rounds]["chaBattleId"].asInt();
    
    if(chaBattleId!=0)
    {
        eneArr=stepData[rounds]["enemyChaArr"];
        chaId=stepData[rounds]["chaId"].asInt();
        actionID=stepData[rounds]["actionId"].asInt();
        skillId=stepData[rounds]["skill"].asInt();
        chaCurrentPower=stepData[rounds]["chaCurrentPower"].asInt();
        
        if(eneArr.size()==1)
        {
            int round=0;
            if(eneChaId-10<10)
            {
                fireinfo * fire=new fireinfo();
                fire->personId=eneChaId-10;
                
                buff=eneArr[round]["enemyBuffArr"];
                for (int i=0; i<buff.size(); i++)
                {
                    fire->buff[i]=buff[i].asInt();
                }
                player_dic->setObject(fire, fire->personId);
            }
            
            eneBattleId=eneArr[round]["enemyBattleId"].asInt();
            eneChaId=eneArr[round]["enemyChaId"].asInt();
            eneActionId=eneArr[round]["enemyActionId"].asInt();
            eneChangeHP=eneArr[round]["enemyChangeHp"].asInt();
            eneCurrentHp=eneArr[round]["enemyCurrentHp"].asInt();
            eneTotalHp=eneArr[round]["enemyTotalHp"].asInt();
            
            if(chaBattleId==-1)
            {
                currHp=eneCurrentHp;
                totalHp=eneTotalHp;
                changeHp=eneChangeHP;
                this->debuff(eneBattleId, eneChangeHP);
            }
            else
            {
                _skillId = skillId;
                changeHp = eneChangeHP;
                currHp = eneCurrentHp;
                eneBID = eneBattleId;
                totalHp = eneTotalHp;
                _actionId = actionID;
                this->playStarting(eneBattleId, chaBattleId);
            }
        }
        else if(eneArr.size()>1)
        {
            int eneChaid[eneArr.size()];
            int eneChanHp[eneArr.size()];
            int eneCurrHp[eneArr.size()];
            int eneTotHp[eneArr.size()];
            int eneBattId[eneArr.size()];
            int eneActId[eneArr.size()];
            for(int i=0;i<eneArr.size();i++)
            {
                if(eneChaId-10<10)
                {
                    fireinfo * fire = new fireinfo();
                    fire->personId=eneChaId-10;
                    
                    buff=eneArr[i]["enemyBuffArr"];
                    for (int i=0; i<buff.size(); i++)
                    {
                        fire->buff[i]=buff[i].asInt();
                    }
                    if(buff.size()==0)
                    {
                        * fire->buff=0;
                        player_dic->setObject(fire, fire->personId);
                    }
                    else
                    {
                        player_dic->setObject(fire, fire->personId);
                    }
                }
                
                eneBattleId=eneArr[i]["enemyBattleId"].asInt();
                eneChaId=eneArr[i]["enemyChaId"].asInt();
                eneActionId=eneArr[i]["enemyActionId"].asInt();
                eneChangeHP=eneArr[i]["enemyChangeHp"].asInt();
                eneCurrentHp=eneArr[i]["enemyCurrentHp"].asInt();
                eneTotalHp=eneArr[i]["enemyTotalHp"].asInt();
                eneChaid[i]=eneChaId;
                eneChanHp[i]=eneChangeHP;
                eneCurrHp[i]=eneCurrentHp;
                eneTotHp[i]=eneTotalHp;
                eneActId[i]=eneActionId;
                eneBattId[i]=eneBattleId;
            }
            
            this->moreActionAttack(eneBattId, eneChaid, eneChanHp, eneCurrHp, eneTotHp, eneActId, chaBattleId, chaId,eneArr.size(),skillId);
        }
        
        int e1=0;
        int e2=0;
        int e3=0;
        for (int i=0; i<9; i++)
        {
            fireinfo *fire=(fireinfo *) player_dic->objectForKey(i);
            if(fire)
            {
                for(int j=0;j<5;j++)
                {
                    if(fire->buff[j]==100005)
                    {
                        e1++;
                    }else if(fire->buff[j]==100003)
                    {
                        e2++;
                    }else if(fire->buff[j]==100001)
                    {
                        e3++;
                    }
                }
            }
        }
        if(e1==0)
        {
            buff1->setVisible(false);
        }
        else if(e2==0)
        {
            buff2->setVisible(false);
        }
        else if(e3==0)
        {
            buff3->setVisible(false);
        }
        
        rounds++;
    }
    else
    {
        AnimatePacker::getInstance()->freeAnimations ("qishouTX.xml");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName ("result_attacked.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName ("jn_name.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName ("begin1.plist");
        for(int i=0;i<20;i++)
        {
            if(skillnumbers[i]!=0)
            {
                const char * texiaoname=sk->skills(skillnumbers[i]);
                char names[40]="";
                sprintf(names, "%s.xml",texiaoname);
                AnimatePacker::getInstance()->freeAnimations(names);
            }
        }
        
        GameOver *game=GameOver::create();
        game->setTag(8021);
        this->addChild(game,9999);
    }
}

void FightPage::moreActionAttack(int *eneBattId, int *eneChaid, int * eneChanHp, int * eneCurrHp, int * eneTotHp, int * eneActId, int chaBattleId, int chaId,int length,int skillId)
{
    max=1;
    for(int i=0;i<length;i++)
    {
        _eneBattId[i]=eneBattId[i];
        _eneChaid[i]=eneChaid[i];
        _eneChanHp[i]=eneChanHp[i];
        _eneCurrHp[i]=eneCurrHp[i];
        _eneTotHp[i]=eneTotHp[i];
        _eneActId[i]=eneActId[i];
    }
    
    _skillId=skillId;
    _chaBattleId=chaBattleId;
    _chaId=chaId;
    _length=length;
    if (kaishi==0)
    {
        kaishi=1;
        
        CCArray * kneif=CCArray::create();
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("01.png");
        CCSprite *explosion = CCSprite::createWithSpriteFrame(pFrame);
        explosion->setTag(1111);
        explosion->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(explosion);
        for(int i=2;i<=10; i++)
        {
            CCSpriteFrame *spf=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%02d.png",i)->getCString());
            
            kneif->addObject(spf);
        }
        
        //播放开始战斗动画
        CCActionInterval * delay=CCDelayTime::create(1);
        CCAnimation * Action=CCAnimation::createWithSpriteFrames(kneif, 0.1f);
        CCCallFunc * fun2=CCCallFunc::create(this, callfunc_selector(FightPage::moreattactnext));
        CCFiniteTimeAction *playerAct=CCSequence::create(delay,CCAnimate::create(Action),fun2,NULL);
        explosion->runAction(playerAct);
    }
    else
    {
        this->moreattactnext();
    }
}

void FightPage::moreattactnext()
{
    player=(CCSprite *)this->getChildByTag(_chaBattleId);
    CCFadeOut *playfadeout=CCFadeOut::create(0.18f);
    CCFadeIn *playerfadein=CCFadeIn::create(0.3f);
    CCCallFunc * fun2=CCCallFunc::create(this, callfunc_selector(FightPage::moreAttackName));
    CCFiniteTimeAction *playerAct=CCSequence::create(playfadeout,playerfadein,fun2,NULL);
    player->runAction(playerAct);
}

void FightPage::moreAttackName()
{
    this->removeChildByTag(1111);
    const char * skillname=sk->skillname(_skillId);
    CCSpriteFrame *pFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(skillname);
    label=CCSprite::createWithSpriteFrame(pFrame);
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setTag(999);
    this->addChild(label);
    
    CCActionInterval *action=CCMoveTo::create(0.88, ccp(size.width/2, size.height/2));
    CCFadeOut *fadeout=CCFadeOut::create(0.12f);
    CCCallFunc * func1=CCCallFunc::create(this, callfunc_selector(FightPage::moreAttackTitle));
    CCFiniteTimeAction *allaction=CCSequence::create(action,fadeout,func1,NULL);
    label->runAction(allaction);
}

void FightPage::moreAttackTitle()
{
    this->removeChildByTag(999,true);
    skill *sk=this->returnskills(_skillId);
    skillList * skk=new skillList();
    if(eneBID-10<10)
    {
        if(_skillId==100002)
        {
            buffbegin(1);
            label3->setColor(ccc3(0, 255, 0));
            label3->setOpacity(GLubyte(0));
            label3->setString("我方防御增加");
            label3->setPosition(ccp(520, 420));
            CCFadeIn * fadein=CCFadeIn::create(0.5);
            CCDelayTime * delaytime=CCDelayTime::create(1);
            CCMoveTo * moveon=CCMoveTo::create(1, ccp(520, 520));
            CCFadeOut * fadeout=CCFadeOut::create(1);
            CCSequence * seq=CCSequence::create(fadein,delaytime,moveon,fadeout,NULL);
            
            label3->runAction(seq);
            
        }
        else if(_skillId==100005)
        {
            buffbegin(2);
            label3->setColor(ccc3(0, 255, 0));
            label3->setOpacity(GLubyte(0));
            label3->setString("我方攻击增加");
            label3->setPosition(ccp(520, 420));
            CCFadeIn * fadein=CCFadeIn::create(0.5);
            CCDelayTime * delaytime=CCDelayTime::create(1);
            CCMoveTo *  moveon=CCMoveTo::create(1, ccp(520, 520));
            CCFadeOut * fadeout=CCFadeOut::create(1);
            CCSequence * seq=CCSequence::create(fadein,delaytime,moveon,fadeout,NULL);
            
            label3->runAction(seq);
        }
        else if(_skillId==100001)
        {
            buffbegin(3);
            
            label3->setColor(ccc3(0, 255, 0));
            label3->setOpacity(GLubyte(0));
            label3->setString("我方暴击增加");
            label3->setPosition(ccp(520, 420));
            CCFadeIn * fadein=CCFadeIn::create(0.5);
            CCDelayTime * delaytime=CCDelayTime::create(1);
            CCMoveTo *  moveon=CCMoveTo::create(1, ccp(520, 520));
            CCFadeOut * fadeout=CCFadeOut::create(1);
            CCSequence * seq=CCSequence::create(fadein,delaytime,moveon,fadeout,NULL);
            
            label3->runAction(seq);
        }
    }
    else
    {
        if(_skillId==100002)
        {
            label3->setColor(ccc3(255, 0, 0));
            label3->setOpacity(GLubyte(0));
            label3->setString("敌方防御增加");
            label3->setPosition(ccp(520, 420));
            CCFadeIn * fadein=CCFadeIn::create(0.5);
            CCDelayTime * delaytime=CCDelayTime::create(1);
            CCMoveTo * moveon=CCMoveTo::create(1, ccp(520, 520));
            CCFadeOut * fadeout=CCFadeOut::create(1);
            CCSequence * seq=CCSequence::create(fadein,delaytime,moveon,fadeout,NULL);
            
            label3->runAction(seq);
        }
        else if(_skillId==100005)
        {
            buffbegin(2);
            label3->setColor(ccc3(255, 0, 0));
            label3->setOpacity(GLubyte(0));
            label3->setString("敌方攻击增加");
            label3->setPosition(ccp(520, 420));
            CCFadeIn * fadein=CCFadeIn::create(0.5);
            CCDelayTime * delaytime=CCDelayTime::create(1);
            CCMoveTo *  moveon=CCMoveTo::create(1, ccp(520, 520));
            CCFadeOut * fadeout=CCFadeOut::create(1);
            CCSequence * seq=CCSequence::create(fadein,delaytime,moveon,fadeout,NULL);
            
            label3->runAction(seq);
        }
        else if(_skillId==100001)
        {
            buffbegin(3);
            label3->setColor(ccc3(255, 0, 0));
            label3->setOpacity(GLubyte(0));
            label3->setString("敌方暴击增加");
            label3->setPosition(ccp(520, 420));
            CCFadeIn * fadein=CCFadeIn::create(0.5);
            CCDelayTime * delaytime=CCDelayTime::create(1);
            CCMoveTo *  moveon=CCMoveTo::create(1, ccp(520, 520));
            CCFadeOut * fadeout=CCFadeOut::create(1);
            CCSequence * seq=CCSequence::create(fadein,delaytime,moveon,fadeout,NULL);
            
            label3->runAction(seq);
        }
    }
    
    const char * namees=skk->releaseName(sk->releaseEffect);
    char combine[40]="";
    sprintf(combine, "%s_01.png",namees);
    texiao=CCSprite::createWithSpriteFrameName(combine);
    texiao->setTag(0014);
    texiao->runAction(AnimatePacker::getInstance()->getAnimate(namees));
    texiao->setPosition(ccp(player->getPosition().x, player->getPosition().y));
    this->addChild(texiao,5);
    CCActionInterval * moveto=CCMoveTo::create(0.7, ccp(player->getPosition().x, player->getPosition().y));
    CCCallFunc * fun2=CCCallFunc::create(this, callfunc_selector(FightPage::moreAttacked));
    CCFiniteTimeAction *playerAct=CCSequence::create(moveto,fun2,NULL);
    
    texiao->runAction(playerAct);
}

void FightPage::moreAttacked()
{
    texiao->removeFromParentAndCleanup(true);
    
    j=0;
    ea=0;
    const char * texiaoname=sk->skills(_skillId);
    char txname[50]="";
    sprintf(txname, "%s.xml",texiaoname);
    
    for (int i=0; i<_length; i++)
    {
        moreene[j]=i;
        j++;
        
        if(_skillId==100001)
        {
            
        }
        else if(_skillId==100002)
        {
            
        }
        enemy=(CCSprite *)this->getChildByTag(_eneBattId[i]);
        enemy->setTag(_eneBattId[i]);
        
        char combine[50]="";
        sprintf(combine, "%s_01.png",texiaoname);
        mz=CCSprite::createWithSpriteFrameName(combine);
        mz->runAction(AnimatePacker::getInstance()->getAnimate(texiaoname));
        mz->setPosition(ccp(50, 50));
        mz->setTag(9023+i);
        char changeBlood[20]="";
        sprintf(changeBlood, "%d",_eneChanHp[i]);
        CCLabelAtlas * reduB=CCLabelAtlas::create(changeBlood, "number.png", 30, 50, '-');
        reduB->setColor(ccc3(255, 0, 0));
        reduB->setTag(9800+i);
        reduB->setAnchorPoint(ccp(0.5, 0));
        reduB->setPosition(ccp(enemy->getContentSize().width/2, enemy->getContentSize().height/2));
        enemy->addChild(mz);
        enemy->addChild(reduB);
        CCActionInterval * set_big=CCScaleBy::create(0.2, 1.5);
        CCActionInterval * set_nor=CCScaleBy::create(0.2, 1);
        
        CCFadeOut *a_jiaxue_fadeout1=CCFadeOut::create(0.5f);
        CCCallFunc*  func2=CCCallFunc::create(this, callfunc_selector(FightPage::MoreAttack));
        CCFiniteTimeAction *jianxueaction1=CCSequence::create(set_big,set_nor,a_jiaxue_fadeout1,func2,NULL);
        reduB->runAction(jianxueaction1);
        
        zhenping(enemy);
    }
}

void FightPage::MoreAttack()
{
    t=0;
    
    enemy=(CCSprite *)this->getChildByTag(_eneBattId[moreene[ea]]);
    enemy->removeChildByTag(9023+moreene[ea],true);
    enemy->removeChildByTag(9800+moreene[ea],true);
    CCSprite * blood=(CCSprite *)enemy->getChildByTag(_eneBattId[moreene[ea]]+1000);
    
    if(_eneChanHp[moreene[ea]]+_eneCurrHp[moreene[ea]]<=0)
    {
        CCActionInterval *  scal=CCScaleTo::create(1/Time, 0, 1);
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(FightPage::removeAction));
        CCFiniteTimeAction *Action=CCSequence::create(scal,func,NULL);
        blood->runAction(Action);
    }
    else
    {
        int ech=_eneChanHp[moreene[ea]];
        int echp=_eneCurrHp[moreene[ea]];
        int eth=_eneTotHp[moreene[ea]];
        percent1=((double)(ech+echp)/(double)(eth));
        CCActionInterval *  scal=CCScaleTo::create(1/Time, percent1, 1);
        CCCallFunc * func=CCCallFunc::create(this,callfunc_selector(FightPage::removeAction));
        CCFiniteTimeAction *Action=CCSequence::create(scal,func,NULL);
        blood->runAction(Action);
    }
    
    ea++;
}

void FightPage::removeAction()
{
    if(_eneChanHp[t]+_eneCurrHp[t]<=0)
    {
        enemy=(CCSprite *)this->getChildByTag(_eneBattId[t]);
        enemy->setTag(_eneBattId[t]);
        
        const char * texiaoname=sk->skills(_skillId);
        char txname[50]="";
        sprintf(txname, "%s.xml",texiaoname);
        
        this->removeChildByTag(_eneBattId[t],true);
        _eneBattId[t]=0;
        if(t==_length-1)
        {
            this->SimpleAttack();
        }
    }
    else
    {
        enemy=(CCSprite *)this->getChildByTag(_eneBattId[t]);
        if(t==_length-1)
        {
            this->SimpleAttack();
        }
    }
    
    t++;
}

void FightPage::removeit()
{
    CCSprite * xuetiao=(CCSprite *)this->getChildByTag(1000+eneBID);
    enemy->removeFromParentAndCleanup(true);
    this->removeChild(xuetiao,true);
    this->checkOut();
}

void FightPage::zhenping(CCNode * sender)
{
    CCPoint size = sender->getPosition();
    CCActionInterval * left1 = CCMoveTo::create(0.03,ccp(sender->getPosition().x+5,sender->getPosition().y));
    CCActionInterval * right1 = CCMoveTo::create(0.03,ccp(sender->getPosition().x-5,sender->getPosition().y));
    CCActionInterval * top1 = CCMoveTo::create(0.03,ccp(sender->getPosition().x,sender->getPosition().y+5));
    CCActionInterval * rom1 = CCMoveTo::create(0.03,size);
    CCFiniteTimeAction* action3 = CCSequence::create(left1,right1,top1,rom1,NULL);
    sender->runAction(action3);
}

void FightPage::SimpleAttack()
{
    this->checkOut();
}

void FightPage::buffbegin(int bufftype)
{
    CCActionInterval * buffrote1=CCRotateBy::create(10, 360);
    CCActionInterval * buffrote2=CCRotateBy::create(15, 360);
    CCActionInterval * fadein1=CCFadeIn::create(1);
    CCActionInterval * fadeout1=CCFadeOut::create(1);
    CCFiniteTimeAction * final=CCSequence::create(fadein1,fadeout1,NULL);
    CCActionInterval * fadein2=CCFadeIn::create(1.5);
    CCActionInterval * fadeout2=CCFadeOut::create(1.5);
    CCFiniteTimeAction * final2=CCSequence::create(fadein2,fadeout2,NULL);
    CCActionInterval * fadein3=CCFadeIn::create(1.2);
    CCActionInterval * fadeout3=CCFadeOut::create(1.2);
    CCFiniteTimeAction * final3=CCSequence::create(fadein3,fadeout3,NULL);
    switch (bufftype)
    {
        case 1:
            buff1->setVisible(true);
            buff1->runAction(CCRepeatForever::create(buffrote1));
            buff1->runAction(CCRepeatForever::create((CCActionInterval *)final));
            break;
        case 2:
            buff2->setVisible(true);
            buff2->runAction(CCRepeatForever::create(buffrote2));
            buff2->runAction(CCRepeatForever::create((CCActionInterval *)final3));
            break;
        case 3:
            buff3->setVisible(true);
            buff3->runAction(CCRepeatForever::create((CCActionInterval *)final2));
            break;
        default:
            break;
    }
}

skill * FightPage::returnskills(int skillId)
{
    Json::Reader read;
    Json::Value root;
    Json::Value data;
    Json::Value row;
    char skillss[40]="";
    sprintf(skillss, "%d",skillId);
    skill * skills=new skill();
    skills->autorelease();
    
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("skill.json");
    CCString* str = CCString::createWithContentsOfFile(path.c_str());
    
    if(read.parse(str->getCString(), root))
    {
        data=root[skillss];
        
        skills->releaseEffect=data["releaseEffect"].asInt();
        skills->throwEffectId=data["throwEffectId"].asInt();
        skills->bearEffect=data["bearEffect"].asInt();
    }
    
    return skills;
}

