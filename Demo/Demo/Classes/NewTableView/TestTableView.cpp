//
//  TestTableView.cpp
//  test_cocos2dx
//
//  Created by xuyi on 05/04/2017.
//
//

#include "TestTableView.h"
#include "NewTableView.h"

#define TEST_SCROLL_VIEW_SIZE_X 400
#define TEST_SCROLL_VIEW_SIZE_Y 600

#define CONTAINER_SIZE_X (TEST_SCROLL_VIEW_SIZE_X)
#define CONTAINER_SIZE_Y (TEST_SCROLL_VIEW_SIZE_Y + 100)

#define CELL_SIZE_X (TEST_SCROLL_VIEW_SIZE_X)
#define CELL_SIZE_Y (100)

#define CEL_NUM (20)

TestTableView * TestTableView::create(CCSize size, ccColor4B c4)
{
    TestTableView * p = new TestTableView();
    if(p)
    {
        p->setContentSize(size);
        p->initWithColor(c4, size.width, size.height);
        p->m_size = size;
        
        p->autorelease();
    }
    
    return p;
}

TestTableView::TestTableView()
{
    
}

TestTableView::~TestTableView()
{
    
}

void TestTableView::onEnter()
{
//#define REAL_TABLE_VIEW
#ifdef REAL_TABLE_VIEW
    
    CCLayer * pContainer = new CCLayer();
    
    extension::CCTableView* tableView = extension::CCTableView::create(pNewTableViewDataSource, CCSizeMake(640, 960), pContainer);
    tableView->setDirection(extension::kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0.5, 0.5));
    tableView->setPosition(ccp(320, 480));
    tableView->setDelegate(pNewTableViewDelegate);
    this->addChild(tableView);
    tableView->reloadData();
#endif
    
#define NEW_TABLE_VIEW
#ifdef NEW_TABLE_VIEW
    
    NewTableViewAll * scroll = NewTableViewAll::create(ccc4(0, 100, 0, 255));
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(ccp(0.5, 0.5));
    scroll->setPosition(ccp(m_size.width / 2, m_size.height / 2));
    this->addChild(scroll);
    
#endif
    
    CCLayerColor::onEnter();
}

void TestTableView::onExit()
{
    CCLayerColor::onExit();
}


/*
 * NewTableView
 */

NewTableViewAll * NewTableViewAll::create(ccColor4B c4)
{
    NewTableViewAll * p = new NewTableViewAll();
    if(p && p->init(c4))
    {
        p->autorelease();
        return p;
    }
    else
    {
        return nullptr;
    }
}

bool NewTableViewAll::init(ccColor4B c4)
{
    if (!initWithColor(c4, TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y))
    {
        return false;
    }
    
    NewTableView* tableView = NewTableView::create(this, CCSizeMake(TEST_SCROLL_VIEW_SIZE_X, TEST_SCROLL_VIEW_SIZE_Y));
    tableView->setDirection(extension::kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(0, 0));
    tableView->setDelegate(this);
//    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView);
    tableView->reloadData();
    
    return true;
}

void NewTableViewAll::tableCellTouched(NewTableView* table, NewTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize NewTableViewAll::tableCellSizeForIndex(NewTableView *table, unsigned int idx)
{
    return CCSizeMake(CELL_SIZE_X, CELL_SIZE_Y);
}

NewTableViewCell* NewTableViewAll::tableCellAtIndex(NewTableView *table, unsigned int idx)
{
    NewTableViewCell *cell = new NewTableViewCell();
    cell->setContentSize(CCSizeMake(CELL_SIZE_X, CELL_SIZE_Y));
    
    cell->setId(idx);
    
    return cell;
}

unsigned int NewTableViewAll::numberOfCellsInTableView(NewTableView *table)
{
    return CEL_NUM;
}


