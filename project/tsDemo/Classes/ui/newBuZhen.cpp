#include "newBuZhen.h"
#include "LayerLogin.h"
#include "message.h"
#include "SocketManager.h"
#include "CustomPop.h"

bool newBuZhen::init()
{
    if(!CCLayerColor::init())
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    
    draging = false;
    readdraging = false;
    acceptindex = -1;
    lock = false;
    
    dragingsp = heroItem::create();
    this->addChild(dragingsp,1000);
    dragingsp->setVisible(false);
    
    size = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(size);
    
    CCSprite *bkgBz=CCSprite::create("zjm_bkg.png");
    bkgBz->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(bkgBz);
    CCSprite* bj = CCSprite::create("bz_bkg.png");
    bj->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(bj);
    
    dic = CCDictionary::create();
    dic->retain();
    
    CCSize sz;
    int kk;
    
    for (int i = 0; i < 19; i++)
    {
        heroItem * item = heroItem::create();
        sz = item->getContentSize();
        kk = sz.width-30;
        
        dic->setObject(item, i+1);
        
        item->setAnchorPoint(ccp(0, 1));
        if(i<9)
        {
            item->setPosition(ccp(82+(i%3)*115,  -115 * (i/3) + 640));
        }
        else
        {
            item->setPosition(ccp(((size.width - kk*5)/8 + kk) * ((i-9)%5 + 1) -kk+13, -75 * ((i-9)/5) + 293));
        }
        
        this->addChild(item);
    }
    
    send_2306();
    
    return true;
}

void newBuZhen::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min()), true);
    CCLayer::registerWithTouchDispatcher();
}

bool newBuZhen::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(lock)
    {
        return false;
    }
    
    CCPoint touchPoint=convertTouchToNodeSpace(touch);
    
    if (touchPoint.y < 80 || touchPoint.y > size.height - 180)
    {
        return false;
    }
    
    heroItem* item;
    int len = dic->count();
    
    for (int i = 1; i<=len; i++)
    {
        item =(heroItem*) dic->objectForKey(i);
        if(item && item->boundingBox().containsPoint(touchPoint))
        {
            if(draging)
            {
                CustomPop::show("错误！");
            }
            else
            {
                if(item->itemdata)
                {
                    readdraging = true;
                    readydragingsp = item;
                    dragingindex = i;
                }
            }
            
            break;
        }
    }

    return true;
}

void newBuZhen::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCPoint touchPoint=convertTouchToNodeSpace(touch);
    
    if (readdraging)
    {
        draging = true;
        dragingsp->setData(readydragingsp->itemdata);
        dragingsp->setVisible(true);
        dragingsp->setPosition(touchPoint);
        readdraging = false;
    }
    
    if(draging)
    {
        dragingsp->setPosition(touchPoint);
    }
}

void newBuZhen::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCPoint touchPoint=convertTouchToNodeSpace(touch);
    
    readdraging = false;
    
    if(draging)
    {
        draging = false;
        heroItem* item;
        int len = dic->count();
        
        for (int i = 1; i<=len; i++)
        {
            item =(heroItem*) dic->objectForKey(i);
            if(item && item->boundingBox().containsPoint(touchPoint))
            {
                acceptsp = item;
                acceptindex = i;
                break;
            }
        }
        
        if(acceptsp && acceptindex!= dragingindex)
        {
            if (acceptsp->itemdata)
            {
                if(acceptindex <= 9)
                {
                    if (dragingindex<= 9)
                    {
                        //英雄互换布阵位置
                        send_2307(dragingsp->itemdata->petId, dragingsp->itemdata->tpid, 2, dragingindex, acceptindex);
                    }
                    else
                    {
                        //英雄上阵替换原来位置武将
                         send_2307(dragingsp->itemdata->petId, dragingsp->itemdata->tpid, 0,0, acceptindex);
                    }
                }
                else
                {
                    if (dragingindex > 9)
                    {
                        //吞噬
                        if (dragingsp->itemdata->tpid == 2 || acceptsp->itemdata->tpid == 2)
                        {
                            CustomPop::show("好友不能吞噬！");
                        }
                        else{
                            send_3505();
                        }
                    }
                    else
                    {
                        CustomPop::show("上阵的英雄不能被吞噬！");
                    }
                }
            }
            else
            {
                if(acceptindex > 9 )
                {
                    if(dragingindex <= 9)
                    {
                        //英雄撤销布阵
                        if (dragingsp->itemdata->tpid == 1)
                        {
                            CustomPop::show("主角必须上阵");
                        }
                        else
                        {
                            send_2307(dragingsp->itemdata->petId, dragingsp->itemdata->tpid, 1, dragingindex, 0);
                        }
                    }
                }
                else
                {
                    if (dragingindex > 9)
                    {
                        //布阵
                         send_2307(dragingsp->itemdata->petId, dragingsp->itemdata->tpid, 0, 0, acceptindex);
                    }
                    else
                    {
                        //英雄换布阵位置
                         send_2307(dragingsp->itemdata->petId, dragingsp->itemdata->tpid, 2, dragingindex, acceptindex);
                        
                    }
                }
            }
        }
        else
        {
            
        }
        
        draging = false;
        dragingsp->setVisible(false);
        
        acceptsp = NULL;
    }
}

void newBuZhen::send_3505()
{
    lock = true;

    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    sendMessage["petid"] = acceptsp->itemdata->petId;
    sendMessage["tpetid"] = dragingsp->itemdata->petId;
    string json_file_jianghu=writer.write(sendMessage);

    SocketManager::getInstance()->sendMessage(json_file_jianghu.c_str(), 3505);
    this->schedule(schedule_selector(newBuZhen::rec_3505));
}

void newBuZhen::rec_3505()
{
    Message* revMsg = (Message*)CData::getCData()->m_dictionary->objectForKey(3505);
    CData::getCData()->m_dictionary->removeObjectForKey(3505);
    if(revMsg)
    {
        this->unschedule(schedule_selector(newBuZhen::rec_3505));
        char * ditu=revMsg->data;
        
        //解析数据
        Json::Reader read;
        Json::Value value;
        if(read.parse(ditu, value))
        {
            bool  ischenggong = value["result"].asBool();
            if(ischenggong == true)
            {
                send_2306();
            }
            else
            {
                lock = false;
                string out = value["message"].asCString();
                CustomPop::show(out.c_str());
            }
        }
    }
}

void newBuZhen::send_2307(int petid,int chatype,int opertype,int frompos,int topos)
{
    lock = true;
       
    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    sendMessage["petId"] = petid;
    sendMessage["chatype"] = chatype;
    sendMessage["operType"] = opertype;
    sendMessage["fromPos"] = frompos;
    sendMessage["toPos"] = topos;
    
    string json_file_jianghu=writer.write(sendMessage);
    
    SocketManager::getInstance()->sendMessage(json_file_jianghu.c_str(), 2307);
    this->schedule(schedule_selector(newBuZhen::rec_2307));
}

void newBuZhen::rec_2307()
{
    Message* revMsg = (Message*)CData::getCData()->m_dictionary->objectForKey(2307);
    CData::getCData()->m_dictionary->removeObjectForKey(2307);
    if(revMsg)
    {
        this->unschedule(schedule_selector(newBuZhen::rec_2307));
        char * ditu=revMsg->data;
        
        //解析数据
        Json::Reader read;
        Json::Value value;
        if(read.parse(ditu, value))
        {
            bool  ischenggong = value["result"].asBool();
            if(ischenggong == true)
            {
                send_2306();
            }
            else
            {
                string out = value["message"].asCString();
                CustomPop::show(out.c_str());

                lock = false;
            }
        }
    }
}

void newBuZhen::send_2300()
{
    lock = true;
    
    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    
    string json_file_jianghu=writer.write(sendMessage);

    SocketManager::getInstance()->sendMessage(json_file_jianghu.c_str(), 2300);
    this->schedule(schedule_selector(newBuZhen::rec_2300));
}

void newBuZhen::rec_2300()
{
    Message *revMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(2300);
    CData::getCData()->m_dictionary->removeObjectForKey(2300);

    if(revMsg)
    {
        this->unschedule(schedule_selector(newBuZhen::rec_2300));
        const char * data=revMsg->data;
        CCLog("%s\n",data);
        Json::Reader read;
        Json::Value root;
        Json::Value jsonData;
        Json::Value petlist;
        
        if(read.parse(data, root))
        {
            jsonData=root["data"];
            petlist=jsonData["petlist"];
            for (int i=0; i<petlist.size(); i++)
            {
                heroItemData* tmpdata = new heroItemData();
                
                tmpdata->petId =petlist[i]["chaid"].asInt();
                tmpdata->tpid = petlist[i]["chatype"].asInt();
                tmpdata->tempid = petlist[i]["tempid"].asInt();
              
                ((heroItem*)dic->objectForKey(i+10))->setData(tmpdata);
            }
            
            lock = false;
        }
    }
}

void newBuZhen::send_2306()
{
    lock = true;
    
    heroItem * tmp;
    for (int i = 1; i < 20; i++)
    {
        tmp =(heroItem*) dic->objectForKey(i);
        tmp->clData();
    }
    
    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    
    string json_file_jianghu=writer.write(sendMessage);

    SocketManager::getInstance()->sendMessage(json_file_jianghu.c_str(), 2306);
    this->schedule(schedule_selector(newBuZhen::rec_2306));
}

void newBuZhen::rec_2306()
{
    Message *revMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(2306);
    CData::getCData()->m_dictionary->removeObjectForKey(2306);
    
    if(revMsg)
    {
        this->unschedule(schedule_selector(newBuZhen::rec_2306));
        const char * data=revMsg->data;
        CCLog("%s",data);
        Json::Reader reader;
        Json::Value root;
        Json::Value jsonData;
        Json::Value matrix;
        
        bool resutl;
        
        if(reader.parse(data, root))
        {
            resutl=root["result"].asBool();
            jsonData=root["data"];
            matrix=jsonData["matrix"];

            for(int i=0;i<matrix.size();i++)
            {
                int tmpchatid =matrix[i]["chaid"].asInt();
                
                if (tmpchatid != -1)
                {
                    heroItemData * tmpdata = new heroItemData();
                    tmpdata->petId  =tmpchatid;
                    tmpdata->pos =matrix[i]["pos"].asInt();
                    tmpdata->tpid =matrix[i]["chatype"].asInt();
                    tmpdata->tempid = matrix[i]["tempid"].asInt();
                    ((heroItem*)dic->objectForKey(tmpdata->pos))->setData(tmpdata);
                }
            }
        }
        
        send_2300();
    }
}

void newBuZhen::refresh_2300()
{
    
}

void newBuZhen::refresh_2306()
{
    
}

void newBuZhen::onExit()
{
    CCLog("newBuZhen onExit");
    CCLayerColor::onExit();
}

newBuZhen::~newBuZhen()
{
    dic->release();
    CCLog("newBuZhen destroy");
}






