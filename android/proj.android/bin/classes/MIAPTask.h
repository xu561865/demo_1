//
//  MIAPTask.h
//  legend
//
//  Created by Minhao Zhang on 8/6/13.
//
//

#ifndef __legend__MIAPTask__
#define __legend__MIAPTask__

#include "mlib_task.h"
#include "MIAPTransaction.h"
#include <vector>

MLIB_NS_BEGIN

class MIAPTask : public MTask
{
public:
    static std::vector<MIAPTransaction> getFinishedTransactions();
    static MIAPTask * createWithProductId(const std::string& pid);
    
    virtual ~MIAPTask();
    
    virtual void buy();
    
    MLIB_DECLARE_PROPERTY(std::string, productId);
    MLIB_DECLARE_PROPERTY(std::string, receipt);

protected:
    MIAPTask();
};

MLIB_NS_END
#endif /* defined(__legend__MIAPTask__) */
