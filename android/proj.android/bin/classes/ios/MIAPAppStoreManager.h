//
//  MIAPAppStoreManager.h
//  legend
//
//  Created by Minhao Zhang on 8/7/13.
//
//

#import <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#import <string>

class MAppStoreManagerDelegate
{
public:
    virtual void onPurchaseFailed() = 0;
    virtual void onPurchaseSucceeded(const std::string & receipt) = 0;
};

@interface MAppStoreManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
}

@property (nonatomic) MAppStoreManagerDelegate * delegate;

+ (MAppStoreManager *)manager;

- (void)buy:(const std::string&)productId;

- (void)load:(const std::string&)productId;
- (void)pay:(const std::string&)productId;

- (NSArray *)getPendingTransactions;
- (void)finishTransaction:(SKPaymentTransaction *)tr;

@end
