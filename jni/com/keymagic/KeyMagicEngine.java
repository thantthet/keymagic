package com.keymagic;

import java.util.HashMap;

public class KeyMagicEngine
{
	private long nativeHandle;
	public KeyMagicEngine() { initialise(); }
	private native void initialise();

    public native boolean loadKeyboardFile(String filePath);
    public native short getKeyState(int keycode);
    public native void setKeyState(int keycode, short state);
    public native void setKeyStates(byte[] states);
    
    public native boolean processKeyEvent(int keyval, int keycode, int modifier);
	public native void reset();

    public native String getContextText();

	public native long getDifference(String contextBefore, StringBuffer difference);

    public native void setHistory(String[] history);
    public native String[] getHistory();

    public native HashMap<Integer, Boolean> getSwitchMap();
    public native void setSwitchMap(HashMap<Integer, Boolean> switchMap);
}