//
//  NewTableView_1.cpp
//  test_cocos2dx
//
//  Created by xuyi on 05/04/2017.
//
//

#include "NewTableView_1.h"

NewTableView_1 * NewTableView_1::create(NewTableViewDataSource* dataSource, CCSize size)
{
    NewTableView_1 * tableView = new NewTableView_1();
    tableView->initWithViewSize(size, nullptr);
    tableView->autorelease();
    tableView->setDataSource(dataSource);
    
    return tableView;
}

NewTableView_1::NewTableView_1()
{
    m_pTouchedCell = nullptr;
    m_pDataSource = nullptr;
    m_pTableViewDelegate = nullptr;
}

NewTableView_1::~NewTableView_1()
{
    
}

bool NewTableView_1::initWithViewSize(CCSize size, CCNode* container/* = NULL*/)
{
    if (CCScrollView::initWithViewSize(size,container))
    {
        m_pCellsUsed      = new extension::CCArrayForObjectSorting();
//        m_pCellsFreed     = new CCArrayForObjectSorting();
        m_pIndices        = new std::set<unsigned int>();
//        m_eVordering      = kCCTableViewFillBottomUp;
//        this->setDirection(kCCScrollViewDirectionVertical);
        
        CCScrollView::setDelegate(this);
        return true;
    }
    return false;
}

void NewTableView_1::reloadData()
{
    this->_updateCellPositions();
    this->_updateContentSize();
    if (m_pDataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);
    }
}

void NewTableView_1::scrollViewDidScroll(extension::CCScrollView* view)
{
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
    if(m_pTableViewDelegate != NULL)
    {
        m_pTableViewDelegate->scrollViewDidScroll(this);
    }
    
    
    for (unsigned int i = 0; i < uCountOfItems; i++)
    {
        this->updateCellAtIndex(i);
    }
}

NewTableViewCell * NewTableView_1::cellAtIndex(unsigned int idx)
{
    NewTableViewCell *found = NULL;
    
    if (m_pIndices->find(idx) != m_pIndices->end())
    {
        found = (NewTableViewCell *)m_pCellsUsed->objectWithObjectID(idx);
    }
    
    return found;
}

void NewTableView_1::updateCellAtIndex(unsigned int idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
//    extension::CCTableViewCell* cell = this->cellAtIndex(idx);
//    if (cell)
//    {
//        this->_moveCellOutOfSight(cell);
//    }
    NewTableViewCell* cell = m_pDataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}

void NewTableView_1::_addCellIfNecessary(NewTableViewCell * cell)
{
    if (cell->getParent() != this->getContainer())
    {
        this->getContainer()->addChild(cell);
    }
    m_pCellsUsed->insertSortedObject(cell);
    m_pIndices->insert(cell->getIdx());
    // [m_pIndices addIndex:cell.idx];
}

void NewTableView_1::_setIndexForCell(unsigned int index, NewTableViewCell *cell)
{
    cell->setAnchorPoint(ccp(0.0f, 0.0f));
    cell->setPosition(this->_offsetFromIndex(index));
    cell->setIdx(index);
}

CCPoint NewTableView_1::_offsetFromIndex(unsigned int index)
{
    CCPoint offset = this->__offsetFromIndex(index);
    
    const CCSize cellSize = m_pDataSource->tableCellSizeForIndex(this, index);
//    if (m_eVordering == kCCTableViewFillTopDown)
//    {
//        offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
//    }
    return offset;
}

CCPoint NewTableView_1::__offsetFromIndex(unsigned int index)
{
    CCPoint offset;
    CCSize  cellSize;
    
    switch (this->getDirection())
    {
        case extension::kCCScrollViewDirectionHorizontal:
            offset = ccp(m_vCellsPositions[index], 0.0f);
            break;
        default:
            offset = ccp(0.0f, m_vCellsPositions[index]);
            break;
    }
    
    return offset;
}

unsigned int NewTableView_1::_indexFromOffset(CCPoint offset)
{
    int index = 0;
    const int maxIdx = m_pDataSource->numberOfCellsInTableView(this) - 1;
    
    index = this->__indexFromOffset(offset);
    if (index != -1)
    {
        index = MAX(0, index);
        if (index > maxIdx)
        {
            index = CC_INVALID_INDEX;
        }
    }
    
    return index;
}

int NewTableView_1::__indexFromOffset(CCPoint offset)
{
    int low = 0;
    int high = m_pDataSource->numberOfCellsInTableView(this) - 1;
    float search;
    switch (this->getDirection())
    {
        case extension::kCCScrollViewDirectionHorizontal:
            search = offset.x;
            break;
        default:
            search = offset.y;
            break;
    }
    
    while (high >= low)
    {
        int index = low + (high - low) / 2;
        float cellStart = m_vCellsPositions[index];
        float cellEnd = m_vCellsPositions[index + 1];
        
        if (search >= cellStart && search <= cellEnd)
        {
            return index;
        }
        else if (search < cellStart)
        {
            high = index - 1;
        }
        else
        {
            low = index + 1;
        }
    }
    
    if (low <= 0)
    {
        return 0;
    }
    
    return -1;
}

void NewTableView_1::_updateCellPositions()
{
    int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
    m_vCellsPositions.resize(cellsCount + 1, 0.0);
    
    if (cellsCount > 0)
    {
        float currentPos = 0;
        CCSize cellSize;
        for (int i=0; i < cellsCount; i++)
        {
            m_vCellsPositions[i] = currentPos;
            cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
            switch (this->getDirection())
            {
                case extension::kCCScrollViewDirectionHorizontal:
                    currentPos += cellSize.width;
                    break;
                default:
                    currentPos += cellSize.height;
                    break;
            }
        }
        m_vCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
    }
}

void NewTableView_1::_updateContentSize()
{
    CCSize size = CCSizeZero;
    unsigned int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
    
    if (cellsCount > 0)
    {
        float maxPosition = m_vCellsPositions[cellsCount];
        
        switch (this->getDirection())
        {
            case extension::kCCScrollViewDirectionHorizontal:
                size = CCSizeMake(maxPosition, m_tViewSize.height);
                break;
            default:
                size = CCSizeMake(m_tViewSize.width, maxPosition);
                break;
        }
    }
    
    this->setContentSize(size);
    
//    if (m_eOldDirection != m_eDirection)
//    {
        if (m_eDirection == extension::kCCScrollViewDirectionHorizontal)
        {
            this->setContentOffset(ccp(0,0));
        }
        else
        {
            this->setContentOffset(ccp(0,this->minContainerOffset().y));
        }
//        m_eOldDirection = m_eDirection;
//    }
    
}

bool NewTableView_1::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    bool touchResult = CCScrollView::ccTouchBegan(pTouch, pEvent);
    
    if(m_pTouches->count() == 1)
    {
        unsigned int        index;
        CCPoint           point;
        
        point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        
        index = this->_indexFromOffset(point);
        if (index == CC_INVALID_INDEX)
        {
            m_pTouchedCell = NULL;
        }
        else
        {
            m_pTouchedCell  = this->cellAtIndex(index);
        }
        
        if (m_pTouchedCell && m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellHighlight(this, m_pTouchedCell);
        }
    }
    else if(m_pTouchedCell)
    {
        if(m_pTableViewDelegate != NULL)
        {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
    
    return touchResult;
}

void NewTableView_1::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (m_pTouchedCell && isTouchMoved())
    {
        if(m_pTableViewDelegate != NULL)
        {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
}

void NewTableView_1::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!this->isVisible())
    {
        return;
    }
    
    if (m_pTouchedCell)
    {
        CCRect bb = this->boundingBox();
        bb.origin = m_pParent->convertToWorldSpace(bb.origin);
        
        if (bb.containsPoint(pTouch->getLocation()) && m_pTableViewDelegate != NULL)
        {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
            m_pTableViewDelegate->tableCellTouched(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
    
    CCScrollView::ccTouchEnded(pTouch, pEvent);
}

void NewTableView_1::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchCancelled(pTouch, pEvent);
    
    if (m_pTouchedCell)
    {
        if(m_pTableViewDelegate != NULL)
        {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
}

