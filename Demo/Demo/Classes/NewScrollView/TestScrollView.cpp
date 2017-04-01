//
//  TestScrollView.cpp
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#include "TestScrollView.h"
#include "LayerColor.h"
#include "NewScrollView.h"
#include "NewScrollView_1.h"
#include "NewScrollView_2.h"
#include "NewScrollView_3.h"


#define TEST_SCROLL_VIEW_SIZE_X 400
#define TEST_SCROLL_VIEW_SIZE_Y 600

#define CONTAINER_SIZE_X (TEST_SCROLL_VIEW_SIZE_X - 100)
#define CONTAINER_SIZE_Y (TEST_SCROLL_VIEW_SIZE_Y + 100)



TestScrollView * TestScrollView::create(CCSize size, ccColor4B c4)
{
    TestScrollView * p = new TestScrollView();
    if(p)
    {
        p->setContentSize(size);
        p->initWithColor(c4, size.width, size.height);
        p->m_size = size;
        
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
    CCLayerColor * scroll_container = CCLayerColor::create();
    scroll_container->initWithColor(ccc4(25, 255, 25, 255), CONTAINER_SIZE_X, CONTAINER_SIZE_Y);
    
//#define REAL_SCROLL
#ifdef REAL_SCROLL
    extension::CCScrollView * scroll = extension::CCScrollView::create(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y), scroll_container);
    scroll->setDirection(extension::kCCScrollViewDirectionVertical);
//    scroll->setBounceable(false);
//    scroll->setContainer(scroll_container);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5, 0.5));
    scroll->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(scroll);
#endif
    
//#define LAYER
#ifdef LAYER
    LayerColor * layer = LayerColor::create(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y), ccc4(0, 0, 100, 255));
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(ccp(0.5, 0.5));
    layer->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(layer);
#endif
    
//#define NEW_SCROLL
#ifdef NEW_SCROLL
    NewScrollView * scroll = NewScrollView::create(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y), ccc4(100, 0, 0, 255));
    scroll->setContainer(scroll_container);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5, 0.5));
    scroll->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(scroll);
#endif
    
//#define NEW_1
#ifdef NEW_1
    NewScrollView_1 * scroll = NewScrollView_1::create(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y), ccc4(100, 0, 0, 255));
    scroll->setContainer(scroll_container);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5, 0.5));
    scroll->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(scroll);
#endif
    
//#define NEW_2
#ifdef NEW_2
    NewScrollView_2 * scroll = NewScrollView_2::create(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y), ccc4(100, 0, 0, 255));
    scroll->setDirection(ScrollViewDirection2::kScrollViewDirectionVertical);
    scroll->setContainer(scroll_container);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5, 0.5));
    scroll->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(scroll);
#endif
    
#define NEW_3
#ifdef NEW_3
    NewScrollView_3 * scroll = NewScrollView_3::create(CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y), ccc4(100, 0, 0, 255));
    scroll->setDirection(ScrollViewDirection3::kScrollViewDirectionVertical);
    scroll->setContainer(scroll_container);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5, 0.5));
    scroll->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(scroll);
    
    NewScrollView_3DelegateNew * delegate = new NewScrollView_3DelegateNew();
    scroll->setDelegate(delegate);
#endif
    
    CCLayerColor::onEnter();
}

void TestScrollView::onExit()
{
    CCLayerColor::onExit();
}


void NewScrollView_3DelegateNew::scrollViewDidScroll(NewScrollView_3 * view)
{
    CCLOG("pos: %f, %f", view->getContainerPos().x, view->getContainerPos().y);
}

