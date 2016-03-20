#include "middlePage.h"
#include "CustomPop.h"
#include "popwindow.h"

static const int ZODER_BACKGROUND = 100;

CCNode* MiddlePage::getNode(CCNode* no)
{
    extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("MiddlePage", MiddlePageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/MiddlePage.ccbi", no);
    ccbReader->release();
    
    return node;
}

MiddlePage::~MiddlePage()
{
    levelSpDic->release();
}

bool MiddlePage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void MiddlePage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    this->setTouchEnabled(true);
    
    levelSpDic = CCDictionary::create();
    levelSpDic->retain();
    
    currentMap = 0;
    
    size = CCDirector::sharedDirector()->getWinSize();
    
    mapSp = CCSprite::create();
    mapSp->setAnchorPoint(ccp(0, 0));
    mapSp->setPosition(ccp(0, 0));
    this->addChild(mapSp);
    
    mapLayer = CCSprite::create();
    mapLayer->setAnchorPoint(ccp(0, 0));
    mapLayer->setPosition(ccp(0, 0));
    mapSp->addChild(mapLayer);
    
    for(int i = 0; i < 5; i++)
    {
        CCSprite* m1 = CCSprite::create("map.png");
        m1->setAnchorPoint(ccp(0, 0));
        m1->setPosition(ccp(0, 441 * i));
        mapLayer->addChild(m1);
    }
    
    levelsMenu = CCMenu::create();
    levelsMenu->setAnchorPoint(ccp(0, 0));
    levelsMenu->setPosition(ccp(0, 0));
    this->addChild(levelsMenu);
    
    _spr_bg->setZOrder(ZODER_BACKGROUND);
    
    for (int i = 0; i < 9; i++)
    {
        CCMenuItemSprite* leveltmp = CCMenuItemImage::create("zjm_building_1.png", "zjm_building_1.png", "zjm_building_2.png",this, menu_selector(MiddlePage::clkBuilding));
        
        int num = floor(i/3);
        
        leveltmp->setPosition(ccp((i-3*num)%2==1?140:size.width-130, (130)*(i%3) + 442*(int)floor(i/3) +240));//160
        levelSpDic->setObject(leveltmp, i);
        levelsMenu->addChild(leveltmp);
        
        CCLabelTTF * name11=CCLabelTTF::create("", "hycc.ttf", 20);
        name11->setDimensions(CCSizeMake(200, 0));
        name11->setHorizontalAlignment(kCCTextAlignmentCenter);
        name11->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        name11->setPosition(ccp(leveltmp->getContentSize().width/2-1, 35));
        name11->setColor(ccc3(255,175,51));
        name11->setTag(1002);
        leveltmp->addChild(name11, 10);
    }
    
    char * json=CData::getCData()->cityData;
    Json::Reader read;
    Json::Value  root;
    if(read.parse(json, root))
    {
        data=root["data"];
    }
    currentLevelId = data["cityid"].asInt();
    
    if (CData::getCData()->levelMapOffsetY > 0)
    {
        offsetY = -((int)ceil((currentLevelId-1000)/3))*442;
    }
    else
    {
        offsetY = CData::getCData()->levelMapOffsetY;
    }
    
    mapNum = CData::getCData()->getLevelMapNum();
    adjustMap(true);
}

void MiddlePage::adjustMap(bool isResetLevel)
{
    //offsetY 是地图位置为(0, -442)时 为0， 当向下移动时，为负的移动距离
    //currentMap为从0开始计数的地图，最开始显示的为地图0，最大为 mapNum-1
    currentMap = -floor(offsetY/442);
    
    mapLayer->setPosition(ccp(0, -currentMap % 3 * 442 + currentMap * 442 + offsetY - 442));
    
    if(isResetLevel)
    {
        int offlevel = 1000 + currentMap * 3 -3;
        
        for (int i = 0; i < 9 ; i++)
        {
            MapItem* tmpLevelItemData = NULL;
            tmpLevelItemData = CData::getCData()->getConfigOfMapLevel(offlevel + i);
            
            CCMenuItemSprite* pBtn = (CCMenuItemSprite*)levelSpDic->objectForKey(i);
            
            CCNode* pNo = pBtn->getChildByTag(9999);
            if(pNo)
            {
                pBtn->removeChildByTag(9999);
            }
            
            if (tmpLevelItemData!= NULL && tmpLevelItemData->bid < currentLevelId)
            {
                CCSize tmpsz = pBtn->getContentSize();
                
                CCNode* ssp = CCNode::create();
                
                int num = data["citylist"][tmpLevelItemData->bid - 1000].asInt();
                ssp->setPosition(ccp(tmpsz.width/2-15*num+15, 0));
                ssp->setTag(9999);
                pBtn->addChild(ssp, 0);
                
                for (int k = 0; k<num; k++)
                {
                    CCSprite * star = CCSprite::create("zjm_star.png");
                    star->setPosition(ccp(k*30, 0));
                    ssp->addChild(star);
                }
            }
            
            if(tmpLevelItemData != NULL)
            {
                if(tmpLevelItemData->bid > currentLevelId)
                {
                    pBtn->setEnabled(false);
                }
                else
                {
                    pBtn->setEnabled(true);
                }
                
                pBtn->setUserObject(tmpLevelItemData);
                
                ((CCLabelTTF*)(pBtn->getChildByTag(1002)))->setString(tmpLevelItemData->nickname.c_str());
                
            }
            
        }
    }
    
    levelsMenu->setPosition(ccp(0, currentMap * 442 + offsetY - 442));
}

void MiddlePage::clkBuilding(CCMenuItemImage *building)
{
    MapItem *item = (MapItem*)building->getUserObject();
    if(item == NULL)
    {
        CustomPop::show("关卡信息为空！");
        return;
    }
    
    buildingpop* buildpop = buildingpop::create();
    this->getParent()->addChild(buildpop, 100);
    buildpop->show(item);
}

void MiddlePage::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);
    CCLayer::registerWithTouchDispatcher();
}

bool MiddlePage::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    
    startP =  convertTouchToNodeSpace(touch);
    isTouchMenu=levelsMenu->ccTouchBegan(touch, event);
    CCPoint touchPoint = convertTouchToNodeSpace(touch);
    
    if (touchPoint.y < 80 || touchPoint.y > size.height - 180)
    {
        return false;
    }
    
    
    return true;
}

void MiddlePage::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    
    movingP = convertTouchToNodeSpace(touch);
    distanceP = ccpSub(movingP, startP);
    CCPoint tmpP = mapLayer->getPosition();
    
    if(offsetY + distanceP.y > 0)
    {
        offsetY = 0;
        CData::getCData()->levelMapOffsetY = offsetY;
    }
    else if (offsetY + distanceP.y < -442*(mapNum-1))
    {
        offsetY = -442*(mapNum-1);
        CData::getCData()->levelMapOffsetY = offsetY;
    }
    else
    {
        offsetY += distanceP.y;
        CData::getCData()->levelMapOffsetY = offsetY;
    }
    adjustMap(true);
    startP = movingP;
    
    if(isTouchMenu)
    {
        levelsMenu->ccTouchMoved(touch, event);
    }
    
}

void MiddlePage::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    
    if (isTouchMenu)
    {
        levelsMenu->ccTouchEnded(touch, event);
        
        isTouchMenu=false;
    }
    
    adjustMap(true);
    
}




