package com.keymagic;

public class test
{
    static {
        System.loadLibrary("keymagic-jni");
    }

    public static void main(String[] args) {
        KeyMagicEngine engine = new KeyMagicEngine();
        engine.loadKeyboardFile("/Users/thantthet/codes/keymagic-git/LayoutScripts/Zawgyi L - Unicode.km2");
        engine.processKeyEvent('a', 'A', 0);

        String text = engine.getContextText();

        engine.processKeyEvent('u', 'U', 0);

        StringBuffer difference = new StringBuffer ("");
        long delete = engine.getDifference(text, difference);
        System.out.print("from = " + text + "\n");
        System.out.print("delete = " + delete + "\n");
        System.out.print("difference = " + difference + "\n");
    }
}