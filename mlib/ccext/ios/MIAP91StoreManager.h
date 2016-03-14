//
//  MIAP91StoreManager.h
//  legend
//
//  Created by Minhao Zhang on 8/7/13.
//
//



#ifdef SDK_91

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import  <NdComPlatform/NdComPlatform.h>
#import  <NdComPlatform/NdCPNotifications.h>
#import  <NdComPlatform/NdComPlatformError.h>

@interface MIAP91StoreManager : NSObject
{
@public
    BOOL shouldShowShopAfterLogin;
}

+ (MIAP91StoreManager *)manager;

- (void)login;

@end
#endif

#endif

