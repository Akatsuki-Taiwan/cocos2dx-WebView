//
// Created by gin0606 on 2014/07/30.
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "WebViewImpl_iOS.h"
#import "UIWebViewWrapper.h"
#include "renderer/CCRenderer.h"
#include "WebView.h"
#include "CCDirector.h"
#include "CCGLView.h"
#include "CCEAGLView-ios.h"
#include "platform/CCFileUtils.h"

namespace cocos2d {
namespace plugin {

WebViewImpl::WebViewImpl(WebView *webView)
        : _uiWebViewWrapper([PluginUIWebViewWrapper webViewWrapper]), _webView(webView) {
    [_uiWebViewWrapper retain];
    _uiWebViewWrapper.shouldStartLoading = [this](std::string url) {
        if (this->_webView->shouldStartLoading) {
            return this->_webView->shouldStartLoading(this->_webView, url);
        }
        return true;
    };
    _uiWebViewWrapper.didFinishLoading = [this](std::string url) {
        if (this->_webView->didFinishLoading) {
            this->_webView->didFinishLoading(this->_webView, url);
        }
    };
    _uiWebViewWrapper.didFailLoading = [this](std::string url) {
        if (this->_webView->didFailLoading) {
            this->_webView->didFailLoading(this->_webView, url);
        }
    };
    _uiWebViewWrapper.onJsCallback = [this](std::string url) {
        if (this->_webView->onJsCallback) {
            this->_webView->onJsCallback(this->_webView, url);
        }
    };
}

WebViewImpl::~WebViewImpl() {
    [_uiWebViewWrapper release];
    _uiWebViewWrapper = nullptr;
}

void WebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
    [_uiWebViewWrapper setJavascriptInterfaceScheme:scheme];
}

void WebViewImpl::loadData(const Data &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    std::string dataString(reinterpret_cast<char *>(data.getBytes()), static_cast<unsigned int>(data.getSize()));
    [_uiWebViewWrapper loadData:dataString MIMEType:MIMEType textEncodingName:encoding baseURL:baseURL];
}

void WebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) {
    [_uiWebViewWrapper loadHTMLString:string baseURL:baseURL];
}

void WebViewImpl::loadUrl(const std::string &url) {
    [_uiWebViewWrapper loadUrl:url];
}

void WebViewImpl::loadFile(const std::string &fileName) {
    auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    [_uiWebViewWrapper loadFile:fullPath];
}

 void WebViewImpl::loadUrlWithHeader(const std::string &url, const std::map<std::string, std::string> &header) {
   [_uiWebViewWrapper loadUrlWithHeader:url header:header];
 }

void WebViewImpl::stopLoading() {
    [_uiWebViewWrapper stopLoading];
}

void WebViewImpl::reload() {
    [_uiWebViewWrapper reload];
}

bool WebViewImpl::canGoBack() {
    return _uiWebViewWrapper.canGoBack;
}

bool WebViewImpl::canGoForward() {
    return _uiWebViewWrapper.canGoForward;
}

void WebViewImpl::goBack() {
    [_uiWebViewWrapper goBack];
}

void WebViewImpl::goForward() {
    [_uiWebViewWrapper goForward];
}

void WebViewImpl::evaluateJS(const std::string &js) {
    [_uiWebViewWrapper evaluateJS:js];
}

void WebViewImpl::setScalesPageToFit(const bool scalesPageToFit) {
    [_uiWebViewWrapper setScalesPageToFit:scalesPageToFit];
}

void WebViewImpl::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags) {
    if (flags & cocos2d::Node::FLAGS_TRANSFORM_DIRTY) {
        auto direcrot = cocos2d::Director::getInstance();
        auto glView = direcrot->getOpenGLView();
        auto frameSize = glView->getFrameSize();
        auto scaleFactor = [static_cast<CCEAGLView *>(glView->getEAGLView()) contentScaleFactor];

        auto winSize = direcrot->getWinSize();

        auto leftBottom = this->_webView->convertToWorldSpace(cocos2d::Vec2::ZERO);
        auto rightTop = this->_webView->convertToWorldSpace(cocos2d::Vec2(this->_webView->getContentSize().width, this->_webView->getContentSize().height));

        auto x = (frameSize.width / 2 + (leftBottom.x - winSize.width / 2) * glView->getScaleX()) / scaleFactor;
        auto y = (frameSize.height / 2 - (rightTop.y - winSize.height / 2) * glView->getScaleY()) / scaleFactor;
        auto width = (rightTop.x - leftBottom.x) * glView->getScaleX() / scaleFactor;
        auto height = (rightTop.y - leftBottom.y) * glView->getScaleY() / scaleFactor;

        [_uiWebViewWrapper setFrameWithX:x
                                      y:y
                                  width:width
                                 height:height];
    }
}

void WebViewImpl::setVisible(bool visible) {
    [_uiWebViewWrapper setVisible:visible];
}

void WebViewImpl::setBounce(bool bounce) {
    [_uiWebViewWrapper setBounce:bounce];
}

void WebViewImpl::setVerticalScrollIndicator(bool indicator) {
    [_uiWebViewWrapper setVarticalScrollIndicator:indicator];
}

void WebViewImpl::setHorizontalScrollIndicator(bool indicator) {
    [_uiWebViewWrapper setHorizontalScrollIndicator:indicator];
}

void WebViewImpl::setFocusable(bool isFocusable) {
    [_uiWebViewWrapper setFocusable:isFocusable];
}
} // namespace cocos2d
} // namespace plugin

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS
