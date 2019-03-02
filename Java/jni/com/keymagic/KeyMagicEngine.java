package com.keymagic;

import android.content.res.AssetManager;

import java.util.HashMap;

public class KeyMagicEngine
{
	private long nativeHandle;
	public KeyMagicEngine() { initialise(); }
	private native void initialise();

    public native boolean loadKeyboardFile(String filePath);
    public native boolean loadKeyboardFile(AssetManager assetManager, String name);
    public native short getKeyState(int keycode);
    public native void setKeyState(int keycode, short state);
    public native void setKeyStates(byte[] states);
    
    public native boolean processKeyEvent(int keyval, int keycode, int modifier);
	public native void reset();

    public native String getContextText();
    public native void setContextText(String textContext);

	public native long getDifference(String contextBefore, StringBuffer difference);

    public native void setHistory(String[] history);
    public native String[] getHistory();

    public native HashMap<Integer, Boolean> getSwitchMap();
    public native void setSwitchMap(HashMap<Integer, Boolean> switchMap);


    public static KeyMagicEngine engineFromFilePath(String filePath) {
        KeyMagicEngine newEngine = new KeyMagicEngine();
        if (!newEngine.loadKeyboardFile(filePath)) {
            Log.e(TAG, "Could not load keyboard from file: " + filePath);
            return null;
        }
        return newEngine;
    }

    public static KeyMagicEngine engineFromAndroidAssets(AssetManager assetManager, String name) {
        KeyMagicEngine newEngine = new KeyMagicEngine();
        if (!newEngine.loadKeyboardFile(assetManager, name)) {
            Log.e(TAG, "Could not load keyboard from asset: " + name);
            return null;
        }
        return newEngine;
    }
}
}