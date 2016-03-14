//
//  MRichLabelTTF.h
//  XMLTest
//
//  Created by lishilei on 13-4-11.
//
//

#ifndef __XMLTest__MRichLabelTTF__
#define __XMLTest__MRichLabelTTF__

#include "cocos-ext.h"
#include "mlib_ccext.h"
#include <string>
#include <vector>
#include <list>

#include "platform/CCSAXParser.h"

class MRichLabelTTF : public cocos2d::CCLayer, public CCSAXDelegator
{
public:
    typedef struct {
        std::string family;
        float size;
        cocos2d::ccColor3B color;
        std::string bmpFont;
    } FontType_t;
public:
    MRichLabelTTF();
    virtual ~MRichLabelTTF();
    CREATE_FUNC(MRichLabelTTF);
    
    virtual bool init();
    void parse();

    void reloadData();
    virtual void visit();
    void enableStroke(const ccColor3B &strokeColor, float strokeSize);
public:
    MLIB_DECLARE_PROPERTY(std::string, text);
    MLIB_DECLARE_PROPERTY(std::string, fontFamily);
    MLIB_DECLARE_PROPERTY(float, fontSize);
    MLIB_DECLARE_PROPERTY(cocos2d::ccColor3B, color);
    
private:
    CCSAXParser _saxParser;
    std::vector<FontType_t> _fontStack;
    std::list<std::pair<std::string, FontType_t> > _texts;
    std::string _cleanText;
    
    virtual void startElement(void *ctx, const char *name, const char **attrs);
    virtual void endElement(void *ctx, const char *name);
    virtual void textHandler(void *ctx, const char *s, int len);
};
#endif /* defined(__XMLTest__MRichLabelTTF__) */





