//
//  MTextViewImplAndroid.h
//  mlib
//
//  Created by Xu Jinyang on 11/28/13.
//
//

#ifndef __legend__MTextViewImplAndroid__
#define __legend__MTextViewImplAndroid__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "MTextViewImpl.h"

MLIB_NS_BEGIN

class MTextView;

class MTextViewImplAndroid : public MTextViewImpl
{
public:
    MTextViewImplAndroid(MTextView* pTextView);
    virtual ~MTextViewImplAndroid();
    
    virtual bool initWithSize(const CCSize& size);
    virtual void setFont(std::string fontName, int fontSize);
    virtual void setFontColor(const ccColor3B& color);
//    virtual void setPlaceholderFont(const char* pFontName, int fontSize);
//    virtual void setPlaceholderFontColor(const ccColor3B& color);
//    virtual void setInputMode(EditBoxInputMode inputMode);
//    virtual void setInputFlag(EditBoxInputFlag inputFlag);
    virtual void setMaxLength(int maxLength);
    virtual int  getMaxLength();
//    virtual void setReturnType(KeyboardReturnType returnType);
    virtual bool isEditing();
    
    virtual void setText(std::string text);
    virtual std::string getText(void);
//    virtual void setPlaceHolder(const char* pText);
    virtual void setPosition(const CCPoint& pos);
    virtual void setVisible(bool visible);
    virtual void setContentSize(const CCSize& size);
	virtual void setAnchorPoint(const CCPoint& anchorPoint);
    virtual void visit(void);
	virtual void onEnter(void);
    virtual void doAnimationWhenKeyboardMove(float duration, float distance);
    virtual void openKeyboard();
    virtual void closeKeyboard();
	
	virtual void onEndEditing();
    virtual void textChange();
    
    virtual float getMaxHeight();
    virtual void setMaxHeight(float maxHeight);
    
private:
//	void			initInactiveLabels(const CCSize& size);
//	void			setInactiveText(const char* pText);
//	void			adjustTextFieldPosition();
//    void			adjustTextViewContentSize();
//    void            placeInactiveLabels();
	
    CCLabelTTF*     m_pLabel;
    CCSize          m_tContentSize;
    CCPoint         m_obPosition;
    CCPoint         m_obAnchorPoint;
    void* m_systemControl;
    int             m_nMaxTextLength;
    bool            m_bInRetinaMode;
    float           m_maxHeight;
    float           m_initHeight;

    EditBoxInputMode m_eEditBoxInputMode;
	EditBoxInputFlag m_eEditBoxInputFlag;
	KeyboardReturnType m_eKeyboardReturnType;

	std::string m_strText;

	ccColor3B m_colText;
};

MLIB_NS_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */

#endif /* defined(__legend__MTextViewImplAndroid__) */
