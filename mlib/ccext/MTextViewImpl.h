//
//  MTextViewImpl.h
//  legend
//
//  Created by Xu Jinyang on 11/28/13.
//
//

#ifndef legend_MTextViewImpl_h
#define legend_MTextViewImpl_h

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "mlib_helpers.h"
#include "MTextView.h"

MLIB_NS_BEGIN

class MTextViewImpl
{
public:
    MTextViewImpl(mlib::MTextView* pTextView) : _textView(pTextView) {}
    virtual ~MTextViewImpl() {}
    
    virtual bool initWithSize(const CCSize& size) = 0;
    virtual void setFont(std::string fontName, int fontSize) = 0;
    virtual void setFontColor(const ccColor3B& color) = 0;
//    virtual void setPlaceholderFont(const char* pFontName, int fontSize) = 0;
//    virtual void setPlaceholderFontColor(const ccColor3B& color) = 0;
//    virtual void setInputMode(EditBoxInputMode inputMode) = 0;
//    virtual void setInputFlag(EditBoxInputFlag inputFlag) = 0;
    virtual void setMaxLength(int maxLength) = 0;
    virtual int  getMaxLength() = 0;
//    virtual void setReturnType(KeyboardReturnType returnType) = 0;
    virtual bool isEditing() = 0;
    
    virtual void setText(std::string text) = 0;
    virtual std::string getText(void) = 0;
//    virtual void setPlaceHolder(const char* pText) = 0;
    virtual void doAnimationWhenKeyboardMove(float duration, float distance) = 0;
    
    virtual void openKeyboard() = 0;
    virtual void closeKeyboard() = 0;
    
    virtual void setPosition(const CCPoint& pos) = 0;
    virtual void setVisible(bool visible) = 0;
    virtual void setContentSize(const CCSize& size) = 0;
	virtual void setAnchorPoint(const CCPoint& anchorPoint) = 0;
    virtual void visit(void) = 0;
    virtual void onEnter(void) = 0;
    
    virtual float getMaxHeight() = 0;
    virtual void setMaxHeight(float maxHeight) = 0;
    
    void setDelegate(MTextViewDelegate* pDelegate) { m_pDelegate = pDelegate; };
    MTextViewDelegate* getDelegate() { return m_pDelegate; };
    mlib::MTextView* getTextView() { return _textView; };
protected:
    MTextViewDelegate* m_pDelegate;
    mlib::MTextView *_textView;
};

extern MTextViewImpl * __createSystemTextView(MTextView *pTextView);

MLIB_NS_END

#endif
