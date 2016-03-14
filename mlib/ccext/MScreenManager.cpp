#include "MScreenManager.h"
#include "mlib_assert.h"
#include "mlib_log.h"
#include "mlib_ccext.h"
#include "LAudio.h"
using namespace cocos2d;
using namespace cocos2d::extension;

MLIB_NS_BEGIN

static CCArray * g_popups = nullptr;

#pragma mark MPopupContainer


MPopupContainer * MPopupContainer::createContainer(cocos2d::CCNode *node, bool isModal, cocos2d::ccColor4B bgColor)
{
    MPopupContainer * ret = new MPopupContainer();
    
    if (ret && ret->initWithColor(isModal ? bgColor : ccc4(0, 0, 0, 0)))
    {
        ret->autorelease();
        ret->isModal = isModal;
        ret->node = node;
        ret->isTemp = false;
        
        ret->ignoreAnchorPointForPosition(false);
        
        auto layer = CCLayer::create();
        layer->setContentSize(CCDirector::sharedDirector()->getWinSize());
        layer->setPosition(ccp(0, 0));
        layer->addChild(node);
        
        ret->addChild(layer);
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

bool MPopupContainer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    M_ASSERT(pTouch != nullptr && node != nullptr);
    
    CCPoint touchLocation = pTouch->getLocation();
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation); // Convert to the node space of this class

    CCRect boundingBox = node->boundingBox();
    bool inside =
    (touchLocation.x >= boundingBox.origin.x && touchLocation.x < boundingBox.origin.x + boundingBox.size.width)
    &&
    (touchLocation.y >= boundingBox.origin.y && touchLocation.y < boundingBox.origin.y + boundingBox.size.height)
    ;
    
    if (isTemp && !inside)
    {
        M_DEBUG("touches outside of popup. as popup is temp, will close it!");
        
        MScreenManager::closePopup(node);
        return isModal;
    }
    
    if (inside || isModal)
    {
//        M_DEBUG("touches will be swallowed by popup bg: " << this->description() << "!");
        
        return true;
    }
    else
    {
//        M_DEBUG("belongs to popup, continue ");
        return false;
    }
}

void MPopupContainer::onEnter()
{
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    CCLayerColor::onEnter();
}

void MPopupContainer::onExit()
{
    CCLayerColor::onExit();
}

class MScreenManagerDummy : public CCObject
{
public:
    static MScreenManagerDummy * sharedInstance()
    {
        static MScreenManagerDummy instance;
        return &instance;
    }
    
    void closePopup(CCNode *node)
    {
        MScreenManager::closePopup(node);
    }
};

#pragma mark MScreenManager

void MScreenManager::popup(cocos2d::CCNode *node,
                           bool isModal,
                           bool isTemp,
                           float showTime,
                           cocos2d::ccColor4B bgColor,
                           bool shouldNotRemovedByCloseAll,
                           AudioEffect_t effectType
                           )
{
    M_ASSERT(node != nullptr, "expect a non-null node");
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = ccp(size.width / 2.f, size.height / 2.f);
    pos.x -= node->getContentSize().width / 2.f;
    pos.y -= node->getContentSize().height / 2.f;
    
    if (!node->isIgnoreAnchorPointForPosition())
    {
        CCPoint pt = node->getAnchorPointInPoints();
        pos.x += pt.x;
        pos.y += pt.y;
    }
    
    MScreenManager::popup(node, pos, isModal, isTemp, showTime, bgColor, shouldNotRemovedByCloseAll, false, effectType);
}

void MScreenManager::popupWithAnimation(cocos2d::CCNode *node,
                                        bool isModal,
                                        bool isTemp,
                                        float showTime,
                                        cocos2d::ccColor4B bgColor,
                                        AudioEffect_t effectType
                                        )
{
    M_ASSERT(node != nullptr, "expect a non-null node");
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = ccp(size.width / 2.f, size.height / 2.f);
    pos.x -= node->getContentSize().width / 2.f;
    pos.y -= node->getContentSize().height / 2.f;
    
    if (!node->isIgnoreAnchorPointForPosition())
    {
        CCPoint pt = node->getAnchorPointInPoints();
        pos.x += pt.x;
        pos.y += pt.y;
    }
    
    MScreenManager::popup(node, pos, isModal, isTemp, showTime, bgColor, false, true, effectType);
}

void MScreenManager::popupWithBgColor(cocos2d::CCNode *node, cocos2d::ccColor4B bgColor, bool shouldNotRemovedByCloseAll)
{
    popup(node, true, false, 0, bgColor, shouldNotRemovedByCloseAll, kEffectNoPlay);
}

void MScreenManager::popup(cocos2d::CCNode *node,
                           cocos2d::CCPoint position,
                           bool isModal,
                           bool isTemp,
                           float showTime,
                           cocos2d::ccColor4B bgColor,
                           bool shouldNotRemovedByCloseAll,
                           bool isAnimated,
                           AudioEffect_t effectType
                           )
{
    M_ASSERT(node != nullptr, "expect a non-null node");
    
    node->setPosition(position);
    
    CCScene * runningScene = CCDirector::sharedDirector()->getCommingScene();
    
    MPopupContainer * bg = MPopupContainer::createContainer(node, isModal, bgColor);
    bg->isTemp = isTemp;
    bg->shouldNotRemovedByCloseAll = shouldNotRemovedByCloseAll;
    
    if (g_popups == nullptr)
    {
        g_popups = CCArray::create();
        g_popups->retain();
    }
    MPopupContainer * top = (MPopupContainer *)g_popups->lastObject();
    if (top)
    {
        bg->priority = top->priority - 2;
    }
    else
    {
        bg->priority = TOUCH_PRIORITY_POPUP_BASE;
    }
    
    g_popups->addObject(bg);
    
    M_DEBUG("popup priority: " << bg->priority);
    bg->setTouchPriority(bg->priority);
    
    CCSize cSize = runningScene->getContentSize();
    bg->setContentSize(cSize);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, 0));
    if (effectType != kEffectNoPlay)
    {
        LAudio::playEffect(effectType);
    }
    runningScene->addChild(bg);
    
    MScreenManager::setTouchPriorityForAllChildren(node, bg->priority - 1);
    
    if (isAnimated)
    {
        node->getParent()->setScale(0);
        node->getParent()->runAction(CCSequence::create(CCEaseElasticOut::create(CCScaleTo::create(0.5, 1), 1), MCallbackAction::create([bg]()
        {
            bg->dispatchEvent(EVENT_POPUP_ANIMATION_END);
        }), nullptr));
    }
    
    if (showTime != 0)
    {
        CCCallFuncN *func = CCCallFuncN::create(MScreenManagerDummy::sharedInstance(), callfuncN_selector(MScreenManagerDummy::closePopup));
        CCSequence *seq = CCSequence::create(CCDelayTime::create(showTime), func, nullptr);
        node->runAction(seq);
    }
    
//    M_DEBUG("popup: " << node->description() << "  curSize: " << g_popups->count());
}

void MScreenManager::closePopup(cocos2d::CCNode *node)
{
    M_ASSERT(node != nullptr, "expect a non-null node");
    M_ASSERT(g_popups != nullptr, "should popped up before");
    
    node->retain();
    node->autorelease();
    
    auto popup = MPopupContainer::getPopupContainer(node);
    if (g_popups->containsObject(popup))
    {
        popup->dispatchEvent(EVENT_POPUP_CLOSED);
        popup->removeFromParent();
        g_popups->removeObject(popup);
    }
    else
    {
        M_WARNING("popup is not found!" << node->description());
    }
}

void MScreenManager::closeAllPopups()
{
    if (g_popups == nullptr)
    {
        return;
    }
    
    MPopupContainer * node = nullptr;
    CCObject * obj = nullptr;
    CCArray *arr = CCArray::create();
    CCARRAY_FOREACH(g_popups, obj)
    {
        node = (MPopupContainer *)obj;
        if (!node->shouldNotRemovedByCloseAll)
        {
            node->dispatchEvent(EVENT_POPUP_CLOSED);
            node->removeFromParent();
        }
        else
        {
            arr->addObject(node);
        }
    }
    
    g_popups->release();
    g_popups = arr;
    g_popups->retain();
}

bool MScreenManager::isAllPopupsClosed()
{
    bool result = false;
    if (nullptr == g_popups)
    {
        result = true;
    }
    else
    {
        result = g_popups->count() > 0 ? false : true;
    }
    return result;
}

void MScreenManager::closeLastPopup()
{
    MPopupContainer *node = (MPopupContainer *)g_popups->lastObject();
    
    if (node != nullptr)
    {
        if (!node->shouldNotRemovedByCloseAll)
        {
            node->dispatchEvent(EVENT_POPUP_CLOSED);
            node->removeFromParent();
            g_popups->removeLastObject();
        }
        
    }
}

void MScreenManager::setTouchPriorityForAllChildren(cocos2d::CCNode *node, int priority)
{
    M_ASSERT(node != nullptr, "expect a non-null node");
    
    CCControl * pControl = nullptr;
    pControl = dynamic_cast<CCControl *>(node);
    if (pControl)
    {
        pControl->setTouchPriority(priority);
    }
    else
    {
        CCLayer * pLayer = nullptr;
        pLayer = dynamic_cast<CCLayer *>(node);
        if (pLayer)
        {
            pLayer->setTouchPriority(priority);
        }
    }
    
    CCObject * obj = nullptr;
    CCNode * child = nullptr;
    CCARRAY_FOREACH(node->getChildren(), obj)
    {
        child = (CCNode *)obj;
        MScreenManager::setTouchPriorityForAllChildren(child, priority);
    }
}

void MScreenManager::refreshPriorityForPopup(cocos2d::CCNode *node)
{
    MPopupContainer *bg = MPopupContainer::getPopupContainer(node);
    if (bg)
    {
        setTouchPriorityForAllChildren(node, bg->priority - 1);
    }
}

MPopupContainer * MPopupContainer::getPopupContainer(cocos2d::CCNode *node)
{
    MPopupContainer *bg = (MPopupContainer *)node->getParent()->getParent();
    return bg;
}

MLIB_NS_END
