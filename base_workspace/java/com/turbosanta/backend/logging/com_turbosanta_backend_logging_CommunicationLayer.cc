#include "java/com/turbosanta/backend/logging/com_turbosanta_backend_logging_CommunicationLayer.h"

#include <cstdint>
#include "cc/backend/logging/communication_layer.h"
#include "java/com/turbosanta/backend/handle.h"

using std::vector;
using backend::logging::CommunicationLayer;
using java_com_turbosanta_backend::getHandle;

jbyteArray Java_com_turbosanta_backend_logging_CommunicationLayer_takeMessageInternal(JNIEnv* env, jobject obj) {
  vector<uint8_t> buffer = getHandle<CommunicationLayer>(env, obj)->TakeMessage();
  jbyteArray java_array = env->NewByteArray(buffer.size());
  env->SetByteArrayRegion(java_array, 0, buffer.size(), reinterpret_cast<jbyte*>(buffer.data()));
  return java_array;
}

jboolean Java_com_turbosanta_backend_logging_CommunicationLayer_isClosed(JNIEnv* env, jobject obj) {
  return getHandle<CommunicationLayer>(env, obj)->is_closed();
}

