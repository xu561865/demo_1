//
//  MWebViewImplIOS.h
//  legend
//
//  Created by xiaofuan on 14-3-5.
//
//

#ifndef __legend__MWebViewImplIOS__
#define __legend__MWebViewImplIOS__

#include <iostream>
#include "MWebViewImpl.h"
#import <Foundation/Foundation.h>
@interface WebView_IOS : NSObject <UIWebViewDelegate>
{
    UIWebView* m_webview;
}

- (void)createWebView;

- (void)openURL:(const char*)url;

- (void)removeWebView;
- (void)setFrame:(CGRect) frame;

@end

class MWebViewImplIOS : public MWebViewImpl
{
public:
    MWebViewImplIOS(MWebView* pWebView);
    virtual ~MWebViewImplIOS();
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
    WebView_IOS *m_webViewIOS;
    CCSize          m_tContentSize;
    CCPoint         m_obPosition;
    float m_Scale;
};


#endif /* defined(__legend__MWebViewImplIOS__) */
