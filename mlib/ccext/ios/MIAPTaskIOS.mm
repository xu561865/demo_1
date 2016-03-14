//
//  MIAPTaskIOS.mm
//  legend
//
//  Created by Minhao Zhang on 8/6/13.
//
//

#include "MIAPTask.h"
#include "MIAPAppStoreManager.h"

#ifdef SDK_91
#else
#ifdef CC_TARGET_OS_IPHONE


MLIB_NS_BEGIN

class MIAPAppStoreTask : public MIAPTask, public MAppStoreManagerDelegate
{
public:
    MIAPAppStoreTask();
    virtual ~MIAPAppStoreTask();
    
    virtual void buy();
    virtual void finish(bool isSuccessful);
    
    virtual void onPurchaseFailed();
    virtual void onPurchaseSucceeded(const std::string & receipt);
};

#ifndef SDK_91
std::vector<MIAPTransaction> MIAPTask::getFinishedTransactions()
{
    std::vector<MIAPTransaction> ret;
    auto transactions = [[MAppStoreManager manager] getPendingTransactions];
    for (SKPaymentTransaction * tr in transactions)
    {
        std::string transactionId = [tr.transactionIdentifier cStringUsingEncoding:NSUTF8StringEncoding];
        std::string productId = [tr.payment.productIdentifier cStringUsingEncoding:NSUTF8StringEncoding];
        std::string receipt = mlib::base64_encode(tr.transactionReceipt.bytes, tr.transactionReceipt.length);
        MIAPTransaction mytr;
        mytr.productId = productId;
        mytr.receipt = receipt;
        ret.push_back(mytr);
        [[MAppStoreManager manager] finishTransaction:tr];
    }
    return ret;
}
#endif

MIAPTask * MIAPTask::createWithProductId(const std::string &pid)
{
    auto ret = new MIAPAppStoreTask;
    ret->productId(pid);
    return ret;
}

MIAPAppStoreTask::MIAPAppStoreTask()
{
//    timeout() = 30 * 1000;
}

MIAPAppStoreTask::~MIAPAppStoreTask()
{
}

void MIAPAppStoreTask::buy()
{
    start();
    [MAppStoreManager manager].delegate = this;
    [[MAppStoreManager manager] buy:_productId];
}

void MIAPAppStoreTask::finish(bool isSuccessful)
{
    MTask::finish(isSuccessful);
    [MAppStoreManager manager].delegate = nullptr;
}

void MIAPAppStoreTask::onPurchaseFailed()
{
    finish(false);
}

void MIAPAppStoreTask::onPurchaseSucceeded(const std::string & receipt)
{
    _receipt = receipt;
    finish(true);
}

MLIB_NS_END

#endif
#endif
