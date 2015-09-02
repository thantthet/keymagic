#!/bin/sh

ANDROID_JAR="$ANDROID_SDK_HOME/platforms/android-23/android.jar"

javac com/keymagic/*.java
java -classpath "$ANDROID_JAR":. com.keymagic.test