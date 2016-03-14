//
//  MIAP91StoreManager.m
//  legend
//
//  Created by Minhao Zhang on 8/7/13.
//
//

#import "MIAP91StoreManager.h"
#import "MIAPTask.h"
#import "MIAPShop.h"

#ifdef SDK_91

@implementation MIAP91StoreManager

+ (void)load
{
    NdInitConfigure * cfg = [[[NdInitConfigure alloc] init] autorelease];
    cfg.appid = 104509;
    cfg.appKey = @"df0154469c731619d3bcd67ca6ce061140f9dede03622483";
    [[NdComPlatform defaultPlatform] NdInit:cfg];
    
    
    [[NdComPlatform defaultPlatform] NdSetDebugMode:0];
    
    [[NSNotificationCenter defaultCenter] addObserver:[self manager]
                                             selector:@selector(onNDInitFinished:)
                                                 name:kNdCPInitDidFinishNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:[self manager]
                                             selector:@selector(onNdLoginFinished:)
                                                 name:kNdCPLoginNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:[self manager]
                                             selector:@selector(onNdBuyResultGot:)
                                                 name:kNdCPBuyResultNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:[self manager]
                                             selector:@selector(onNdPlatformLeft:)
                                                 name:kNdCPLeavePlatformNotification
                                               object:nil];
}

+ (MIAP91StoreManager *)manager
{
    static MIAP91StoreManager * inst = nil;
    if (inst == nil)
    {
        inst = [[self alloc] init];
    }
    return inst;
}

- (void)login
{
    [[NdComPlatform defaultPlatform] NdLogin:0];
}

- (void)onNDInitFinished:(NSNotification *)notif
{
    NSLog(@"HELLO, finished: %@", notif);
    
    //    [[NdComPlatform defaultPlatform] NdLoginEx:0];
    //    [[NdComPlatform defaultPlatform] NdLogin:0];
}

- (void)onNdLoginFinished:(NSNotification *)notif
{
    NSDictionary *dict = [notif userInfo ];
    BOOL success = [[dict objectForKey:@"result"] boolValue];
    NdGuestAccountStatus* guestStatus = (NdGuestAccountStatus*)[dict objectForKey:@"NdGuestAccountStatus" ];
    //登录成功后处理
    if([[ NdComPlatform  defaultPlatform] isLogined] && success) {
        //也可以通过[[NdComPlatform defaultPlatform] getCurrentLoginState]判断是否游客登录状态
        NSString * strTip = nil;
        if (guestStatus) {
            if ([guestStatus isGuestLogined]) {
                //游客账号登录成功;
                strTip = @"login succeeded as guest";
            }
            else if ([guestStatus isGuestRegistered]) {
                //游客成功注册为普通账号
                strTip = @"login succeeded as guest (reged)";
            }
        }
        else  {
            //普通账号登录成功！
            strTip = @"login succeeded as normal account";
            
            if (shouldShowShopAfterLogin)
            {
                mlib::MIAPShop::sharedInstance()->show();
            }
            shouldShowShopAfterLogin = NO;
        }
#ifdef DEBUG
        [[[[UIAlertView alloc] initWithTitle:@"" message:strTip delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease] show];
#else
        NSLog(@"login ok result: %@", strTip);
#endif
        
    }
    //登录失败处理和相应提示
    else {
        int  error = [[dict objectForKey:@"error" ] intValue ];
        NSString * strTip = [NSString  stringWithFormat:@"登录失败, error=%d",
                             error];
        switch (error) {
            case ND_COM_PLATFORM_ERROR_USER_CANCEL ://用户取消登录
                if (([[NdComPlatform  defaultPlatform ] getCurrentLoginState] ==
                     ND_LOGIN_STATE_GUEST_LOGIN)) {
                    strTip =   @"当前仍处于游客登录状态";
                }
                else {
                    strTip = @"用户未登录";
                }
                break;
            case ND_COM_PLATFORM_ERROR_APP_KEY_INVALID://appId未授权接入, 或 appKey 无效
                strTip = @"登录失败, 请检查appId/appKey" ;
                break;
            case ND_COM_PLATFORM_ERROR_CLIENT_APP_ID_INVALID ://无效的应用ID
                strTip = @"登录失败, 无效的应用ID" ;
                break;
            case ND_COM_PLATFORM_ERROR_HAS_ASSOCIATE_91 :
                strTip = @"有关联的91账号，不能以游客方式登录";
                break;
            default:
                //其他类型的错误提示
                break;
        }
        
#ifdef DEBUG
        [[[[UIAlertView alloc] initWithTitle:@"" message:strTip delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease] show];
#else
        NSLog(@"login error result: %@", strTip);
#endif
    }
}

- (void)onNdBuyResultGot:(NSNotification *)notif
{
    NSDictionary * dict = [notif userInfo];
    BOOL isSuccess = [[dict objectForKey:@"result"] boolValue];
    int errCode = [[dict objectForKey:@"error"] intValue];
    NSLog(@"Purchase success = %@, error code = %d", isSuccess ? @"true" : @"false", errCode);
    
    if (isSuccess)
    {
        NSLog(@"info: %@", dict);
        auto buyInfo = (NdBuyInfo *)([dict objectForKey:@"buyInfo"]);
        std::string trid = [buyInfo.cooOrderSerial cStringUsingEncoding:NSUTF8StringEncoding];
        mlib::MEvent evt(mlib::EVENT_IAP_SHOP_ITEM_BOUGHT, &trid);
        mlib::MIAPShop::sharedInstance()->dispatchEvent(evt);
    }
    else
    {
        mlib::MIAPShop::sharedInstance()->dispatchEvent(mlib::EVENT_IAP_SHOP_ITEM_PURCHASE_FAILED);
    }
}

- (void)onNdPlatformLeft:(NSNotification *)notif
{
    NSLog(@"Leaving 91 shop page");
    mlib::MIAPShop::sharedInstance()->dispatchEvent(mlib::EVENT_IAP_SHOP_CLOSED);
}

@end

MLIB_NS_BEGIN
std::vector<MIAPTransaction> MIAPTask::getFinishedTransactions()
{
    std::vector<MIAPTransaction> ret;
    return ret;
}

void MIAPShop::show()
{
    if (![[NdComPlatform defaultPlatform] isLogined])
    {
        [MIAP91StoreManager manager]->shouldShowShopAfterLogin = YES;
        [[MIAP91StoreManager manager] login];
        return;
    }
    
    int  nFeeType =  ND_VG_FEE_TYPE_POSSESS    |   ND_VG_FEE_TYPE_SUBSCRIBE   |   ND_VG_FEE_TYPE_CONSUME;
    [[ NdComPlatform  defaultPlatform]  NdEnterVirtualShop:nil     feeType:nFeeType];

}

MLIB_NS_END

#endif