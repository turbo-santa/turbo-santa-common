#ifndef JAVA_COM_TURBOSANTA_BACKEND_HANDLE_
#define JAVA_COM_TURBOSANTA_BACKEND_HANDLE_

#include <jni.h>

namespace java_com_turbosanta_backend {

jfieldID getHandleField(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  return env->GetFieldID(klass, "nativeHandle", "J");
}

template <class T>
T* getHandle(JNIEnv* env, jobject obj) {
  jlong handle = env->GetLongField(obj, getHandleField(env, obj));
  return reinterpret_cast<T*>(handle);
}

template <class T>
void setHandle(JNIEnv* env, jobject obj, T* ptr) {
  jlong handle = reinterpret_cast<jlong>(ptr);
  env->SetLongField(obj, getHandleField(env, obj), handle);
}

} // namespace java_com_turbosanta_backend

#endif // JAVA_COM_TURBOSANTA_BACKEND_HANDLE_
