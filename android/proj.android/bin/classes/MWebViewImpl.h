//
//  MWebViewImpl.h
//  legend
//
//  Created by xiaofuan on 14-3-5.
//
//

#ifndef __legend__MWebViewImpl__
#define __legend__MWebViewImpl__

#include <iostream>
#include "MWebView.h"
class MWebView;
class MWebViewImpl
{
public:
    MWebViewImpl(MWebView* pWebView) : m_pWebView(pWebView) {}
    virtual ~MWebViewImpl() {}
    virtual void showWebView(const std::string url) = 0;
    virtual void openURL(std::string url) = 0;
    virtual void removeWebView() = 0;
    virtual void setPosition(const CCPoint& pos) = 0;
    virtual void setVisible(bool visible) = 0;
    virtual void setContentSize(const CCSize& size) = 0;
    virtual bool initWithSize(const CCSize& size) = 0;
    virtual void visit() = 0;
protected:
    MWebView* m_pWebView;
};
// This method must be implemented at each subclass of MWebViewImpl.
extern MWebViewImpl* __createSystemWebView(MWebView* pWebView);
#endif /* defined(__legend__MWebViewImpl__) */
