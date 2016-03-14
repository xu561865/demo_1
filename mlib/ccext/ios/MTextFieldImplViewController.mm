//
//  MTextFieldImplViewController.m
//  legend
//
//  Created by Minhao Zhang on 5/12/13.
//
//

#import "MTextFieldImplViewController.h"
#import "EAGLView.h"

USING_NS_CC;
USING_NS_CC_EXT;

@interface MTextFieldImplViewController ()

@end

@implementation MTextFieldImplViewController
{
    std::string _oldText;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    [[self view] setAutoresizingMask:UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated
{
    CGRect bounds = [[[UIApplication sharedApplication] keyWindow] bounds];
    [[self view] setFrame:bounds];
    // center view container
    CGRect frame = [viewContainer frame];
    frame.origin.x = (bounds.size.width - frame.size.width) / 2.f;
    frame.origin.y = (bounds.size.height - frame.size.height) / 2.f;
    [viewContainer setFrame:frame];
    
    _oldText = owner->text();
    
    [txtInput setPlaceholder:[NSString stringWithUTF8String:owner->placeholder().c_str()]];
    [txtInput setText:[NSString stringWithUTF8String:owner->text().c_str()]];
    [lblPrompt setText:[NSString stringWithUTF8String:owner->prompt().c_str()]];
    [txtInput setSecureTextEntry:owner->isSecure()];

    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [txtInput becomeFirstResponder];
}

- (void)dealloc {
    [lblPrompt release];
    [txtInput release];
    [viewContainer release];
    [super dealloc];
}

- (IBAction)onOKTapped:(id)sender
{
    owner->text([[txtInput text] cStringUsingEncoding:NSUTF8StringEncoding]);
    [self close];
}

- (IBAction)onCancelTapped:(id)sender
{
    owner->text(_oldText);
    [self close];
}

- (void)close
{
    [txtInput resignFirstResponder];

    [self.view removeFromSuperview];
    [self autorelease];

    owner->dispatchEvent(mlib::MEvent(MTextField::EVENT_INPUT_FINISHED));
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self close];
    return YES;
}
@end

void MTextFieldImpl::show(MTextField *owner)
{
    NSString * nibName = nil;
    CCString * str = dynamic_cast<CCString *>(owner->userData()->objectForKey("nib"));
    if (str)
    {
        nibName = [NSString stringWithUTF8String:str->getCString()];
    }
    
    MTextFieldImplViewController * vc =
    [[MTextFieldImplViewController alloc] initWithNibName:nibName bundle:nil];
    
    vc->owner = owner;
    
    [[[UIApplication sharedApplication] keyWindow] addSubview:vc.view];
}
