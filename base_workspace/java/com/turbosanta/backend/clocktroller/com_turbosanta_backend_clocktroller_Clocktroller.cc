#include "java/com/turbosanta/backend/clocktroller/com_turbosanta_backend_clocktroller_Clocktroller.h"

#include <vector>
#include "cc/backend/clocktroller/clocktroller.h"
#include "cc/backend/graphics/screen.h"
#include "java/com/turbosanta/backend/graphics/screen.h"
#include "java/com/turbosanta/backend/handle.h"
#include "glog/logging.h"

using std::vector;
using backend::clocktroller::Clocktroller;
using java_com_turbosanta_backend::graphics::Screen;
using java_com_turbosanta_backend::setHandle;
using java_com_turbosanta_backend::getHandle;

Screen* GetScreen(JNIEnv* env, jobject clocktroller_obj) {
  jclass clocktroller_class = env->GetObjectClass(clocktroller_obj);
  jfieldID screen_fid = env->GetFieldID(clocktroller_class, "screen", "Lcom/turbosanta/backend/graphics/Screen;");
  jobject screen_obj = env->GetObjectField(clocktroller_obj, screen_fid);
  return getHandle<Screen>(env, screen_obj);
}

void Java_com_turbosanta_backend_clocktroller_Clocktroller_init(JNIEnv* env, jobject obj, jbyteArray rom, jlong length) {
  // Create clocktroller.
  Clocktroller* clocktroller = new Clocktroller(GetScreen(env, obj));

  // Init logging.
  FLAGS_logtostderr = 0;
  FLAGS_log_dir = "log";
  const char* turbo = "turbo";
  google::InitGoogleLogging(turbo);
  google::InstallFailureSignalHandler();

  // Init with ROM data.
  vector<uint8_t> buffer(length);
  env->GetByteArrayRegion(rom, 0, length, reinterpret_cast<jbyte*>(buffer.data()));
  clocktroller->Init(buffer.data(), length);

  // Store pointer to clocktroller in java object.
  setHandle<Clocktroller>(env, obj, clocktroller);
}

void Java_com_turbosanta_backend_clocktroller_Clocktroller_run(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Run();
}

void Java_com_turbosanta_backend_clocktroller_Clocktroller_pause(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Pause();
}

void Java_com_turbosanta_backend_clocktroller_Clocktroller_kill(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Kill();
}
