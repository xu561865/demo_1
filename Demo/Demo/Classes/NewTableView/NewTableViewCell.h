//
//  NewTableViewCell.h
//  test_cocos2dx
//
//  Created by xuyi on 10/04/2017.
//
//

#ifndef NewTableViewCell_h
#define NewTableViewCell_h

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class NewTableViewCell : public extension::CCTableViewCell
{
public:
    virtual void onEnter() override;
    
    uint32_t getId() { return _idx;}
    void setId(uint32_t id) { _idx = id;}
private:
    uint32_t _idx;
};

#endif /* NewTableViewCell_h */
