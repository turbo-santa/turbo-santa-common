#include "java/com/turbosanta/backend/com_turbosanta_backend_Clocktroller.h"

#include <vector>
#include "backend/clocktroller/clocktroller.h"
#include "backend/graphics/screen.h"
#include "java/com/turbosanta/backend/handle.h"

using std::vector;
using back_end::clocktroller::Clocktroller;
using back_end::graphics::Screen;
using java_com_turbosanta_backend::setHandle;
using java_com_turbosanta_backend::getHandle;

class NullScreen : public Screen {
 public:
  void Draw(const back_end::graphics::ScreenRaster&) override {}
};

void Java_com_turbosanta_backend_Clocktroller_init(JNIEnv* env, jobject obj, jbyteArray rom, jlong length) {
  // Create clocktroller.
  Clocktroller* clocktroller = new Clocktroller(new NullScreen());

  // Init with ROM data.
  vector<uint8_t> buffer(length);
  env->GetByteArrayRegion(rom, 0, length, reinterpret_cast<jbyte*>(buffer.data()));
  clocktroller->Init(buffer.data(), length);

  // Store pointer to clocktroller in java object.
  setHandle<Clocktroller>(env, obj, clocktroller);
}

void Java_com_turbosanta_backend_Clocktroller_run(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Run();
}

void Java_com_turbosanta_backend_Clocktroller_pause(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Pause();
}

void Java_com_turbosanta_backend_Clocktroller_kill(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Kill();
}

void Java_com_turbosanta_backend_Clocktroller_waitUntilDone(JNIEnv* env, jobject obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, obj);
  clocktroller->Wait();
}
