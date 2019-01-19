package nativeread.reset.com.nativeread;

import android.content.Context;
import android.os.Build;
import android.provider.Settings;
import android.support.annotation.RequiresApi;
import android.util.Log;
import android.view.DisplayCutout;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.unity3d.player.UnityPlayer;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class ScreenManager {

    public static final int FLAG_NOTCH_SUPPORT=0x00010000;

    /**
     * 启用华为适配
     * @param window
     */
    public static void setFullScreenWindowLayoutInDisplayCutout(Window window) {
        if (window == null) {
            return;
        }
        WindowManager.LayoutParams layoutParams = window.getAttributes();
        try {
            Class layoutParamsExCls = Class.forName("com.huawei.android.view.LayoutParamsEx");
            Constructor con=layoutParamsExCls.getConstructor(WindowManager.LayoutParams.class);
            Object layoutParamsExObj=con.newInstance(layoutParams);
            Method method=layoutParamsExCls.getMethod("addHwFlags", int.class);
            method.invoke(layoutParamsExObj, FLAG_NOTCH_SUPPORT);
        } catch (ClassNotFoundException e) {
            Log.e("wesee", "hw add notch screen flag api error");
        } catch (NoSuchMethodException e) {
            Log.e("wesee", "hw add notch screen flag api error");
        } catch (IllegalAccessException e) {
            Log.e("wesee", "hw add notch screen flag api error");
        } catch (InstantiationException e) {
            Log.e("wesee", "hw add notch screen flag api error");
        } catch (InvocationTargetException e) {
            Log.e("wesee", "hw add notch screen flag api error");
        } catch (Exception e) {
            Log.e("wesee", "other Exception");
        }
    }

    /**
     * 获取华为中是否有刘海屏
     * @param context
     * @return
     */
    public static boolean hasNotchInHuawei(Context context) {
        boolean hasNotch = false;
        try {
            ClassLoader cl = context.getClassLoader();
            Class HwNotchSizeUtil = cl.loadClass("com.huawei.android.util.HwNotchSizeUtil");
            Method hasNotchInScreen = HwNotchSizeUtil.getMethod("hasNotchInScreen");
            if(hasNotchInScreen != null) {
                hasNotch = (boolean) hasNotchInScreen.invoke(HwNotchSizeUtil);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return hasNotch;
    }

    /**
     * 获取华为刘海屏高度
     * @param context
     * @return
     */
    public static int[] getNotchSize(Context context) {
        int[] ret = new int[]{0, 0};
        try {
            ClassLoader cl = context.getClassLoader();
            Class HwNotchSizeUtil = cl.loadClass("com.huawei.android.util.HwNotchSizeUtil");
            Method get = HwNotchSizeUtil.getMethod("getNotchSize");
            ret = (int[]) get.invoke(HwNotchSizeUtil);
        } catch (ClassNotFoundException e) {
            Log.e("wesee", "getNotchSize ClassNotFoundException");
        } catch (NoSuchMethodException e) {
            Log.e("wesee", "getNotchSize NoSuchMethodException");
        } catch (Exception e) {
            Log.e("wesee", "getNotchSize Exception");
        } finally {
            return ret;
        }
    }

    /**
     * Oppo中获取是否有刘海屏
     * @param context
     * @return
     */
    public static boolean hasNotchInOppo(Context context) {
        return context.getPackageManager().hasSystemFeature("com.oppo.feature.screen.heteromorphism");
    }


    /**
     * Vivo中获取是否有刘海屏
     * @param context
     * @return
     */
    public static boolean hasNotchInVivo(Context context) {
        boolean hasNotch = false;
        try {
            ClassLoader cl = context.getClassLoader();
            Class ftFeature = cl.loadClass("android.util.FtFeature");
            Method[] methods = ftFeature.getDeclaredMethods();
            if (methods != null) {
                for (int i = 0; i < methods.length; i++) {
                    Method method = methods[i];
                    if(method != null) {
                        if (method.getName().equalsIgnoreCase("isFeatureSupport")) {
                            hasNotch = (boolean) method.invoke(ftFeature, 0x00000020);
                            break;
                        }
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            hasNotch = false;
        }
        return hasNotch;
    }

    /**
     * 获取小米是否存在刘海屏
     * @param context
     * @return
     */
    public static boolean hasNotchInXiaoMi(Context context){
        boolean result = SystemProperties.getInt("ro.miui.notch", 0) == 1;
        if(Settings.Global.getInt(context.getContentResolver(), "force_black", 0) == 1){
            result = false;
        }
        return result;
    }

    public static int getNotchHeightInXiaoMi(Context context){
        int result = -1;
        int resourceId = context.getResources().getIdentifier("notch_height", "dimen", "android");
        if (resourceId > 0) {
            result = context.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    public static int getNotchWidthInXiaoMi(Context context){
        int result = -1;
        int resourceId = context.getResources().getIdentifier("notch_width", "dimen", "android");
        if (resourceId > 0) {
            result = context.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    /**
     * 获取状态栏高度
     * @param context
     * @return
     */
    public static int getStatusBarHeight(Context context) {
        int result = 0;
        int resourceId = context.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = context.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    /**
     * 是否支持Android P
     * @return
     */
    public static int supportGoogleAPI(){
        return Build.VERSION.SDK_INT;
    }

    @RequiresApi(api = 28)
    public static int[] getNotchInGoogle(){
        View view = UnityPlayer.currentActivity.getWindow().getDecorView();
        DisplayCutout cutout = view.getRootWindowInsets().getDisplayCutout();
        if(cutout == null){
            Utility.LogError("cutout is null!!!");
            return new int[4];
        }
        int[] result = new int[]{
                cutout.getSafeInsetBottom(),
                cutout.getSafeInsetTop(),
                cutout.getSafeInsetLeft(),
                cutout.getSafeInsetRight()
        };
        return result;
    }

    /**
     * 尝试获取刘海屏高度
     * @return
     */
    public static int TryGetNotchSize(){
        Context context = UnityPlayer.currentActivity;
        if(hasNotchInHuawei(context)){
            return getNotchSize(context)[1];
        }else if(hasNotchInOppo(context)){
            return getStatusBarHeight(context);
        }else if(hasNotchInVivo(context)) {
            return getStatusBarHeight(context);
        }else if(hasNotchInXiaoMi(context)){
            return getNotchHeightInXiaoMi(context);
        }else{
            return -1;
        }
    }


}
