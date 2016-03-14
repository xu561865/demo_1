//
//  MWebViewImplIOS.cpp
//  legend
//
//  Created by xiaofuan on 14-3-5.
//
//

#include "MWebViewImplIOS.h"
#import <Foundation/Foundation.h>
#include "MUtils.h"
#include "EAGLView.h"
#include "mlib_ccext.h"

@implementation WebView_IOS

- (void)createWebView
{
    if (!m_webview)
    {
        m_webview = [[UIWebView alloc] init];
        [m_webview setDelegate:self];
        [[m_webview scrollView] setUserInteractionEnabled: YES];
        [[EAGLView sharedEGLView] addSubview:m_webview];
        [m_webview release];
        m_webview.backgroundColor = [UIColor clearColor];
        m_webview.opaque = NO;
        
        for (UIView *aView in [m_webview subviews])
        {
            if ([aView isKindOfClass:[UIScrollView class]])
            {
                UIScrollView* scView = (UIScrollView *)aView;
                
                //[scView setShowsVerticalScrollIndicator:YES];
                
                for (UIView *shadowView in scView.subviews)
                {
                    if ([shadowView isKindOfClass:[UIImageView class]])
                    {
                        shadowView.hidden = YES;
                    }
                }
            }
        }
    }
    
}

- (void)openURL:(const char*)url
{
    NSString *str = [NSString stringWithUTF8String:url];
    NSURL *urlS = [NSURL URLWithString:str];
    NSData *data = [NSData dataWithContentsOfURL:urlS];
    [m_webview loadData:data MIMEType:@"text/html" textEncodingName:@"UTF-8" baseURL:nil];
}

- (void)removeWebView
{
    [m_webview stopLoading];
    [m_webview removeFromSuperview];
    m_webview = NULL;
}

-(void)setFrame:(CGRect)frame
{
    [m_webview setFrame:frame];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    return true;
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    
}

@end

MWebViewImpl* __createSystemWebView(MWebView* pWebView)
{
    return new MWebViewImplIOS(pWebView);
}

MWebViewImplIOS::MWebViewImplIOS(MWebView* pWebView)
: MWebViewImpl(pWebView)
{
    m_webViewIOS = [[WebView_IOS alloc] init];
    m_Scale = mlib::get_content_scale_factor() / [[EAGLView sharedEGLView] contentScaleFactor];
}

MWebViewImplIOS::~MWebViewImplIOS()
{
    [m_webViewIOS release];
    m_webViewIOS = NULL;
}

void MWebViewImplIOS::showWebView(const std::string url)
{
    [m_webViewIOS createWebView];
    setWebViewFrame();
    this->openURL(url);
}

void MWebViewImplIOS::openURL(std::string url)
{
    [m_webViewIOS openURL:url.c_str()];
}

void MWebViewImplIOS::removeWebView()
{
    [m_webViewIOS removeWebView];
}

void MWebViewImplIOS::setPosition(const CCPoint& pos)
{
    CCPoint position = pos;
    CGRect rect = [[EAGLView sharedEGLView] frame];
    position.x *= m_Scale;
    position.y *= m_Scale;
    position.y = rect.size.height - m_tContentSize.height - position.y;
	m_obPosition = position;
}

void MWebViewImplIOS::setContentSize(const CCSize& size)
{
	m_tContentSize.width = size.width * m_Scale;
    m_tContentSize.height = size.height * m_Scale;
}

bool MWebViewImplIOS::initWithSize(const cocos2d::CCSize &size)
{
    return true;
}

void MWebViewImplIOS::setVisible(bool visible)
{
    
}

void MWebViewImplIOS::setWebViewFrame()
{
    CCPoint pos = this->m_pWebView->getPosition();
    pos = this->m_pWebView->getParent()->convertToWorldSpace(pos);

    CGRect rect;
    this->setPosition(pos);
    rect.size.width = m_tContentSize.width;
    rect.size.height = m_tContentSize.height;
    rect.origin.x = m_obPosition.x;
    rect.origin.y = m_obPosition.y;
    
    [m_webViewIOS setFrame:rect];
}

void MWebViewImplIOS::visit()
{
    if (this->m_pWebView)
    {
        setWebViewFrame();
    }
}
