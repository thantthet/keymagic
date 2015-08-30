#include "com_keymagic_KeyMagicEngine.h" 
#include "handle.h" 
#include <keymagic.h>
#include <codecvt>

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

jboolean Java_com_keymagic_KeyMagicEngine_loadKeyboardFile(JNIEnv *env, jobject obj, jstring pathString)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
    const char *str_path = env->GetStringUTFChars(pathString, 0);
    bool success = engine->loadKeyboardFile(str_path);
    env->ReleaseStringUTFChars(pathString, str_path);
    return success;
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

jlong Java_com_keymagic_KeyMagicEngine_getDifference(JNIEnv *env, jobject obj, jstring contextBefore_java, jobject difference_java)
{
	KeyMagicEngine *engine = getHandle<KeyMagicEngine>(env, obj);
	const char *contextBeforeUTF8 = env->GetStringUTFChars(contextBefore_java, 0);
	
	KeyMagicString contextBefore;
	string2wstring(contextBeforeUTF8, contextBefore);
	KeyMagicString difference;

	unsigned long deleteCount = engine->getDifference(contextBefore, &difference);

	if (difference_java != 0) {
		jclass clazz = env->GetObjectClass(difference_java);
		jmethodID mid = 
        env->GetMethodID (clazz, 
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