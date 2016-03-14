//
//  MTextViewImplIOS.mm
//  mlib
//
//  Created by Xu Jinyang on 11/28/13.
//
//

#include "MTextViewImplIOS.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define kLabelZOrder 9999

#include "MTextView.h"
#import "EAGLView.h"

#define getTextViewImplIOS() ((mlib::MTextViewImplIOS*)_textView)

static const int MTEXTVIEW_PADDING = 5;

@implementation CustomUITextView
- (CGRect)textRectForBounds:(CGRect)bounds {
    float padding = MTEXTVIEW_PADDING * cocos2d::CCEGLView::sharedOpenGLView()->getScaleX() / [[EAGLView sharedEGLView] contentScaleFactor ];
    return CGRectMake(bounds.origin.x + padding, bounds.origin.y + padding,
                      bounds.size.width - padding*2, bounds.size.height - padding*2);
}
@end

@implementation TextViewImplIOS

@synthesize textViewIOS = _textViewIOS;
@synthesize editState = _editState;
@synthesize textView = _textView;

- (void)dealloc
{
    [_textViewIOS resignFirstResponder];
    [_textViewIOS removeFromSuperview];
    self.textViewIOS = nullptr;
    [super dealloc];
}

-(id) initWithFrame: (CGRect) frameRect textView: (void*) textView
{
    self = [super init];
    
    do
    {
        if (self == nil) break;
        _editState = NO;
        self.textViewIOS = [[[CustomUITextView alloc] initWithFrame: frameRect] autorelease];
        if (!_textViewIOS) break;
        [_textViewIOS setTextColor:[UIColor whiteColor]];
        _textViewIOS.font = [UIFont systemFontOfSize:frameRect.size.height*2/3];
//		_textViewIOS.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
        _textViewIOS.backgroundColor = [UIColor clearColor];
//        _textViewIOS.borderStyle = UITextBorderStyleNone;
        _textViewIOS.delegate = self;
        _textViewIOS.returnKeyType = UIReturnKeyDefault;
//        [_textViewIOS addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
        self.textView = textView;
        
		_textViewIOS.hidden = YES;
        
        return self;
    }while(0);
    
    return nil;
}

-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance
{
    id eglView = [EAGLView sharedEGLView];
    [eglView doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
}

-(void) setPosition:(CGPoint) pos
{
    CGRect frame = [_textViewIOS frame];
    frame.origin = pos;
    [_textViewIOS setFrame:frame];
}

-(void) setContentSize:(CGSize) size
{
    CGRect frame = [_textViewIOS frame];
    frame.size = size;
    [_textViewIOS setFrame:frame];
}

-(void) visit
{
    
}

-(void) openKeyboard
{
    [[EAGLView sharedEGLView] addSubview:_textViewIOS];
    [_textViewIOS becomeFirstResponder];
}

-(void) closeKeyboard
{
    [_textViewIOS resignFirstResponder];
    [_textViewIOS removeFromSuperview];
}

//- (BOOL)textViewShouldEndEditing:(UITextView *)textView
//{
//    if (textView == _textViewIOS) {
//        [textView resignFirstResponder];
//    }
//    return YES;
//}

-(void)animationSelector
{
    id eglView = [EAGLView sharedEGLView];
    [eglView doAnimationWhenAnotherEditBeClicked];
}

- (BOOL)textViewShouldBeginEditing:(UITextView *)textView
{
    _editState = YES;
    id eglView = [EAGLView sharedEGLView];
    if ([eglView isKeyboardShown])
    {
        [self performSelector:@selector(animationSelector) withObject:nil];
    }
    
    mlib::MTextViewDelegate* pDelegate = getTextViewImplIOS()->getDelegate();
    if (pDelegate != nullptr)
    {
        pDelegate->textViewEditingDidBegin(getTextViewImplIOS()->getTextView());
    }
    
//    cocos2d::extension::CCEditBoxDelegate *pDel
    
    return YES;
}

- (BOOL)textViewShouldEndEditing:(UITextView *)textView
{
    _editState = NO;
    getTextViewImplIOS()->setText(getTextViewImplIOS()->getText());
    
    mlib::MTextViewDelegate* pDelegate = getTextViewImplIOS()->getDelegate();
    if (pDelegate != nullptr)
    {
        pDelegate->textViewEditingDidEnd(getTextViewImplIOS()->getTextView());
        pDelegate->textViewReturn(getTextViewImplIOS()->getTextView());
    }
    
    getTextViewImplIOS()->onEndEditing();
    return YES;
}

-(void)textViewDidChange:(UITextView *)textView
{
    
    mlib::MTextViewDelegate* pDelegate = getTextViewImplIOS()->getDelegate();
    getTextViewImplIOS()->textChange();
    if (pDelegate != nullptr)
    {
        pDelegate->textViewTextChanged(getTextViewImplIOS()->getTextView(), getTextViewImplIOS()->getText());
    }
    
    

}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if (getTextViewImplIOS()->getMaxLength() < 0)
    {
        return YES;
    }
    
    NSUInteger oldLength = [textView.text length];
    NSUInteger replacementLength = [text length];
    NSUInteger rangeLength = range.length;
    
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    
    return newLength <= getTextViewImplIOS()->getMaxLength();
}

@end

MLIB_NS_BEGIN

MTextViewImpl * __createSystemTextView(MTextView *pTextView)
{
    return new MTextViewImplIOS(pTextView);
}

MTextViewImplIOS::MTextViewImplIOS(MTextView *pTextView)
: MTextViewImpl(pTextView)
, m_pLabel(nullptr)
, m_systemControl(nullptr)
, m_obAnchorPoint(ccp(0.5, 0.5))
, m_nMaxTextLength(-1)
, m_maxHeight(0)
{
    m_bInRetinaMode = [[EAGLView sharedEGLView] contentScaleFactor] == 2.0f ? true : false;
}

MTextViewImplIOS::~MTextViewImplIOS()
{
    [m_systemControl release];
}

void MTextViewImplIOS::doAnimationWhenKeyboardMove(float duration, float distance)
{
    if ([m_systemControl isEditState] || distance < 0.f)
    {
        [m_systemControl doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
    }
}

bool MTextViewImplIOS::initWithSize(const cocos2d::CCSize &size)
{
    do
    {
        CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
        
        CGRect rect = CGRectMake(0, 0, size.width * eglView->getScaleX(),size.height * eglView->getScaleY());
        
        // modified by xujinyang(for cc)
        rect.size.width *= CC_CONTENT_SCALE_FACTOR();
        rect.size.height *= CC_CONTENT_SCALE_FACTOR();
        //        if (m_bInRetinaMode)
        //        {
        //            rect.size.width /= 2.0f;
        //            rect.size.height /= 2.0f;
        //        }
        
        m_systemControl = [[TextViewImplIOS alloc] initWithFrame:rect textView:this];
        if (!m_systemControl) break;
        
		initInactiveLabels(size);
        setContentSize(size);
		
        return true;
    }while (0);
    
    return false;
}

void MTextViewImplIOS::initInactiveLabels(const cocos2d::CCSize &size)
{
    const char* pDefaultFontName = [[m_systemControl.textViewIOS.font fontName] UTF8String];
    
	m_pLabel = CCLabelTTF::create("", "", 0.0f);
    m_pLabel->setAnchorPoint(ccp(0, 1));
    m_pLabel->setFontFillColor(ccWHITE);
    m_pLabel->setVisible(false);
    m_pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    _textView->addChild(m_pLabel, kLabelZOrder);
    
    setFont(pDefaultFontName, size.height*2/3);
}

void MTextViewImplIOS::placeInactiveLabels()
{
    auto dim = CCSizeMake(fmaxf(m_tContentSize.width - MTEXTVIEW_PADDING*2, 0), fmaxf(m_tContentSize.height - MTEXTVIEW_PADDING*2, 0));
    m_pLabel->setDimensions(dim);
    
    m_pLabel->setPosition(ccp(MTEXTVIEW_PADDING, m_tContentSize.height - MTEXTVIEW_PADDING));
}

void MTextViewImplIOS::setInactiveText(const char *pText)
{
    m_pLabel->setString(getText().c_str());
	
	// Clip the text width to fit to the text box
	float fMaxWidth = m_pLabel->getDimensions().width;
	float fMaxHeight = m_pLabel->getDimensions().height;
	CCRect clippingRect = m_pLabel->getTextureRect();
	if(clippingRect.size.width > fMaxWidth
       || clippingRect.size.height > fMaxHeight) {
		clippingRect.size.width = fMaxWidth;
        clippingRect.size.height = fMaxHeight;
		m_pLabel->setTextureRect(clippingRect);
	}
}

void MTextViewImplIOS::setFont(std::string fontName, int fontSize)
{
    bool isValidFontName = true;
	if(fontName.length() == 0) {
        isValidFontName = false;
    }
    
    float retinaFactor = m_bInRetinaMode ? 2.0f : 1.0f;
	NSString * fntName = [NSString stringWithUTF8String:fontName.c_str()];
    float scaleFactor = CCEGLView::sharedOpenGLView()->getScaleX();
    UIFont *textFont = nil;
    if (isValidFontName) {
        textFont = [UIFont fontWithName:fntName size:fontSize * scaleFactor / retinaFactor];
    }
    
    if (!isValidFontName || textFont == nil){
        textFont = [UIFont systemFontOfSize:fontSize * scaleFactor / retinaFactor];
    }
    
	if(textFont != nil) {
		[m_systemControl.textViewIOS setFont:textFont];
    }
    
	m_pLabel->setFontName(fontName.c_str());
	m_pLabel->setFontSize(fontSize);
}

void MTextViewImplIOS::setFontColor(const ccColor3B& color)
{
    m_systemControl.textViewIOS.textColor = [UIColor colorWithRed:color.r / 255.0f green:color.g / 255.0f blue:color.b / 255.0f alpha:1.0f];
	m_pLabel->setFontFillColor(color);
}

void MTextViewImplIOS::setMaxLength(int maxLength)
{
    m_nMaxTextLength = maxLength;
}

int MTextViewImplIOS::getMaxLength()
{
    return m_nMaxTextLength;
}

float MTextViewImplIOS::getMaxHeight()
{
    return m_maxHeight;
}

void MTextViewImplIOS::setMaxHeight(float maxHeight)
{
    m_maxHeight = maxHeight;
}

bool MTextViewImplIOS::isEditing()
{
    return [m_systemControl isEditState] ? true : false;
}

void MTextViewImplIOS::setText(std::string text)
{
    m_systemControl.textViewIOS.text = [NSString stringWithUTF8String:text.c_str()];
    adjustTextViewContentSize();
    adjustTextFieldPosition();
	if(m_systemControl.textViewIOS.hidden == YES) {
		setInactiveText(text.c_str());
        m_pLabel->setVisible(true);
	}
}

std::string MTextViewImplIOS::getText(void)
{
    return [m_systemControl.textViewIOS.text UTF8String];
}

static CGPoint convertDesignCoordToScreenCoord(const CCPoint& designCoord, bool bInRetinaMode)
{
    // modified by xujinyang(for cc)
    CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
    
    CCPoint pt = CCPointMake(designCoord.x * eglView->getScaleX(), designCoord.y * eglView->getScaleY());
    float viewH = (float)[[EAGLView sharedEGLView] getHeight];
    
    CGPoint screenPos = CGPointMake(pt.x, viewH - pt.y);
    
    if (bInRetinaMode)
    {
        screenPos.x = screenPos.x / 2.0f;
        screenPos.y = screenPos.y / 2.0f;
    }
    return screenPos;
}

void MTextViewImplIOS::setPosition(const CCPoint& pos)
{
	m_obPosition = pos;
	adjustTextFieldPosition();
}

void MTextViewImplIOS::setVisible(bool visible)
{
    m_systemControl.textViewIOS.hidden = !visible;
}

void MTextViewImplIOS::setContentSize(const CCSize& size)
{
    m_tContentSize = size;

    placeInactiveLabels();
    CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
    CGSize controlSize = CGSizeMake(size.width * eglView->getScaleX(),size.height * eglView->getScaleY());
    
    // modified by xujinyang(for cc)
    controlSize.width *= CC_CONTENT_SCALE_FACTOR();
    controlSize.height *= CC_CONTENT_SCALE_FACTOR();
    if (m_bInRetinaMode)
    {
        controlSize.width /= 2.0f;
        controlSize.height /= 2.0f;
    }
    [m_systemControl setContentSize:controlSize];
}

void MTextViewImplIOS::setAnchorPoint(const CCPoint& anchorPoint)
{
	m_obAnchorPoint = anchorPoint;
	setPosition(m_obPosition);
}

void MTextViewImplIOS::visit(void)
{
    
}

void MTextViewImplIOS::onEnter(void)
{
    m_initHeight = _textView->getContentSize().height;
    
    adjustTextViewContentSize();
    adjustTextFieldPosition();
    setInactiveText(getText().c_str());
}

void MTextViewImplIOS::adjustTextFieldPosition()
{
	CCSize contentSize = _textView->getContentSize();
	CCRect rect = CCRectMake(0, 0, contentSize.width, contentSize.height);
    rect = CCRectApplyAffineTransform(rect, _textView->nodeToWorldTransform());
	
	CCPoint designCoord = ccp(rect.origin.x, rect.origin.y + rect.size.height);
    CGPoint screenPos = convertDesignCoordToScreenCoord(designCoord, m_bInRetinaMode);
    [m_systemControl setPosition:screenPos];
}

void MTextViewImplIOS::adjustTextViewContentSize()
{
    if (_textView->getAutoSizing())
    {
        float scale = mlib::get_content_scale_factor() / [[EAGLView sharedEGLView] contentScaleFactor];

        float width = _textView->getContentSize().width;
        float textViewHeight = _textView->getContentSize().height;
        NSLog(@"message is   %@", m_systemControl.textViewIOS.text);
        CGRect chatBoxFrame = m_systemControl.textViewIOS.frame;
        
        CGSize newSize = [m_systemControl.textViewIOS.text sizeWithFont:m_systemControl.textViewIOS.font  constrainedToSize:CGSizeMake(width, 9999)];
        
        CGFloat newSizeH = newSize.height / scale;
        
        NSString *blanks = @" ";
        CGSize tmp = [blanks sizeWithFont:m_systemControl.textViewIOS.font constrainedToSize:CGSizeMake(width, 9999)];
        CCSize size;
        size.width = chatBoxFrame.size.width / scale / _textView->getScaleX();
        size.height = (newSizeH + tmp.height / scale) / _textView->getScaleY();
        
        float diffHeight = 0;
        if (getMaxHeight() > 0 && size.height > getMaxHeight() / _textView->getScaleY())
        {
            size.height = getMaxHeight() / _textView->getScaleY();
        }
        if (size.height < m_initHeight)
        {
            size.height = m_initHeight;
        }
        
        _textView->setContentSize(size);
        _textView->setPreferredSize(size);
        
        diffHeight = size.height - textViewHeight;
        
        mlib::MTextViewDelegate* pDelegate = _textView->getDelegate();
        if (pDelegate)
        {
            pDelegate->textViewTextChangeSize(_textView, diffHeight);
        }
    }
}



void MTextViewImplIOS::openKeyboard()
{
	m_pLabel->setVisible(false);
    
    adjustTextFieldPosition();
    
	m_systemControl.textViewIOS.hidden = NO;
    [m_systemControl openKeyboard];
}

void MTextViewImplIOS::textChange()
{
    NSLog(@"text view did change");
    adjustTextViewContentSize();
    adjustTextFieldPosition();
}

void MTextViewImplIOS::closeKeyboard()
{
    [m_systemControl closeKeyboard];
}

void MTextViewImplIOS::onEndEditing()
{
	m_systemControl.textViewIOS.hidden = YES;
	m_pLabel->setVisible(true);
    setInactiveText(getText().c_str());
}

MLIB_NS_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */
