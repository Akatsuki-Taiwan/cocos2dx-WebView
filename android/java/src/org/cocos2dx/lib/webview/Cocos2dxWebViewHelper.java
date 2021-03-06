package org.cocos2dx.lib.webview;

import android.os.Handler;
import android.os.Looper;
import android.os.Build;
import android.util.SparseArray;
import android.view.View;
import android.widget.FrameLayout;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.HashMap;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;


public class Cocos2dxWebViewHelper {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();
    private static Handler handler;
    private static Cocos2dxActivity cocos2dxActivity;
    private static FrameLayout layout;

    private static SparseArray<Cocos2dxWebView> webViews;
    private static int viewTag = 0;

    public Cocos2dxWebViewHelper(FrameLayout layout) {
        Cocos2dxWebViewHelper.layout = layout;
        Cocos2dxWebViewHelper.handler = new Handler(Looper.myLooper());

        Cocos2dxWebViewHelper.cocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        if (Cocos2dxWebViewHelper.webViews == null) {
            Cocos2dxWebViewHelper.webViews = new SparseArray<Cocos2dxWebView>();
        }
    }

    public static Cocos2dxActivity getCocos2dxActivity() { return cocos2dxActivity; }

    private static native boolean shouldStartLoading(int index, String message);

    public static boolean _shouldStartLoading(int index, String message) {
        return !shouldStartLoading(index, message);
    }

    private static native void didFinishLoading(int index, String message);

    public static void _didFinishLoading(int index, String message) {
        didFinishLoading(index, message);
    }

    private static native void didFailLoading(int index, String message);

    public static void _didFailLoading(int index, String message) {
        didFailLoading(index, message);
    }

    private static native void onJsCallback(int index, String message);

    public static void _onJsCallback(int index, String message) {
        onJsCallback(index, message);
    }

    public static void restartWebView(int index) {
        final int key = index;
        final Cocos2dxActivity context = (Cocos2dxActivity)Cocos2dxActivity.getContext();
        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView oldView = webViews.get(key);
                Cocos2dxWebView newView =  new Cocos2dxWebView(context, key);
                FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                        FrameLayout.LayoutParams.WRAP_CONTENT,
                        FrameLayout.LayoutParams.WRAP_CONTENT);
                layout.addView(newView, lParams);

                newView.setVisibility(oldView.getVisibility());
                newView.setOverScrollMode(oldView.getOverScrollMode());
                newView.setVerticalScrollBarEnabled(oldView.isVerticalScrollBarEnabled());
                newView.setHorizontalScrollBarEnabled(oldView.isHorizontalScrollBarEnabled());
                newView.setWebViewRect(oldView.getLeft(), oldView.getTop(), oldView.getWidth(), oldView.getHeight());
                newView.clearCache(true);
                newView.loadUrl(oldView.getUrl());

                webViews.put(key, newView);
            }
        });
    }

    @SuppressWarnings("unused")
    public static int createWebView() {
        final int index = viewTag;
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = new Cocos2dxWebView(cocos2dxActivity, index);
                FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                        FrameLayout.LayoutParams.WRAP_CONTENT,
                        FrameLayout.LayoutParams.WRAP_CONTENT);
                layout.addView(webView, lParams);

                webViews.put(index, webView);
            }
        });
        return viewTag++;
    }

    @SuppressWarnings("unused")
    public static void removeWebView(final int index) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webViews.remove(index);
                    layout.removeView(webView);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setVisible(final int index, final boolean visible) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setVisibility(visible ? View.VISIBLE : View.GONE);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setBounce(final int index, final boolean bounce) {
      cocos2dxActivity.runOnUiThread(new Runnable() {
        @Override
        public void run() {
          Cocos2dxWebView webView = webViews.get(index);
          if (webView != null) {
            webView.setOverScrollMode(bounce ? View.OVER_SCROLL_ALWAYS : View.OVER_SCROLL_NEVER);
          }
        }
      });
    }

    @SuppressWarnings("unused")
    public static void setVerticalScrollIndicator(final int index, final boolean indicator) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setVerticalScrollBarEnabled(indicator);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setHorizontalScrollIndicator(final int index, final boolean indicator) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                  if (webView != null) {
                      webView.setHorizontalScrollBarEnabled(indicator);
                  }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setWebViewRect(final int index, final int left, final int top, final int maxWidth, final int maxHeight) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setWebViewRect(left, top, maxWidth, maxHeight);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setJavascriptInterfaceScheme(final int index, final String scheme) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setJavascriptInterfaceScheme(scheme);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadData(final int index, final String data, final String mimeType, final String encoding, final String baseURL) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseURL, data, mimeType, encoding, null);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadHTMLString(final int index, final String htmlString, final String mimeType, final String encoding) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadData(htmlString, mimeType, encoding);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadUrl(final int index, final String url) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.clearCache(true);
                    webView.loadUrl(url);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadFile(final int index, final String filePath) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.clearCache(true);
                    webView.loadUrl(filePath);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadUrlWithHeader(final int index, final String url, final String[][] headers) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebView webView = webViews.get(index);
                    if (webView != null) {
                        HashMap<String,String> extraHeader = new HashMap<>();
                        for (String[] header: headers) {
                            extraHeader.put(header[0], header[1]);
                        }
                        webView.clearCache(true);
                        webView.loadUrl(url, extraHeader);
                    }
                }
            });
    }

    public static void stopLoading(final int index) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.stopLoading();
                }
            }
        });

    }

    public static void reload(final int index) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.reload();
                }
            }
        });
    }

    public static <T> T callInMainThread(Callable<T> call) throws ExecutionException, InterruptedException {
        FutureTask<T> task = new FutureTask<T>(call);
        handler.post(task);
        return task.get();
    }

    @SuppressWarnings("unused")
    public static boolean canGoBack(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                Cocos2dxWebView webView = webViews.get(index);
                return webView != null && webView.canGoBack();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    @SuppressWarnings("unused")
    public static boolean canGoForward(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                Cocos2dxWebView webView = webViews.get(index);
                return webView != null && webView.canGoForward();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    @SuppressWarnings("unused")
    public static void goBack(final int index) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goBack();
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void goForward(final int index) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goForward();
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void evaluateJS(final int index, final String js) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                        webView.evaluateJavascript(js, null);
                    }
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setScalesPageToFit(final int index, final boolean scalesPageToFit) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setScalesPageToFit(scalesPageToFit);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setFocusable(final int index, final boolean isFocusable) {
        cocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setFocusable(isFocusable);
                    webView.setFocusableInTouchMode(isFocusable);
                }
            }
        });
    }
}
