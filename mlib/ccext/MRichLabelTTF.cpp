//
//  MRichLabelTTF.cpp
//  XMLTest
//
//  Created by lishilei on 13-4-11.
//
//

#include "MRichLabelTTF.h"

#include "mlib_utils.h"

using namespace cocos2d;
using namespace std;
using namespace mlib;

MRichLabelTTF::MRichLabelTTF()
{
}

MRichLabelTTF::~MRichLabelTTF()
{
}

bool MRichLabelTTF::init()
{
    if (CCLayer::init())
    {
        _cleanText = "";
        _fontFamily = "Helvetica";
        _fontSize = 12;
        _color = ccWHITE;
        
        this->setContentSize(CCSizeZero);
        this->setAnchorPoint(ccp(0.5, 0.5));
        this->ignoreAnchorPointForPosition(false);
        
        _saxParser.setDelegator(this);

        return true;
    }
    return false;
}

void MRichLabelTTF::parse()
{
    if (_cleanText == _text) return;
    
    std::string atext = _text;
    atext = SSTR("<text>" << atext << "</text>");
//    if (atext.find('<') != 0)
//    {
//        atext = SSTR("<text>" << atext << "</text>");
//    }
    _fontStack.clear();
    _texts.clear();
    
    bool result = _saxParser.parse(atext.data(), atext.size());
    CC_UNUSED_PARAM(result);
    M_ASSERT(result, "parsing text error!");
    
    reloadData();
    _cleanText = _text;
}

void MRichLabelTTF::startElement(void *ctx, const char *name, const char **attrs)
{
    MRichLabelTTF *pThis = this;
    
    std::string tag = (char *)name;
    M_ASSERT(tag == "text");
    
    FontType_t ft;
    if (pThis->_fontStack.size())
    {
        ft = pThis->_fontStack.back();
    }
    else
    {
        ft.family = pThis->_fontFamily;
        ft.size = pThis->_fontSize;
        ft.color = pThis->_color;
    }
    ft.bmpFont = "";
    if (attrs)
    {
        for (int i = 0; attrs[i] != nullptr; ++i)
        {
            char * attrName = (char *)(attrs[i]);
            char * attrValue = (char *)(attrs[++i]);
            M_ASSERT(attrValue != nullptr);
            
            std::string attr = attrName;
            std::string value = attrValue;
            
            if (attr == "c")
            {
                if (value.find('#') == 0) // hex
                {
                    auto val = strtol(value.c_str() + 1, nullptr, 16);
                    ft.color.r = (val & 0xff0000) >> 4;
                    ft.color.g = (val & 0x00ff00) >> 2;
                    ft.color.b = (val & 0x0000ff);
                }
                else
                {
                    auto vals = split_string(value, ",");
                    M_ASSERT(vals.size() == 3);
                    ft.color.r = VALUE_FROM_STRING(vals[0], uint32_t);
                    ft.color.g = VALUE_FROM_STRING(vals[1], uint32_t);
                    ft.color.b = VALUE_FROM_STRING(vals[2], uint32_t);
                }
            }
            else if (attr == "f")
            {
                ft.family = value;
            }
            else if (attr == "s")
            {
                ft.size = VALUE_FROM_STRING(value, float);
            }
            else if (attr == "bmp")
            {
                ft.bmpFont = value;
            }
        }
    }
    
    pThis->_fontStack.push_back(ft);
}

void MRichLabelTTF::textHandler(void *ctx, const char *s, int len)
{
    MRichLabelTTF *pThis = this;
    M_ASSERT(pThis->_fontStack.size());
    string text((char *)s, len);
    auto ft = pThis->_fontStack.back();
    pThis->_texts.push_back(make_pair(text, ft));
}

void MRichLabelTTF::endElement(void *ctx, const char *name)
{
    MRichLabelTTF *pThis = this;
    M_ASSERT(pThis->_fontStack.size());
    pThis->_fontStack.pop_back();
}

void MRichLabelTTF::reloadData()
{
    this->removeAllChildren();
    
    CCPoint pos = ccp(0, 0);
    float maxHeight = 0;
    for (auto iter : _texts)
    {
        auto text = iter.first;
        auto font = iter.second;
        CCSize size;
        if (!font.bmpFont.empty())
        {
            CCLabelBMFont *bmpFont = CCLabelBMFont::create(text.c_str(), font.bmpFont.c_str());
            size = bmpFont->getContentSize() * CC_CONTENT_SCALE_FACTOR();
            bmpFont->setAnchorPoint(ccp(0, 0.5));
            bmpFont->setPosition(pos);
            bmpFont->setScale(bmpFont->getScale() * CC_CONTENT_SCALE_FACTOR());
            addChild(bmpFont);
        }
        else
        {
            CCLabelTTF * label = CCLabelTTF::create(text.c_str(), font.family.c_str(), font.size, CCSizeZero, kCCTextAlignmentLeft);
            size = label->getContentSize();
            label->setFontFillColor(font.color);
            label->setAnchorPoint(ccp(0, 0.5));
            label->setPosition(pos);
            addChild(label);
        }
        
        pos.x += size.width;
        if (size.height > maxHeight) maxHeight = size.height;
    }
    
    CCSize size = CCSizeMake(pos.x, maxHeight);
    this->setContentSize(size);
    
    CCObject * pObj = nullptr;
    auto arr = this->getChildren();
    CCARRAY_FOREACH(arr, pObj)
    {
        auto node = (CCNode *)pObj;
        node->setPositionY(size.height / 2);
    }
    
//    CCLayerColor * bg = CCLayerColor::create(ccc4(0, 0, 255, 40), size.width, size.height);
//    bg->setAnchorPoint(ccp(0, 0));
//    bg->setPosition(ccp(0, 0));
//    this->addChild(bg);
    
}

void MRichLabelTTF::visit()
{
    parse();
    CCLayer::visit();
}


void MRichLabelTTF::enableStroke(const ccColor3B &strokeColor, float strokeSize)
{
    auto arr = getChildren();
    CCObject * pObj = nullptr;
    CCARRAY_FOREACH(arr, pObj)
    {
        auto label = dynamic_cast<CCLabelTTF *>(pObj);
        if (nullptr != label)
        {
            label->enableStroke(strokeColor, strokeSize);
        }
    }
    
    return;
}