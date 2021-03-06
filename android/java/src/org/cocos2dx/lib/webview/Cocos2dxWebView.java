package org.cocos2dx.lib.webview;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.webkit.WebResourceRequest;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebSettings;
import android.widget.FrameLayout;
import android.graphics.Color;
import android.os.Build;

import java.lang.reflect.Method;
import java.net.URI;

public class Cocos2dxWebView extends WebView {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();

    private int viewTag;
    private String jsScheme;

    public Cocos2dxWebView(Context context) {
        this(context, -1);
    }

    @SuppressLint("SetJavaScriptEnabled")
    public Cocos2dxWebView(Context context, int viewTag) {
        super(context);
        this.viewTag = viewTag;
        this.jsScheme = "";

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);

        this.getSettings().setSupportZoom(false);

        this.getSettings().setJavaScriptEnabled(true);

        this.setBackgroundColor(Color.TRANSPARENT);

        // httpから始まるURLを許可する
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            this.getSettings().setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);
        }

        // `searchBoxJavaBridge_` has big security risk. http://jvn.jp/en/jp/JVN53768697
        try {
            Method method = this.getClass().getMethod("removeJavascriptInterface", new Class[]{String.class});
            method.invoke(this, "searchBoxJavaBridge_");
        } catch (Exception e) {
            Log.d(TAG, "This API level do not support `removeJavascriptInterface`");
        }

        this.setWebViewClient(new Cocos2dxWebViewClient());
    }

    public void setJavascriptInterfaceScheme(String scheme) {
        this.jsScheme = scheme != null ? scheme : "";
    }

    public void setScalesPageToFit(boolean scalesPageToFit) {
        this.getSettings().setSupportZoom(scalesPageToFit);
    }

    class Cocos2dxWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
            String urlString = request.getUrl().toString();
            // アクセス先がtwitterである限りは外部ブラウザを起動しない
            if (-1 != urlString.indexOf("twitter")) {
                return false;
            }
            if (urlString.startsWith("mailto:")) {
                Intent intent = new Intent(Intent.ACTION_SENDTO, Uri.parse(urlString));
                Cocos2dxWebViewHelper.getCocos2dxActivity().startActivity(intent);
                return true;
            }
            // WebView上のURLリンクをタップした時、起動するブラウザを選択させる（複数ブラウザがインストールされてる場合のみ）
            Intent target = new Intent(Intent.ACTION_VIEW, Uri.parse(urlString));
            Intent chooser = Intent.createChooser(target, null);
            Cocos2dxWebViewHelper.getCocos2dxActivity().startActivity(chooser);
            return true;
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            super.onPageFinished(view, url);
            Cocos2dxWebViewHelper._didFinishLoading(viewTag, url);
        }

        @Override
        public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
            super.onReceivedError(view, errorCode, description, failingUrl);
            Cocos2dxWebViewHelper._didFailLoading(viewTag, failingUrl);
        }
    }

    public void setWebViewRect(int left, int top, int maxWidth, int maxHeight) {
        fixSize(left, top, maxWidth, maxHeight);
    }

    private void fixSize(int left, int top, int width, int height) {
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.leftMargin = left;
        layoutParams.topMargin = top;
        layoutParams.width = width;
        layoutParams.height = height;
        this.setLayoutParams(layoutParams);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (canGoBack()) {
                goBack();
            }
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }
}
