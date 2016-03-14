#ifndef mlib_MScreenManager_h
#define mlib_MScreenManager_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "mlib_helpers.h"
#include "mlib_event.h"
#include "LConstants.h"

#define TOUCH_PRIORITY_ALERTVIEW_HIGHEST -200
#define TOUCH_PRIORITY_DISABLE_FULLSCREEN -150
#define TOUCH_PRIORITY_LOADING -127
#define TOUCH_PRIORITY_DIALOG -100
#define TOUCH_PRIORITY_POPUP_BASE -10
#define TOUCH_PRIORITY_UI 0 // for normal CCControl
#define TOUCH_PRIORITY_STAGE 10 // for game stages

MLIB_DEFINE_EVENT(EVENT_POPUP_ANIMATION_END)
MLIB_DEFINE_EVENT(EVENT_POPUP_CLOSED)

using namespace cocos2d;
using namespace cocos2d::extension;

MLIB_NS_BEGIN


class MScreenManager
{
public:
    
    static void popup(cocos2d::CCNode * node, bool isModal = true, bool isTemp = false, float showTime = 0, cocos2d::ccColor4B bgColor = cocos2d::ccc4(0, 0, 0, 128), bool shouldNotRemovedByCloseAll = false, AudioEffect_t effectType = kEffectNoPlay);
    static void popup(cocos2d::CCNode * node,
                      cocos2d::CCPoint position,
                      bool isModal = true,
                      bool isTemp = false,
                      float showTime = 0,
                      cocos2d::ccColor4B bgColor = cocos2d::ccc4(0, 0, 0, 128),
                      bool shouldNotRemovedByCloseAll = false,
                      bool isAnimated = false,
                      AudioEffect_t effectType = kEffectNoPlay
                      );
    static void popupWithBgColor(cocos2d::CCNode *node, cocos2d::ccColor4B bgColor, bool shouldNotRemovedByCloseAll = false);
    static void popupWithAnimation(cocos2d::CCNode * node, bool isModal = true, bool isTemp = false, float showTime = 0, cocos2d::ccColor4B bgColor = cocos2d::ccc4(0, 0, 0, 128), AudioEffect_t effectType = kEffectNoPlay);
    static void closePopup(cocos2d::CCNode * node);
    static void closeAllPopups();
    static void closeLastPopup();
  
    static void setTouchPriorityForAllChildren(cocos2d::CCNode * node,
                                               int priority
                                               );
    static void refreshPriorityForPopup(cocos2d::CCNode *node);
    static bool isAllPopupsClosed();
};

class MPopupContainer : public CCLayerColor, public mlib::MEventDispatcher
{
public:
    static MPopupContainer * createContainer(CCNode * node, bool isModal, cocos2d::ccColor4B bgColor);
    
    bool isModal;
    bool isTemp;
    int priority;
    CCNode * node;
    bool shouldNotRemovedByCloseAll;
    static MPopupContainer * getPopupContainer(cocos2d::CCNode *node);
    
public:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};
MLIB_NS_END

#endif // mlib_MScreenManager_h

