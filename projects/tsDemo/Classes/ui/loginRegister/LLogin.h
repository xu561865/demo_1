//
//  LLogin.h
//  legend
//
//  Created by xuyi on 3/3/16.
//
//

#ifndef LLogin_h
#define LLogin_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "mlib_ccext.h"

USING_NS_CC;

class LLogin :
public CCLayer,
public extension::CCBMemberVariableAssigner,
public extension::CCBSelectorResolver,
public extension::CCNodeLoaderListener
{
public:
    CREATE_FUNC(LLogin);
    MLIB_CREATE_WITH_CCB(LLogin, "Login.ccbi");
    LLogin();
    virtual ~LLogin();
    
private:
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) override;
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) override
    {
        return nullptr;
    }
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) override;
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader) override;
    
public:
    
    void reloadData();
    
private:
    void menuItemCallbackLogin(CCObject * sender, cocos2d::extension::CCControlEvent * evt);
    void menuItemCallbackStart(CCObject * sender, cocos2d::extension::CCControlEvent * evt);
    
private:
    MLIB_DECLARE_WEAK_PROPERTY(extension::CCControlButton *, btn_login);
    MLIB_DECLARE_WEAK_PROPERTY(extension::CCControlButton *, btn_start);
    MLIB_DECLARE_WEAK_PROPERTY(extension::CCEditBox *, editBoxUsername);
    MLIB_DECLARE_WEAK_PROPERTY(extension::CCEditBox *, editBoxPassword);
    MLIB_DECLARE_WEAK_PROPERTY(CCLabelTTF *, lblError);
    MLIB_DECLARE_WEAK_PROPERTY(CCSprite *, logo);

};

MLIB_DECLARE_CCBLOADER(LLogin, CCLayer)

#endif /* LLogin_h */
