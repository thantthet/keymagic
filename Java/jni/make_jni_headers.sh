#!/bin/sh

ANDROID_JAR="$ANDROID_SDK_HOME/platforms/android-23/android.jar"
javac -classpath "$ANDROID_JAR" com/keymagic/*.java
javah -jni -classpath "$ANDROID_JAR":. com.keymagic.KeyMagicEngine