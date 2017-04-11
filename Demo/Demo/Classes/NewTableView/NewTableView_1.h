//
//  NewTableView_1.h
//  test_cocos2dx
//
//  Created by xuyi on 05/04/2017.
//
//

#ifndef NewTableView_1_h
#define NewTableView_1_h

#include "CCScrollView.h"
#include "NewTableViewCell.h"

USING_NS_CC;

class NewTableView_1;

class NewTableViewDelegate : public extension::CCScrollViewDelegate
{
public:
    virtual void tableCellTouched(NewTableView_1* table, NewTableViewCell* cell) = 0;
    virtual void tableCellHighlight(NewTableView_1* table, NewTableViewCell* cell){};
    virtual void tableCellUnhighlight(NewTableView_1* table, NewTableViewCell* cell){};
    virtual void tableCellWillRecycle(NewTableView_1* table, NewTableViewCell* cell){};
    
};


class NewTableViewDataSource
{
public:
    ~NewTableViewDataSource(){};
    virtual CCSize tableCellSizeForIndex(NewTableView_1 *table, unsigned int idx) = 0;
    virtual CCSize cellSizeForTable(NewTableView_1 *table) {return CCSizeMake(0, 0);};
    virtual NewTableViewCell* tableCellAtIndex(NewTableView_1 *table, unsigned int idx) = 0;
    virtual unsigned int numberOfCellsInTableView(NewTableView_1 *table) = 0;
};

class NewTableView_1 : public extension::CCScrollView, public extension::CCScrollViewDelegate
{
public:
    static NewTableView_1 * create(NewTableViewDataSource* dataSource, CCSize size);
    NewTableView_1();
    virtual ~NewTableView_1();
    
    bool initWithViewSize(CCSize size, CCNode* container/* = NULL*/);
    
    virtual void scrollViewDidScroll(CCScrollView* view) override;
    virtual void scrollViewDidZoom(CCScrollView* view) override {}
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) override;
    
    void reloadData();
    void updateCellAtIndex(unsigned int idx);
    NewTableViewCell *cellAtIndex(unsigned int idx);
    NewTableViewDataSource* getDataSource() { return m_pDataSource; }
    void setDataSource(NewTableViewDataSource* source) { m_pDataSource = source; }
    NewTableViewDelegate* getDelegate() { return m_pTableViewDelegate; }
    void setDelegate(NewTableViewDelegate* pDelegate) { m_pTableViewDelegate = pDelegate; }
    
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
    std::set<unsigned int>* m_pIndices;
    extension::CCArrayForObjectSorting* m_pCellsUsed;
    NewTableViewCell * m_pTouchedCell;
    NewTableViewDataSource* m_pDataSource;
    NewTableViewDelegate* m_pTableViewDelegate;
    std::vector<float> m_vCellsPositions;
};

#endif /* NewTableView_1_h */
