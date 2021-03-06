//
// Created by gin0606 on 2014/07/30.
//

#include "WebViewImpl_android.h"
#include "WebView.h"
#include "org_cocos2dx_lib_webview_Cocos2dxWebViewHelper.h"
#include "jni/JniHelper.h"
#include "CCGLView.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventType.h"
#include "base/CCEventListenerCustom.h"
#include "platform/CCFileUtils.h"
#include <unordered_map>

#define CLASS_NAME "org/cocos2dx/lib/webview/Cocos2dxWebViewHelper"

namespace {
int createWebViewJNI() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createWebView", "()I")) {
        jint viewTag = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return viewTag;
    }
    return -1;
}

void removeWebViewJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "removeWebView", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void restartWebViewJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "restartWebView", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setWebViewRectJNI(const int index, const int left, const int top, const int width, const int height) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setWebViewRect", "(IIIII)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, left, top, width, height);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setJavascriptInterfaceSchemeJNI(const int index, const std::string &scheme) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setJavascriptInterfaceScheme", "(ILjava/lang/String;)V")) {
        jstring jScheme = t.env->NewStringUTF(scheme.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jScheme);

        t.env->DeleteLocalRef(jScheme);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadDataJNI(const int index, const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadData", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring jData = t.env->NewStringUTF(data.c_str());
        jstring jMIMEType = t.env->NewStringUTF(MIMEType.c_str());
        jstring jEncoding = t.env->NewStringUTF(encoding.c_str());
        jstring jBaseURL = t.env->NewStringUTF(baseURL.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jData, jMIMEType, jEncoding, jBaseURL);

        t.env->DeleteLocalRef(jData);
        t.env->DeleteLocalRef(jMIMEType);
        t.env->DeleteLocalRef(jEncoding);
        t.env->DeleteLocalRef(jBaseURL);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadHTMLStringJNI(const int index, const std::string &string, const std::string &baseURL) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadHTMLString", "(ILjava/lang/String;Ljava/lang/String;)V")) {
        jstring jString = t.env->NewStringUTF(string.c_str());
        jstring jBaseURL = t.env->NewStringUTF(baseURL.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jString, jBaseURL);

        t.env->DeleteLocalRef(jString);
        t.env->DeleteLocalRef(jBaseURL);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadUrlJNI(const int index, const std::string &url) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadUrl", "(ILjava/lang/String;)V")) {
        jstring jUrl = t.env->NewStringUTF(url.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jUrl);

        t.env->DeleteLocalRef(jUrl);
        t.env->DeleteLocalRef(t.classID);
    }
}

void loadFileJNI(const int index, const std::string &filePath) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "loadFile", "(ILjava/lang/String;)V")) {
        jstring jFilePath = t.env->NewStringUTF(filePath.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jFilePath);

        t.env->DeleteLocalRef(jFilePath);
        t.env->DeleteLocalRef(t.classID);
    }
}

  void loadUrlWithHeaderJNI(const int index, const std::string &url, const std::map<std::string, std::string> &header)
  {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t,
                                                CLASS_NAME,
                                                "loadUrlWithHeader",
                                                "(ILjava/lang/String;[[Ljava/lang/String;)V")) {
      jstring jUrl = t.env->NewStringUTF(url.c_str());
      auto length = header.size();
      jobjectArray jHeader = t.env->NewObjectArray(length,
                                                   t.env->FindClass("[Ljava/lang/String;"),
                                                   NULL);

      for (auto itr = header.begin(); itr != header.end(); itr++) {
        jobjectArray pair = t.env->NewObjectArray(2,
                                                  t.env->FindClass("java/lang/String"),
                                                  NULL);
        t.env->SetObjectArrayElement(pair,
                                     0,
                                     t.env->NewStringUTF((itr->first).c_str()));
        t.env->SetObjectArrayElement(pair,
                                     1,
                                     t.env->NewStringUTF((itr->second).c_str()));
        t.env->SetObjectArrayElement(jHeader,
                                     std::distance(header.begin(), itr),
                                     pair);
      }

      t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jUrl, jHeader);
      t.env->DeleteLocalRef(jUrl);
      t.env->DeleteLocalRef(jHeader);
      t.env->DeleteLocalRef(t.classID);
    }
  }

void stopLoadingJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopLoading", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void reloadJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "reload", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

bool canGoBackJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "canGoBack", "(I)Z")) {
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

bool canGoForwardJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "canGoForward", "(I)Z")) {
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
    return false;
}

void goBackJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "goBack", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void goForwardJNI(const int index) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "goForward", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index);
        t.env->DeleteLocalRef(t.classID);
    }
}

void evaluateJSJNI(const int index, const std::string &js) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "evaluateJS", "(ILjava/lang/String;)V")) {
        jstring jjs = t.env->NewStringUTF(js.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, jjs);

        t.env->DeleteLocalRef(jjs);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setScalesPageToFitJNI(const int index, const bool scalesPageToFit) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setScalesPageToFit", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, scalesPageToFit);
        t.env->DeleteLocalRef(t.classID);
  }
}

void setWebViewVisibleJNI(const int index, const bool visible) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setVisible", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, visible);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setWebViewBounceJNI(const int index, const bool bounce) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setBounce", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, bounce);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setWebViewVerticalScrollIndicatorJNI(const int index, const bool indicator) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setVerticalScrollIndicator", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, indicator);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setWebViewHorizontalScrollIndicatorJNI(const int index, const bool indicator) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setHorizontalScrollIndicator", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, indicator);
        t.env->DeleteLocalRef(t.classID);
    }
}

std::string getUrlStringByFileName(const std::string &fileName) {
    const std::string basePath("file:///android_asset/");

    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    const std::string assetsPath("assets/");

    const std::string dataPath("/");
    const std::string basePathForDataPath("file://");

    std::string urlString;
    auto pos = fullPath.find(dataPath);
    if (pos == 0) {
        // for load from download asset
        urlString = basePathForDataPath + fullPath;
    } else {
        if (fullPath.find(assetsPath) != std::string::npos) {
            // for load from bundle asset
            urlString = fullPath.replace(fullPath.find_first_of(assetsPath), assetsPath.length(), basePath);
        } else {
            urlString = fullPath;
        }
    }
    return urlString;
}

void setWebViewFocusableJNI(const int index, const bool isFocusable) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setFocusable", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, index, isFocusable);
        t.env->DeleteLocalRef(t.classID);
    }
}
} // namespace

namespace cocos2d {
namespace plugin {
static std::unordered_map<int, cocos2d::plugin::WebViewImpl*> s_WebViewImpls;

WebViewImpl::WebViewImpl(WebView *webView) : _viewTag(-1), _webView(webView) {
    _viewTag = createWebViewJNI();
    s_WebViewImpls[_viewTag] = this;

    // Activityに再起動がかかった時はwebviewをリロードする
    auto listener = cocos2d::EventListenerCustom::create(EVENT_TEXTURE_RELOADED, [this] (EventCustom *) {
        restartWebViewJNI(_viewTag);
    });

    webView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, webView);
}

WebViewImpl::~WebViewImpl() {
    removeWebViewJNI(_viewTag);
    s_WebViewImpls.erase(_viewTag);
}

void WebViewImpl::loadData(const Data &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    std::string dataString(reinterpret_cast<char *>(data.getBytes()), static_cast<unsigned int>(data.getSize()));
    loadDataJNI(_viewTag, dataString, MIMEType, encoding, baseURL);
}

void WebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) {
    loadHTMLStringJNI(_viewTag, string, baseURL);
}

void WebViewImpl::loadUrl(const std::string &url) {
    loadUrlJNI(_viewTag, url);
}

void WebViewImpl::loadFile(const std::string &fileName) {
    auto fullPath = getUrlStringByFileName(fileName);
    loadFileJNI(_viewTag, fullPath);
}

void WebViewImpl::loadUrlWithHeader(const std::string &url, const std::map<std::string, std::string> &header) {
  loadUrlWithHeaderJNI(_viewTag, url, header);
}

void WebViewImpl::stopLoading() {
    stopLoadingJNI(_viewTag);
}

void WebViewImpl::reload() {
    reloadJNI(_viewTag);
}

bool WebViewImpl::canGoBack() {
    return canGoBackJNI(_viewTag);
}

bool WebViewImpl::canGoForward() {
    return canGoForwardJNI(_viewTag);
}

void WebViewImpl::goBack() {
    goBackJNI(_viewTag);
}

void WebViewImpl::goForward() {
    goForwardJNI(_viewTag);
}

void WebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
    setJavascriptInterfaceSchemeJNI(_viewTag, scheme);
}

void WebViewImpl::evaluateJS(const std::string &js) {
    evaluateJSJNI(_viewTag, js);
}

void WebViewImpl::setScalesPageToFit(const bool scalesPageToFit) {
    setScalesPageToFitJNI(_viewTag, scalesPageToFit);
}

bool WebViewImpl::shouldStartLoading(const int viewTag, const std::string &url) {
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->_webView;
        if (webView->shouldStartLoading) {
            return webView->shouldStartLoading(webView, url);
        }
    }
    return true;
}

void WebViewImpl::didFinishLoading(const int viewTag, const std::string &url){
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->_webView;
        if (webView->didFinishLoading) {
            webView->didFinishLoading(webView, url);
        }
    }
}

void WebViewImpl::didFailLoading(const int viewTag, const std::string &url){
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->_webView;
        if (webView->didFailLoading) {
            webView->didFailLoading(webView, url);
        }
    }
}

void WebViewImpl::onJsCallback(const int viewTag, const std::string &message){
    auto it = s_WebViewImpls.find(viewTag);
    if (it != s_WebViewImpls.end()) {
        auto webView = s_WebViewImpls[viewTag]->_webView;
        if (webView->onJsCallback) {
            webView->onJsCallback(webView, message);
        }
    }
}

void WebViewImpl::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags) {
    if (flags & cocos2d::Node::FLAGS_TRANSFORM_DIRTY) {
        auto directorInstance = cocos2d::Director::getInstance();
        auto glView = directorInstance->getOpenGLView();
        auto frameSize = glView->getFrameSize();

        auto winSize = directorInstance->getWinSize();

        auto leftBottom = this->_webView->convertToWorldSpace(cocos2d::Point::ZERO);
        auto rightTop = this->_webView->convertToWorldSpace(cocos2d::Point(this->_webView->getContentSize().width,this->_webView->getContentSize().height));

        auto uiLeft = frameSize.width / 2 + (leftBottom.x - winSize.width / 2 ) * glView->getScaleX();
        auto uiTop = frameSize.height /2 - (rightTop.y - winSize.height / 2) * glView->getScaleY();

        setWebViewRectJNI(_viewTag,uiLeft,uiTop,
                        (rightTop.x - leftBottom.x) * glView->getScaleX(),
                        (rightTop.y - leftBottom.y) * glView->getScaleY());
    }
}

void WebViewImpl::setVisible(bool visible) {
    setWebViewVisibleJNI(_viewTag, visible);
}

void WebViewImpl::setBounce(bool bounce) {
    setWebViewBounceJNI(_viewTag, bounce);
}

void WebViewImpl::setVerticalScrollIndicator(bool indicator) {
  setWebViewVerticalScrollIndicatorJNI(_viewTag, indicator);
}

void WebViewImpl::setHorizontalScrollIndicator(bool indicator) {
  setWebViewHorizontalScrollIndicatorJNI(_viewTag, indicator);
}

void WebViewImpl::setFocusable(bool isFocusable) {
    setWebViewFocusableJNI(_viewTag, isFocusable);
}
} // namespace cocos2d
} // namespace plugin
