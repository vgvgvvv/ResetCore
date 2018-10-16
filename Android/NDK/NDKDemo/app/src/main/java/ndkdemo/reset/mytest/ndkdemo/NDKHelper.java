package ndkdemo.reset.mytest.ndkdemo;

public class NDKHelper {

    public static NDKHelper Instance = new NDKHelper();

    static {
        System.loadLibrary("NDKDemo");
    }

    public native int Add(int a, int b);

}
