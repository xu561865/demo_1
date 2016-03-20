//
//  MWebView.m
//  legend
//
//  Created by xiaofuan on 14-2-28.
//
//
#include "MWebView.h"
#include "MUtils.h"
#include "MWebViewImpl.h"

MWebView::MWebView()
{
    webViewImpl = nullptr;
}

MWebView::~MWebView()
{
    CC_SAFE_DELETE(webViewImpl);
}

void MWebView::showWebView(const std::string url)
{
    webViewImpl->showWebView(url);
}

void MWebView::removeWebView()
{
    webViewImpl->removeWebView();
}

void MWebView::visit()
{
    CCNode::visit();
    if (webViewImpl)
    {
        webViewImpl->visit();
    }
}

void MWebView::initWithSize(CCSize webSize)
{
    this->setContentSize(webSize);
    webViewImpl->setContentSize(webSize);
}

bool MWebView::init()
{
    webViewImpl = __createSystemWebView(this);
    if (webViewImpl)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}