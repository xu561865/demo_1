//
//  NewTableViewCell.cpp
//  test_cocos2dx
//
//  Created by xuyi on 10/04/2017.
//
//

#include "NewTableViewCell.h"

#define CELL_COLOR ccc4(100, 0, 0, 255)

void NewTableViewCell::onEnter()
{
    CCSize size = this->getContentSize();
    CCLayerColor * lay = CCLayerColor::create(CELL_COLOR, size.width, size.height - 10);
    lay->setAnchorPoint(ccp(0, 0));
    lay->setPosition(ccp(0, 0));
    this->addChild(lay);
    
    std::stringstream str;
    str << this->getId();
    CCLabelTTF* ttf = CCLabelTTF::create(str.str().c_str(), "Helvetica", 24, CCSizeMake(256, 32), kCCTextAlignmentLeft);
    ttf->setPosition(ccp(100, 50));
    ttf->setAnchorPoint(ccp(0, 0));
    this->addChild(ttf);
    
    extension::CCTableViewCell::onEnter();
}
