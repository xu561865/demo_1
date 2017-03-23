#include "startAnimate.h"
#include "xuanren.h"

SEL_MenuHandler StartAnimate::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_skip", StartAnimate::callbackSelectRole);
    
    return NULL;
}

SEL_CCControlHandler StartAnimate::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool StartAnimate::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str1", CCSprite*, this->str1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str2", CCSprite*, this->str2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str3", CCSprite*, this->str3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str4", CCSprite*, this->str4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str5", CCSprite*, this->str5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str6", CCSprite*, this->str6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str7", CCSprite*, this->str7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "str8", CCSprite*, this->str8);
    
    return false;
}

bool StartAnimate::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    return true;
}

void StartAnimate::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    str2->setVisible(false);
    str3->setVisible(false);
    str4->setVisible(false);
    str5->setVisible(false);
    str6->setVisible(false);
    str7->setVisible(false);
    str8->setVisible(false);
    
    CCActionInterval *fadein=CCFadeIn::create(1.5);
    CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(StartAnimate::animateDesc));
    CCFiniteTimeAction * final=CCSequence::create(fadein,func,NULL);
    
    str1->setVisible(true);
    str1->runAction(final);
}

StartAnimate::~StartAnimate()
{

}

bool StartAnimate::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    round=0;
    
    return true;
}

void StartAnimate::kong()
{
    str1->setVisible(true);
}

void StartAnimate::animateDesc()
{
    if(round==0)
    {   
        str2->setVisible(true);
        CCActionInterval *fin=CCFadeIn::create(2);
        str2->runAction(fin);
        CCActionInterval *fadeout=CCFadeOut::create(0.7);
        CCActionInterval *delay=CCDelayTime::create(0.7);
        CCActionInterval *moveTo=CCMoveTo::create(0.7, ccp(str2->getPosition().x, str2->getPosition().y));
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(StartAnimate::animateDesc));
        CCFiniteTimeAction * final=CCSequence::create(delay,moveTo,fadeout,func,NULL);
        str2->runAction(final);
    }
    else if(round==1)
    {
        CCActionInterval * fadeout=CCFadeOut::create(0.1);
        str3->setVisible(true);
        CCActionInterval * delay=CCDelayTime::create(1.5);
        CCActionInterval * moveto=CCMoveTo::create(0.7, ccp(str3->getPosition().x, str3->getPosition().y));
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(StartAnimate::animateTwo));
        CCFiniteTimeAction * final=CCSequence::create(delay,moveto,fadeout,func,NULL);
        str3->runAction(final);
    }
    
    round++;
}

void StartAnimate::animateTwo()
{
    CCFadeOut *out=CCFadeOut::create(1.0);
    str1->runAction(out);
    str4->setVisible(true);
    CCActionInterval *fadein=CCFadeIn::create(1);
    str4->runAction(fadein);
    
    CCActionInterval *fadeout=CCFadeOut::create(1.5);
    CCActionInterval *delay=CCDelayTime::create(1.5);
    str7->setVisible(true);
    CCActionInterval *moveTo=CCMoveTo::create(0.7, ccp(str7->getPosition().x, str7->getPosition().y));
    CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(StartAnimate::animateThree));
    CCFiniteTimeAction * final=CCSequence::create(delay,moveTo,fadeout,func,NULL);
    str7->runAction(final);
}

void StartAnimate::animateThree()
{
    CCFadeOut *out=CCFadeOut::create(1.0);
    str4->runAction(out);
    str5->setVisible(true);
    CCFadeIn *fadein=CCFadeIn::create(2.5);
    str5->runAction(fadein);
    str8->setVisible(true);
    CCFadeIn *in=CCFadeIn::create(1.0);
    str8->runAction(in);
    CCActionInterval * fadeout=CCFadeOut::create(2);
    CCActionInterval * delay=CCDelayTime::create(1.5);

    CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(StartAnimate::animateFour));
    CCFiniteTimeAction * final=CCSequence::create(delay,fadeout,func,NULL);
    str8->runAction(final);
}

void StartAnimate::animateFour()
{
    CCFadeOut *out=CCFadeOut::create(2.0);
    str5->runAction(out);
    str6->setVisible(true);
    CCFadeIn *fadein=CCFadeIn::create(1.0);
    str6->runAction(fadein);
    CCFadeOut *fadeout=CCFadeOut::create(2.5);
    CCActionInterval *delay=CCDelayTime::create(2);
    CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(StartAnimate::callbackSelectRole));
    CCFiniteTimeAction *final=CCSequence::create(delay,fadeout,func,NULL);
    str6->runAction(final);
}

void StartAnimate::callbackSelectRole(CCObject *pSender)
{
    CCDirector::sharedDirector()->replaceScene(SelectRole::getScene());
}

CCScene* StartAnimate::getScene()
{
    CCScene *pScene = CCScene::create();
    CCScene* pp = CCTransitionFlipY::create(2, pScene, kCCTransitionOrientationDownOver);
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("StartAnimate", StartAnimateLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/StartAnimate.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pp;
}
