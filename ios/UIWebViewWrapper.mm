//
// Created by gin0606 on 2014/07/29.
//


#import "UIWebViewWrapper.h"
#import "WebView.h"
#import "CCGLView.h"
#import "CCEAGLView-ios.h"
#import "CCDirector.h"

const NSTimeInterval DEFAULT_INTERVAL = 60.0;

@interface PluginUIWebViewWrapper () <UIWebViewDelegate>
@property(nonatomic, retain) UIWebView *uiWebView;
@property(nonatomic, copy) NSString *jsScheme;
@end

@implementation PluginUIWebViewWrapper {

}

+ (instancetype)webViewWrapper {
    return [[[self alloc] init] autorelease];
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.uiWebView = nil;
        self.shouldStartLoading = nullptr;
        self.didFinishLoading = nullptr;
        self.didFailLoading = nullptr;
    }
    return self;
}

- (void)dealloc {
    self.uiWebView.delegate = nil;
    [self.uiWebView removeFromSuperview];
    self.jsScheme = nil;
    [super dealloc];
}

- (void)setupWebView {
    if (!self.uiWebView) {
        self.uiWebView = [[[UIWebView alloc] init] autorelease];
        self.uiWebView.delegate = self;
        // 初期設定：背景色なし、自動リンクなし
        [self.uiWebView setOpaque:NO];
        [self.uiWebView setBackgroundColor:[UIColor clearColor]];
        self.uiWebView.dataDetectorTypes = UIDataDetectorTypeNone;
    }
    if (!self.uiWebView.superview) {
        auto view = cocos2d::Director::getInstance()->getOpenGLView();
        auto eaglview = (CCEAGLView *) view->getEAGLView();
        [eaglview addSubview:self.uiWebView];
    }
}

- (void)setVisible:(bool)visible {
    self.uiWebView.hidden = !visible;
}

- (void)setBounce:(bool)bounce {
    self.uiWebView.scrollView.bounces = bounce;
}

- (void)setVarticalScrollIndicator:(bool)indicator {
    self.uiWebView.scrollView.showsVerticalScrollIndicator = indicator;
}

- (void)setHorizontalScrollIndicator:(bool)indicator {
    self.uiWebView.scrollView.showsHorizontalScrollIndicator = indicator;
}

- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height {
    if (!self.uiWebView) {[self setupWebView];}
    CGRect newFrame = CGRectMake(x, y, width, height);
    if (!CGRectEqualToRect(self.uiWebView.frame, newFrame)) {
        self.uiWebView.frame = CGRectMake(x, y, width, height);
    }
}

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme {
    self.jsScheme = @(scheme.c_str());
}

- (void)loadData:(const std::string &)data MIMEType:(const std::string &)MIMEType textEncodingName:(const std::string &)encodingName baseURL:(const std::string &)baseURL {
    [self.uiWebView loadData:[NSData dataWithBytes:data.c_str() length:data.length()]
                    MIMEType:@(MIMEType.c_str())
            textEncodingName:@(encodingName.c_str())
                     baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL {
    [self.uiWebView loadHTMLString:@(string.c_str()) baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadUrl:(const std::string &)urlString {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url
                             cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
                             timeoutInterval:DEFAULT_INTERVAL];
    [self.uiWebView loadRequest:request];
}

- (void)loadFile:(const std::string &)filePath {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url
                             cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
                             timeoutInterval:DEFAULT_INTERVAL];
    [self.uiWebView loadRequest:request];
}

- (void)loadUrlWithHeader:(const std::string &)urlString header:(const std::map<std::string, std::string> &)header {
  if (!self.uiWebView) { [self setupWebView]; }
  NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
  NSMutableURLRequest *mutableRequest = [NSMutableURLRequest requestWithURL:url];
  for (auto itr = header.begin(); itr != header.end(); itr++) {
    [mutableRequest setValue:@((itr->second).c_str()) forHTTPHeaderField:@((itr->first).c_str())];
  }
  [self.uiWebView loadRequest:mutableRequest];
}

- (void)stopLoading {
    [self.uiWebView stopLoading];
}

- (void)reload {
    [self.uiWebView reload];
}

- (BOOL)canGoForward {
    return self.uiWebView.canGoForward;
}

- (BOOL)canGoBack {
    return self.uiWebView.canGoBack;
}

- (void)goBack {
    [self.uiWebView goBack];
}

- (void)goForward {
    [self.uiWebView goForward];
}

- (void)evaluateJS:(const std::string &)js {
    if (!self.uiWebView) {[self setupWebView];}
    [self.uiWebView stringByEvaluatingJavaScriptFromString:@(js.c_str())];
}

- (void)setScalesPageToFit:(const bool)scalesPageToFit {
    self.uiWebView.scalesPageToFit = scalesPageToFit;
}

#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *checkURL = [request.URL absoluteString];
    if (navigationType == UIWebViewNavigationTypeLinkClicked) {
        NSRange linkMatch  = [checkURL rangeOfString:@"twitter"];

        if (linkMatch.location == NSNotFound) {
            [[UIApplication sharedApplication] openURL:request.URL];
            return NO;
        }
    }
    if (self.shouldStartLoading) {
        return self.shouldStartLoading([checkURL UTF8String]);
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    if (self.didFinishLoading) {
        NSString *url = [[webView.request URL] absoluteString];
        self.didFinishLoading([url UTF8String]);
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    if (self.didFailLoading) {
        NSString *url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
        self.didFailLoading([url UTF8String]);
    }
}

- (void)setFocusable:(bool)isFocusable {
    if (isFocusable) {
        [self.uiWebView becomeFirstResponder];
    } else {
        [self.uiWebView resignFirstResponder];
    }
}

@end
