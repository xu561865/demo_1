//
//  MWebView.h
//  legend
//
//  Created by xiaofuan on 14-2-28.
//
//

#ifndef __legend__MWebView__
#define __legend__MWebView__

#include <iostream>
#include "mlib_ccext.h"


class MWebViewImpl;

class MWebView : public CCNode
{
public:
    MWebView();
    ~MWebView();
    CREATE_FUNC(MWebView);
    virtual bool init();
    void showWebView(const std::string url);
    
    void openURL(const std::string url);
    
    void removeWebView();
    virtual void visit(void);
    //virtual void setContentSize(const CCSize& contentSize);
    virtual void initWithSize(CCSize webSize);
private:
    MWebViewImpl *webViewImpl;
};
#endif /* defined(__legend__MWebView__) */
