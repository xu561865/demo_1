//
//  MTextView.h
//  legend
//
//  Created by Minhao Zhang on 5/30/13.
//
//

#ifndef __legend__MTextView__
#define __legend__MTextView__

#include <iostream>
#include "cocos2d.h"
#include "mlib_ccext.h"
#include "mlib_helpers.h"


MLIB_NS_BEGIN

class MTextView;
class MTextViewImpl;

class MTextViewDelegate
{
public:
    virtual ~MTextViewDelegate() {};
    
    virtual void textViewEditingDidBegin(MTextView *textView) {}
    virtual void textViewEditingDidEnd(MTextView *textView) {}
    virtual void textViewTextChanged(MTextView *textView, const std::string &text) {};
    virtual void textViewReturn(MTextView *textView) = 0;
    virtual void textViewKeyboardWillShow(MTextView *textView, float keyboardHeight, float duration) {};
    virtual void textViewKeyboardWillHide(MTextView *textView, float keyboardHeight, float duration) {};
    virtual void textViewKeyboardChangeFrame(MTextView *textView, float keyboardHeight, float duration) {};
    virtual void textViewTextChangeSize(MTextView *textView, float height ) {};
};

class MTextView
: public cocos2d::extension::CCControlButton
, public CCIMEDelegate
{
public:
    MTextView();
    virtual ~MTextView();
    
    static MTextView * create();
    
    void setText(std::string text);
    std::string getText();
    
public:
    void setDelegate(MTextViewDelegate* pDelegate);
    MTextViewDelegate* getDelegate();
    
    void setPreferredSize(CCSize size);
    
	void setFont(std::string fontName, int fontSize);
    void setFontName(std::string fontName);
    void setFontSize(int fontSize);

    void setFontColor(const ccColor3B& color);
    
    void setMaxLength(int maxLength);
    
    void endEditing();
    
    /* override functions */
    virtual void setPosition(const CCPoint& pos);
    virtual void setVisible(bool visible);
    virtual void setContentSize(const CCSize& size);
	virtual void setAnchorPoint(const CCPoint& anchorPoint);
    virtual void visit(void);
	virtual void onEnter(void);
    virtual void onExit(void);
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardChangeFrame(CCIMEKeyboardNotificationInfo& info);
    bool getAutoSizing();
    void setAutoSizing(bool autoSizing);
    /* callback funtions */
    void touchDownAction(CCObject *sender, CCControlEvent controlEvent);
    void setMaxHeight(float maxHeight);

protected:
    mlib::MTextViewImpl*      m_pEditBoxImpl;
    MTextViewDelegate*  m_pDelegate;
    
//    EditBoxInputMode    m_eEditBoxInputMode;
//    EditBoxInputFlag    m_eEditBoxInputFlag;
//    KeyboardReturnType  m_eKeyboardReturnType;
    
    std::string m_strFontName;
//    std::string m_strPlaceholderFontName;
    
    int m_nFontSize;
//    int m_nPlaceholderFontSize;
    
    ccColor3B m_colText;
//    ccColor3B m_colPlaceHolder;
    
    int   m_nMaxLength;
    float m_fAdjustHeight;
//    int   m_nScriptEditBoxHandler;
    
public:
    virtual void setScale(float scale);
    virtual void setScaleX(float fScaleX);
    virtual void setScaleY(float fScaleY);
    
    void setFocus(void);
protected:
    bool _isKeyboardOpen;
    bool _isAutoSizing;
};

MLIB_NS_END

#endif /* defined(__legend__MTextView__) */
