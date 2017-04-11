//
//  TestTableView.h
//  test_cocos2dx
//
//  Created by xuyi on 05/04/2017.
//
//

#ifndef TestTableView_h
#define TestTableView_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "NewTableViewCell.h"

//#define NEW_TABLE_VIEW
#ifdef NEW_TABLE_VIEW
#include "NewTableView.h"
#define TableView NewTableView
#endif

#define NEW_TABLE_VIEW_1
#include "NewTableView_1.h"
#ifdef NEW_TABLE_VIEW_1
#define TableView NewTableView_1
#endif

USING_NS_CC;

class TableView;


class NewTableViewAll : public CCLayerColor, public NewTableViewDataSource, public NewTableViewDelegate
{
public:
    static NewTableViewAll * create(ccColor4B c4);
    virtual bool init(ccColor4B c4);
    
    virtual void scrollViewDidScroll(extension::CCScrollView* view) override {};
    virtual void scrollViewDidZoom(extension::CCScrollView* view) override {}
    
    virtual void tableCellTouched(TableView* table, NewTableViewCell* cell) override;
    
    virtual CCSize tableCellSizeForIndex(TableView *table, unsigned int idx) override;
    virtual CCSize cellSizeForTable(TableView *table) override {return CCSizeMake(0, 0);};
    virtual NewTableViewCell* tableCellAtIndex(TableView *table, unsigned int idx) override;
    virtual unsigned int numberOfCellsInTableView(TableView *table) override;
};


class TestTableView : public CCLayerColor
{
public:
    TestTableView();
    virtual ~TestTableView();
    
    static TestTableView * create(CCSize, ccColor4B);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    CCSize m_size;
};



#endif /* TestTableView_h */
