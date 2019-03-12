#include "com_keymagic_KeyMagicEngine.h" 
#include "handle.h" 
#include <keymagic.h>
#include <codecvt>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "android_fopen.h"

#define APPNAME "KeyMagicJNI"

using namespace libkm;

void wstring2string(std::wstring wstr, std::string &str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
	std::string u8str = conv1.to_bytes(wstr);
	str.assign(u8str);
}

void string2wstring(std::string str, std::wstring &wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	std::wstring u16str = myconv.from_bytes(str);
	wstr.assign(u16str);
}

void Java_com_keymagic_KeyMagicEngine_initialise(JNIEnv *env, jobject obj)
{
	KeyMagicEngine *engine = new KeyMagicEngine();
	setHandle(env, obj, engine);
}

jboolean Java_com_keymagic_KeyMagicEngine_loadKeyboardFile__Ljava_lang_String_2(JNIEnv *env, jobject obj, jstring pathString)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	const char *str_path = env->GetStringUTFChars(pathString, 0);
	bool success = engine->loadKeyboardFile(str_path);
	env->ReleaseStringUTFChars(pathString, str_path);
	return success;
}

jboolean Java_com_keymagic_KeyMagicEngine_loadKeyboardFile__Landroid_content_res_AssetManager_2Ljava_lang_String_2(JNIEnv *env, jobject obj, jobject assetManager, jstring filename)
{
	// convert Java string to UTF-8
	const char *utf8 = env->GetStringUTFChars(filename, NULL);
	assert(NULL != utf8);

	__android_log_print(ANDROID_LOG_INFO, APPNAME, "opening asset file: %s.", utf8);

	// use asset manager to open asset by filename
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	assert(NULL != mgr);
	
	android_fopen_set_asset_manager(mgr);

	FILE *hFile = android_fopen(utf8, "rb");

	env->ReleaseStringUTFChars(filename, utf8);

	__android_log_print(ANDROID_LOG_INFO, APPNAME, "opening keyboard from android file handle.");

	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	return engine->loadKeyboardFromFileHandle(hFile);
}

jshort Java_com_keymagic_KeyMagicEngine_getKeyState(JNIEnv *env, jobject obj, jint keycode)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	return engine->getKeyState(keycode);
}

void Java_com_keymagic_KeyMagicEngine_setKeyState(JNIEnv *env, jobject obj, jint keycode, jshort state)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	engine->setKeyState(keycode, state);
}

void Java_com_keymagic_KeyMagicEngine_setKeyStates(JNIEnv *env, jobject obj, jbyteArray states_java)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	jbyte *states = env->GetByteArrayElements(states_java, 0);

	engine->setKeyStates((unsigned char *)states);

	env->ReleaseByteArrayElements(states_java, states, JNI_ABORT);
}

jboolean Java_com_keymagic_KeyMagicEngine_processKeyEvent(JNIEnv *env, jobject obj, jint keyval, jint keycode, jint modifier)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	return engine->processKeyEvent(keyval, keycode, modifier);
}

void Java_com_keymagic_KeyMagicEngine_reset(JNIEnv *env, jobject obj)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	engine->reset();
}

jstring Java_com_keymagic_KeyMagicEngine_getContextText(JNIEnv *env, jobject obj)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	KeyMagicString text = engine->getContextText();
	std::string utf8str;
	wstring2string(text, utf8str);
	jstring text_java = env->NewStringUTF(utf8str.c_str());
	return text_java;
}

void Java_com_keymagic_KeyMagicEngine_setContextText(JNIEnv *env, jobject obj, jstring context)
{
	const char *contextUTF8 = env->GetStringUTFChars(context, 0);
	KeyMagicString contextString;
	string2wstring(contextUTF8, contextString);

	env->ReleaseStringUTFChars(context, contextUTF8);

	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	engine->setContextText(contextString);
}

jlong Java_com_keymagic_KeyMagicEngine_getDifference(JNIEnv *env, jobject obj, jstring contextBefore_java, jobject difference_java)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	const char *contextBeforeUTF8 = env->GetStringUTFChars(contextBefore_java, 0);
	
	KeyMagicString contextBefore;
	string2wstring(contextBeforeUTF8, contextBefore);
	env->ReleaseStringUTFChars(contextBefore_java, contextBeforeUTF8);

	KeyMagicString difference;

	unsigned long deleteCount = engine->getDifference(contextBefore, &difference);

	if (difference_java != 0) {
		jclass clazz = env->GetObjectClass(difference_java);
		jmethodID mid = 
		env->GetMethodID (
			clazz, 
			"append",
			"(Ljava/lang/String;)Ljava/lang/StringBuffer;");
		if (mid != 0) {
			std::string differenceUTF8;
			wstring2string(difference, differenceUTF8);
			jstring _jstring = env->NewStringUTF (differenceUTF8.c_str());
			env->CallObjectMethod (difference_java, mid, _jstring);
		}
	}
	return deleteCount;
}

void Java_com_keymagic_KeyMagicEngine_setHistory(JNIEnv *env, jobject obj, jobjectArray history_java) {
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	int stringCount = env->GetArrayLength(history_java);

	TContextHistory history_native;

	for (int i=0; i<stringCount; i++) {
		jstring string = (jstring) env->GetObjectArrayElement(history_java, i);
		const char *rawString = env->GetStringUTFChars(string, 0);
		
		KeyMagicString contextString;
		string2wstring(rawString, contextString);

		history_native.push_back(contextString);

		env->ReleaseStringUTFChars(string, rawString);
	}

	engine->setHistory(history_native);
}

jobjectArray Java_com_keymagic_KeyMagicEngine_getHistory(JNIEnv *env, jobject obj) {
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	TContextHistory history_native = engine->getHistory();

	jobjectArray history_java = (jobjectArray)env->NewObjectArray(history_native.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));

	for(int i=0; i<history_native.size(); i++) {
		std::string contextUTF8;
		KeyMagicString context = history_native.at(i);
		wstring2string(context, contextUTF8);
		env->SetObjectArrayElement(history_java, i, env->NewStringUTF(contextUTF8.c_str()));
	}

	return history_java;
}
