//
//  NewTableView.h
//  test_cocos2dx
//
//  Created by xuyi on 05/04/2017.
//
//

#ifndef NewTableView_h
#define NewTableView_h

#include "CCScrollView.h"
#include "NewTableViewCell.h"

USING_NS_CC;

class NewTableView;

class NewTableViewDelegate : public extension::CCScrollViewDelegate
{
public:
    virtual void tableCellTouched(NewTableView* table, NewTableViewCell* cell) = 0;
    virtual void tableCellHighlight(NewTableView* table, NewTableViewCell* cell){};
    virtual void tableCellUnhighlight(NewTableView* table, NewTableViewCell* cell){};
    virtual void tableCellWillRecycle(NewTableView* table, NewTableViewCell* cell){};
    
};


class NewTableViewDataSource
{
public:
    ~NewTableViewDataSource(){};
    virtual CCSize tableCellSizeForIndex(NewTableView *table, unsigned int idx) = 0;
    virtual CCSize cellSizeForTable(NewTableView *table) {return CCSizeMake(0, 0);};
    virtual NewTableViewCell* tableCellAtIndex(NewTableView *table, unsigned int idx) = 0;
    virtual unsigned int numberOfCellsInTableView(NewTableView *table) = 0;
};

class NewTableView : public extension::CCScrollView, public extension::CCScrollViewDelegate
{
public:
    static NewTableView * create(NewTableViewDataSource* dataSource, CCSize size);
    NewTableView();
    virtual ~NewTableView();
    
    bool initWithViewSize(CCSize size, CCNode* container/* = NULL*/);
    
    virtual void scrollViewDidScroll(CCScrollView* view) override;
    virtual void scrollViewDidZoom(CCScrollView* view) override {}
    
    void reloadData();
    void updateCellAtIndex(unsigned int idx);
    NewTableViewDataSource* getDataSource() { return m_pDataSource; }
    void setDataSource(NewTableViewDataSource* source) { m_pDataSource = source; }
    
protected:
    unsigned int _indexFromOffset(CCPoint offset);
    int __indexFromOffset(CCPoint offset);
    void _updateCellPositions();
    void _updateContentSize();
    void _setIndexForCell(unsigned int index, NewTableViewCell *cell);
    CCPoint _offsetFromIndex(unsigned int index);
    CCPoint __offsetFromIndex(unsigned int index);
    void _addCellIfNecessary(NewTableViewCell * cell);
    
protected:
    NewTableViewDataSource* m_pDataSource;
    NewTableViewDelegate* m_pTableViewDelegate;
    std::vector<float> m_vCellsPositions;
};

#endif /* NewTableView_h */
