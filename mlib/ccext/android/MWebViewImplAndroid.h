//
//  MWebViewImplIOS.h
//  legend
//
//  Created by xiaofuan on 14-3-5.
//
//

#ifndef __legend__MWebViewImplAndroid__
#define __legend__MWebViewImplAndroid__

#include <iostream>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MWebViewImpl.h"

class MWebViewImplAndroid : public MWebViewImpl
{
public:
    MWebViewImplAndroid(MWebView* pWebView);
    virtual ~MWebViewImplAndroid();
    virtual void showWebView(const std::string url);
    virtual void openURL(std::string url);
    virtual void removeWebView();
    virtual void setPosition(const CCPoint& pos);
    virtual void setVisible(bool visible);
    virtual void setContentSize(const CCSize& size);
    virtual bool initWithSize(const CCSize& size);
    virtual void visit();
    void setWebViewFrame();
private:
    CCSize          m_tContentSize;
    CCPoint         m_obPosition;
    float m_Scale;
};

#endif
#endif /* defined(__legend__MWebViewImplAndroid__) */
