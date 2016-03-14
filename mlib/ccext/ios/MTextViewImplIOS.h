//
//  MTextViewImplIOS.h
//  mlib
//
//  Created by Xu Jinyang on 11/28/13.
//
//

#ifndef __legend__MTextViewImplIOS__
#define __legend__MTextViewImplIOS__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "MTextViewImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface CustomUITextView : UITextView
{
}

@end

@interface TextViewImplIOS : NSObject <UITextViewDelegate>
{
    CustomUITextView* _textViewIOS;
    void* _textView;
    BOOL _editState;
}

@property(nonatomic, retain) UITextView* textViewIOS;
@property(nonatomic, readonly, getter = isEditState) BOOL editState;
@property(nonatomic, assign) void* textView;

-(id) initWithFrame: (CGRect) frameRect textView: (void*) textView;
-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance;
-(void) setPosition:(CGPoint) pos;
-(void) setContentSize:(CGSize) size;
-(void) visit;
-(void) openKeyboard;
-(void) closeKeyboard;

@end

MLIB_NS_BEGIN

class MTextView;

class MTextViewImplIOS : public MTextViewImpl
{
public:
    MTextViewImplIOS(MTextView* pTextView);
    virtual ~MTextViewImplIOS();
    
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
	void			initInactiveLabels(const CCSize& size);
	void			setInactiveText(const char* pText);
	void			adjustTextFieldPosition();
    void			adjustTextViewContentSize();
    void            placeInactiveLabels();
	
    CCLabelTTF*     m_pLabel;
    CCSize          m_tContentSize;
    CCPoint         m_obPosition;
    CCPoint         m_obAnchorPoint;
    TextViewImplIOS* m_systemControl;
    int             m_nMaxTextLength;
    bool            m_bInRetinaMode;
    float           m_maxHeight;
    float           m_initHeight;
};

MLIB_NS_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */

#endif /* defined(__legend__MTextViewImplIOS__) */
