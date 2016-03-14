//
//  MTextFieldImplViewController.h
//  legend
//
//  Created by Minhao Zhang on 5/12/13.
//
//


#import <UIKit/UIKit.h>
#include "MTextField.h"

@interface MTextFieldImplViewController : UIViewController <UITextFieldDelegate>
{
@public
    IBOutlet UILabel *lblPrompt;
    IBOutlet UITextField *txtInput;
    IBOutlet UIView *viewContainer;
    
    MTextField * owner;
}

- (IBAction)onOKTapped:(id)sender;
- (IBAction)onCancelTapped:(id)sender;

- (void)close;
@end
