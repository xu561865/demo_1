//
//  MTextViewImplAndroid.mm
//  mlib
//
//  Created by Xu Jinyang on 11/28/13.
//
//

#include "MTextViewImplAndroid.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define kLabelZOrder 9999

#include "MTextView.h"

#define getTextViewImplIOS() ((mlib::MTextViewImplAndroid*)_textView)

static const int MTEXTVIEW_PADDING = 5;

MLIB_NS_BEGIN

MTextViewImpl * __createSystemTextView(MTextView *pTextView)
{
	return new MTextViewImplAndroid(pTextView);
}

MTextViewImplAndroid::MTextViewImplAndroid(MTextView *pTextView)
: MTextViewImpl(pTextView)
, m_pLabel(nullptr)
, m_systemControl(nullptr)
, m_obAnchorPoint(ccp(0.5, 0.5))
, m_nMaxTextLength(-1)
, m_maxHeight(0)
, m_eEditBoxInputMode(kEditBoxInputModeSingleLine)
, m_eEditBoxInputFlag(kEditBoxInputFlagInitialCapsAllCharacters)
, m_eKeyboardReturnType(kKeyboardReturnTypeSend)
, m_colText(ccWHITE)
{
//    m_bInRetinaMode = [[EAGLView sharedEGLView] contentScaleFactor] == 2.0f ? true : false;
}

MTextViewImplAndroid::~MTextViewImplAndroid()
{
//    [m_systemControl release];
}

void MTextViewImplAndroid::doAnimationWhenKeyboardMove(float duration, float distance)
{
//    if ([m_systemControl isEditState] || distance < 0.f)
//    {
//        [m_systemControl doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
//    }
}

bool MTextViewImplAndroid::initWithSize(const cocos2d::CCSize &size)
{
	int fontSize = 12;
	m_pLabel = CCLabelTTF::create("", "", fontSize);
	m_pLabel->setAnchorPoint(ccp(0, 1));
	m_pLabel->setColor(ccWHITE);
//	m_pLabel->setVisible(false);
	m_pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	_textView->addChild(m_pLabel, kLabelZOrder);

	m_tContentSize = size;
	return true;
}

void MTextViewImplAndroid::setFont(std::string fontName, int fontSize)
{
	M_INFO("set fontsize: " << fontSize);
	if(m_pLabel != nullptr)
	{
		m_pLabel->setFontName(fontName.c_str());
		m_pLabel->setFontSize(fontSize);
	}
}

void MTextViewImplAndroid::setFontColor(const ccColor3B& color)
{
	m_colText = color;
	m_pLabel->setColor(color);
}

void MTextViewImplAndroid::setMaxLength(int maxLength)
{
	m_nMaxTextLength = maxLength;
}

int MTextViewImplAndroid::getMaxLength()
{
	return m_nMaxTextLength;
}

float MTextViewImplAndroid::getMaxHeight()
{
	return m_maxHeight;
}

void MTextViewImplAndroid::setMaxHeight(float maxHeight)
{
	m_maxHeight = maxHeight;
}

bool MTextViewImplAndroid::isEditing()
{
//    return [m_systemControl isEditState] ? true : false;
	return false;
}

void MTextViewImplAndroid::setText(std::string text)
{
	m_strText = text;

	if (m_strText.length() > 0)
	{
		std::string strToShow;

		if (kEditBoxInputFlagPassword == m_eEditBoxInputFlag)
		{
			long length = cc_utf8_strlen(m_strText.c_str());
			for (long i = 0; i < length; i++)
			{
				strToShow.append("\u25CF");
			}
		}
		else
		{
			strToShow = m_strText;
		}

		m_pLabel->setString(strToShow.c_str());

		auto pt = m_pLabel->getParent()->convertToWorldSpace(m_pLabel->getPosition());
		M_DEBUG("pt: " << pt.x << ", " << pt.y);

		// Clip the text width to fit to the text box
//		float fMaxWidth = m_tContentSize.width - MTEXTVIEW_PADDING * 2;
//		CCRect clippingRect = m_pLabel->getTextureRect();
//		if(clippingRect.size.width > fMaxWidth) {
//			clippingRect.size.width = fMaxWidth;
//			m_pLabel->setTextureRect(clippingRect);
//		}

	}
	else
	{
		m_pLabel->setString("");
	}
}

std::string MTextViewImplAndroid::getText(void)
{
	return m_strText.c_str();
}

void MTextViewImplAndroid::setPosition(const CCPoint& pos)
{
//	m_obPosition = pos;
//	adjustTextFieldPosition();
}

void MTextViewImplAndroid::setVisible(bool visible)
{
//    m_systemControl.textViewIOS.hidden = !visible;
}

void MTextViewImplAndroid::setContentSize(const CCSize& size)
{
	m_tContentSize = size;

	m_pLabel->setDimensions(CCSizeMake(m_tContentSize.width - MTEXTVIEW_PADDING*2, m_tContentSize.height - MTEXTVIEW_PADDING*2));
	auto pt = m_pLabel->getDimensions();
	M_DEBUG("dimensions: " << pt.width << ", " << pt.height);


	m_pLabel->setPosition(ccp(MTEXTVIEW_PADDING, m_tContentSize.height - MTEXTVIEW_PADDING));
//
//    placeInactiveLabels();
//    CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
//    CGSize controlSize = CGSizeMake(size.width * eglView->getScaleX(),size.height * eglView->getScaleY());
//
//    // modified by xujinyang(for cc)
//    controlSize.width *= CC_CONTENT_SCALE_FACTOR();
//    controlSize.height *= CC_CONTENT_SCALE_FACTOR();
//    if (m_bInRetinaMode)
//    {
//        controlSize.width /= 2.0f;
//        controlSize.height /= 2.0f;
//    }
//    [m_systemControl setContentSize:controlSize];
}

void MTextViewImplAndroid::setAnchorPoint(const CCPoint& anchorPoint)
{
//	m_obAnchorPoint = anchorPoint;
//	setPosition(m_obPosition);
}

void MTextViewImplAndroid::visit(void)
{

}

void MTextViewImplAndroid::onEnter(void)
{
//    m_initHeight = _textView->getContentSize().height;
//
//    adjustTextViewContentSize();
//    adjustTextFieldPosition();
//    setInactiveText(getText().c_str());
}

static void editBoxCallbackFunc(const char* pText, void* ctx)
{
	MTextViewImplAndroid* thiz = (MTextViewImplAndroid*)ctx;
	thiz->setText(pText);

	if (thiz->getDelegate() != NULL)
	{
		thiz->getDelegate()->textViewTextChanged(thiz->getTextView(), thiz->getText());
		thiz->getDelegate()->textViewEditingDidEnd(thiz->getTextView());
		thiz->getDelegate()->textViewReturn(thiz->getTextView());
	}
}

void MTextViewImplAndroid::openKeyboard()
{
	if (m_pDelegate != NULL)
	{
		m_pDelegate->textViewEditingDidBegin(_textView);
	}

	showEditTextDialogJNI("",
			m_strText.c_str(),
			m_eEditBoxInputMode,
			m_eEditBoxInputFlag,
			m_eKeyboardReturnType,
			m_nMaxTextLength,
			editBoxCallbackFunc,
			(void*)this );
}

void MTextViewImplAndroid::textChange()
{
//    NSLog(@"text view did change");
//    adjustTextViewContentSize();
//    adjustTextFieldPosition();
}

void MTextViewImplAndroid::closeKeyboard()
{
//    [m_systemControl closeKeyboard];
}

void MTextViewImplAndroid::onEndEditing()
{
//	m_systemControl.textViewIOS.hidden = YES;
//	m_pLabel->setVisible(true);
//    setInactiveText(getText().c_str());
}

MLIB_NS_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */
