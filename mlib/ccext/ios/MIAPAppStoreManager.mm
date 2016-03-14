//
//  MIAPAppStoreManager.m
//  legend
//
//  Created by Minhao Zhang on 8/7/13.
//
//

#import "MIAPAppStoreManager.h"
#include <map>

using namespace std;
map<string, SKProduct *> g_products;
std::recursive_mutex g_mutex;

@implementation MAppStoreManager
{
    NSMutableArray * _pendingTransactions;
}

+ (void)load
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:[self manager]];
}

+ (MAppStoreManager *)manager
{
    static MAppStoreManager * inst = nil;
    if (inst == nil)
    {
        inst = [[self alloc] init];
    }
    return inst;
}

- (id)init
{
    if (self = [super init])
    {
        _pendingTransactions = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc
{
    [_pendingTransactions release];
    [super dealloc];
}

- (void)load:(const std::string &)productId
{
    std::lock_guard<decltype(g_mutex)> guard(g_mutex);
    if (g_products.find(productId) != g_products.end())
    {
        [self pay:productId];
        return;
    }
    
    NSMutableSet * productIdentifiers = [[[NSMutableSet alloc] init] autorelease];
    
    [productIdentifiers addObject:[NSString stringWithUTF8String:productId.c_str()]];
    
    auto req = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    req.delegate = self;
    [req start];
}

- (void)buy:(const std::string &)productId
{
    std::lock_guard<decltype(g_mutex)> guard(g_mutex);
    if (g_products.find(productId) != g_products.end())
    {
        [self pay:productId];
        return;
    }
    
    [self load:productId];
}

- (void)pay:(const std::string &)productId
{
    std::lock_guard<decltype(g_mutex)> guard(g_mutex);
    SKProduct * product = g_products[productId];
    SKMutablePayment * payment = [SKMutablePayment paymentWithProduct:product];
    payment.quantity = 1;
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (NSArray *)getPendingTransactions
{
    return [NSArray arrayWithArray:_pendingTransactions];
}

- (void)finishTransaction:(SKPaymentTransaction *)tr
{
    [[SKPaymentQueue defaultQueue] finishTransaction:tr];
    [_pendingTransactions removeObject:tr];
}

#pragma mark SKProductsRequestDelegate

- (void)requestDidFinish:(SKRequest *)request
{
    M_INFO("products request finished!");
    [request release];
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    M_ERROR("products request failed! error = " << [error.description UTF8String]);
    [request release];
    if (_delegate)
    {
        _delegate->onPurchaseFailed();
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    std::lock_guard<decltype(g_mutex)> guard(g_mutex);
    M_DEBUG("products loaded, size = " << response.products.count);

    if (response.products.count == 0)
    {
        M_DEBUG("to call delegate ? _del = " << (long)_delegate);
        if (_delegate)
        {
            _delegate->onPurchaseFailed();
        }
    }
    M_DEBUG("products loaded");
    for (SKProduct * p in response.products)
    {
        [p retain];
        std::string productId = [p.productIdentifier cStringUsingEncoding:NSUTF8StringEncoding];
        M_DEBUG("product " << productId << " loaded");

        g_products[productId] = p;
        [self pay:productId];
    }

}
#pragma mark SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
    
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    NSLog(@"Received updated transactions: %@", transactions);
    for (SKPaymentTransaction *transaction in transactions)
    {
        std::string productId = [transaction.payment.productIdentifier cStringUsingEncoding:NSUTF8StringEncoding];
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStateFailed:
                NSLog(@"transaction failed, id=%@, error=%@", transaction.transactionIdentifier, transaction.error);
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                if (_delegate)
                {
                    _delegate->onPurchaseFailed();
                }
                break;
                
            case SKPaymentTransactionStatePurchasing:
                NSLog(@"Transaction purchasing, id=%@", transaction.transactionIdentifier);
                break;
                
            case SKPaymentTransactionStatePurchased:
            {
                NSLog(@"Transaction purchased, id=%@", transaction.transactionIdentifier);
                if (_delegate)
                {
                    std::string receipt = mlib::base64_encode(transaction.transactionReceipt.bytes, transaction.transactionReceipt.length);
                    _delegate->onPurchaseSucceeded(receipt);
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                }
                else
                {
                    [_pendingTransactions addObject:transaction];
                }
            }
                break;
                
            case SKPaymentTransactionStateRestored:
                NSLog(@"Transaction restored, id=%@", transaction.transactionIdentifier);
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                break;
        }
    }
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    
}

@end
