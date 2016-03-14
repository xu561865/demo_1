//
//  MTextView.cpp
//  legend
//
//  Created by Minhao Zhang on 5/30/13.
//
//

#include "MTextView.h"
#include "MTextViewImpl.h"

MLIB_NS_BEGIN

MTextView::MTextView()
: m_pEditBoxImpl(nullptr)
, m_nFontSize(-1)
, m_colText(ccWHITE)
, m_nMaxLength(0)
, m_fAdjustHeight(0.f)
, _isKeyboardOpen(false)
,_isAutoSizing(false)
{
    
}

MTextView::~MTextView()
{
    CC_SAFE_DELETE(m_pEditBoxImpl);
}

MTextView * MTextView::create()
{
    MTextView *pTextView = new MTextView();
    if (pTextView && pTextView->init())
    {
        pTextView->setZoomOnTouchDown(false);
        pTextView->addTargetWithActionForControlEvent(pTextView, cccontrol_selector(MTextView::touchDownAction), CCControlEventTouchUpInside);
        
        pTextView->autorelease();
        return pTextView;
    }
    CC_SAFE_DELETE(pTextView);
    return nullptr;
}

void MTextView::setText(std::string text)
{
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->setText(text);
    }
}

std::string MTextView::getText()
{
    std::string str = "";
    
    if (m_pEditBoxImpl != nullptr)
    {
        str = m_pEditBoxImpl->getText();
    }
    
    return str;
}

void MTextView::setDelegate(MTextViewDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setDelegate(pDelegate);
    }
}

MTextViewDelegate* MTextView::getDelegate()
{
    return m_pDelegate;
}

void MTextView::setPreferredSize(CCSize size)
{
    CCControlButton::setPreferredSize(size);
    
    if (m_pEditBoxImpl == nullptr)
    {
        m_pEditBoxImpl = __createSystemTextView(this);
        m_pEditBoxImpl->initWithSize(size);
    }
}

bool MTextView::getAutoSizing()
{
    return _isAutoSizing;
}

void MTextView::setAutoSizing(bool autoSizing)
{
    _isAutoSizing = autoSizing;
}

void MTextView::touchDownAction(CCObject *sender, CCControlEvent controlEvent)
{
    m_pEditBoxImpl->openKeyboard();
}

void MTextView::setFont(std::string fontName, int fontSize)
{
    m_strFontName = fontName;
    m_nFontSize = fontSize;
    
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->setFont(fontName, fontSize);
    }
}

void MTextView::setFontName(std::string fontName)
{
    m_strFontName = fontName;
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->setFont(fontName, m_nFontSize);
    }
}

void MTextView::setFontSize(int fontSize)
{
    m_nFontSize = fontSize;
    if (m_strFontName.length() > 0)
    {
        m_pEditBoxImpl->setFont(m_strFontName, m_nFontSize);
    }
}

void MTextView::setFontColor(const ccColor3B& color)
{
    m_colText = color;
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->setFontColor(color);
    }
}

void MTextView::setMaxLength(int maxLength)
{
    m_nMaxLength = maxLength;
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->setMaxLength(maxLength);
    }
}

void MTextView::endEditing()
{
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->closeKeyboard();
    }
}

/* override function */
void MTextView::setPosition(const CCPoint& pos)
{
    CCControlButton::setPosition(pos);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setPosition(pos);
    }
}

void MTextView::setVisible(bool visible)
{
    CCControlButton::setVisible(visible);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setVisible(visible);
    }
}

void MTextView::setContentSize(const CCSize& size)
{
    CCControlButton::setContentSize(size);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setContentSize(size);
    }
}

void MTextView::setAnchorPoint(const CCPoint& anchorPoint)
{
    CCControlButton::setAnchorPoint(anchorPoint);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setAnchorPoint(anchorPoint);
    }
}

void MTextView::visit(void)
{
    CCControlButton::visit();
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->visit();
    }
}

void MTextView::onEnter(void)
{
    CCControlButton::onEnter();
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->onEnter();
    }
}

void MTextView::onExit(void)
{
    CCControlButton::onExit();
    if (m_pEditBoxImpl != NULL)
    {
        // remove system edit control
        m_pEditBoxImpl->closeKeyboard();
    }
}

static CCRect getRect(CCNode * pNode)
{
	CCSize contentSize = pNode->getContentSize();
	CCRect rect = CCRectMake(0, 0, contentSize.width, contentSize.height);
	return CCRectApplyAffineTransform(rect, pNode->nodeToWorldTransform());
}

void MTextView::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    // CCLOG("MTextView::keyboardWillShow");
    CCRect rectTracked = getRect(this);
	// some adjustment for margin between the keyboard and the edit box.
	rectTracked.origin.y -= 4;
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
    if (!rectTracked.intersectsRect(info.end))
    {
        CCLOG("needn't to adjust view layout.");
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    m_fAdjustHeight = info.end.getMaxY() - rectTracked.getMinY();
    // CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", m_fAdjustHeight);
    float adjustHeight = m_fAdjustHeight;
    float duration = info.duration;
    if (_isAutoSizing)
    {
        adjustHeight = 0.0;
        duration = 0.0;
        if (m_pDelegate)
        {
            m_pDelegate->textViewKeyboardWillShow(this, info.end.size.height, info.duration);
        }
    }
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->doAnimationWhenKeyboardMove(duration, adjustHeight);
    }
    
}

void MTextView::keyboardDidShow(CCIMEKeyboardNotificationInfo& info)
{
	
}

void MTextView::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    // CCLOG("MTextView::keyboardWillHide");
    float adjustHeight = m_fAdjustHeight;
    float duration = info.duration;
    if (_isAutoSizing)
    {
        adjustHeight = 0.0;
        duration = 0.0;
        if (m_pDelegate)
        {
            m_pDelegate->textViewKeyboardWillHide(this, info.begin.size.height, info.duration);
        }
    }
    
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->doAnimationWhenKeyboardMove(duration, -adjustHeight);
    }
}

void MTextView::keyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    _isKeyboardOpen = false;
}

void MTextView::keyboardChangeFrame(CCIMEKeyboardNotificationInfo &info)
{
    if (_isAutoSizing)
    {
        if (m_pDelegate)
        {
            m_pDelegate->textViewKeyboardChangeFrame(this, info.end.size.height, info.duration);
        }
    }
}

void MTextView::setScale(float scale)
{
    CCControlButton::setScale(scale);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setContentSize(getContentSize());
        m_pEditBoxImpl->setPosition(getPosition());
    }
}

void MTextView::setScaleX(float fScaleX)
{
    CCControlButton::setScaleX(fScaleX);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setContentSize(getContentSize());
        m_pEditBoxImpl->setPosition(getPosition());
    }
}

void MTextView::setScaleY(float fScaleY)
{
    CCControlButton::setScaleY(fScaleY);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setContentSize(getContentSize());
        m_pEditBoxImpl->setPosition(getPosition());
    }
}

void MTextView::setFocus(void)
{
    if (!_isKeyboardOpen)
    {
        m_pEditBoxImpl->openKeyboard();
        _isKeyboardOpen = true;
    }
    else
    {}
}

void MTextView::setMaxHeight(float maxHeight)
{
    if (m_pEditBoxImpl != nullptr)
    {
        m_pEditBoxImpl->setMaxHeight(maxHeight);
    }
}

MLIB_NS_END
