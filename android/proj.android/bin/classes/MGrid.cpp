//
//  MGrid.cpp
//  legend
//
//  Created by Xu Jinyang on 4/10/13.
//
//

#include "MGrid.h"

MLIB_NS_BEGIN

MLIB_REGISTER_CCBLOADER(MGrid);

USING_NS_CC;
USING_NS_CC_EXT;


MGrid::MGrid()
{
    _nodes = nullptr;
    _padding = 5;
    _gapHorizontal =
    _gapVertical = 5;
}

MGrid::~MGrid()
{
    nodes(nullptr);
}

void MGrid::nodes(cocos2d::CCArray *anodes)
{
    MLIB_IMPL_RETAIN_PROPERTY_SETTER(nodes);
    
    if (_nodes != nullptr)
    {
        reloadData();
    }
}

void MGrid::onEnter()
{
    CCLayer::onEnter();
    
//    for (int i =0; i < 10000; i++)
//    {
//        reloadData();
//    }
}

void MGrid::reloadData()
{
    removeAllChildren();
    
    CCSize size = getContentSize();
    CCPoint pt = ccp(padding(), size.height - padding());
    float nextY = pt.y;
    bool isFirst = true;
    
    CCObject *obj;
    CCARRAY_FOREACH(_nodes, obj)
    {
        CCNode *node = (CCNode *)obj;
        CCSize nodeSize = node->getContentSize();
        nodeSize.width *= node->getScaleX();
        nodeSize.height *= node->getScaleY();
        
        if (!isFirst)
        {
            pt.x += gapHorizontal();
            if (pt.x + nodeSize.width > size.width - padding())
            {
                // next row
                pt.x = padding();
                pt.y = nextY - gapVertical();
            }
        }
        
        CCPoint ap = node->getAnchorPoint();
        CCPoint pos = ccpAdd(pt, ccp(nodeSize.width * ap.x, nodeSize.height * (ap.y - 1)));
        
        node->setPosition(pos);
        addChild(node);
        isFirst = false;
        
        pt.x += nodeSize.width;
        if (nextY > pt.y - nodeSize.height)
        {
            nextY = pt.y - nodeSize.height;
        }
        
        MEventDispatcher *disp = dynamic_cast<MEventDispatcher *>(node);
        if (disp != nullptr)
        {
            disp->parentDispatcher(this);
        }
    }
}

MLIB_NS_END