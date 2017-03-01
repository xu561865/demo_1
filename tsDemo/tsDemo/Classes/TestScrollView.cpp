//
//  TestScrollView.cpp
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#include "TestScrollView.h"
#include "NewScrollView.h"

#define TEST_SCROLL_VIEW_SIZE_X 480
#define TEST_SCROLL_VIEW_SIZE_Y 800

TestScrollView * TestScrollView::create()
{
    TestScrollView * p = new TestScrollView();
    if(p)
    {
        p->autorelease();
    }
    
    return p;
}

TestScrollView::TestScrollView()
{
    
}

TestScrollView::~TestScrollView()
{
    
}

void TestScrollView::onEnter()
{
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5, 0.5));
    this->setContentSize(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y));
    this->initWithColor(ccc4(0, 100, 500, 100), TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y);
    
    NewScrollView * scroll = NewScrollView::create();
    
    CCLayerColor * scroll_container = CCLayerColor::create();
    scroll_container->setContentSize(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y));
    scroll_container->initWithColor(ccc4(0, 100, 0, 100), TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y);
    
    scroll->setContainer(scroll_container);
    scroll->setPosition(ccp(0, 0));
    this->addChild(scroll);
    
    CCLayerColor::onEnter();
}

void TestScrollView::onExit()
{
    CCLayerColor::onExit();
}
