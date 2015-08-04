#include "java/com/turbosanta/backend/joypad/com_turbosanta_backend_joypad_Joypad.h"

#include "backend/clocktroller/clocktroller.h"
#include "backend/memory/joypad_module.h"
#include "java/com/turbosanta/backend/handle.h"

using back_end::clocktroller::Clocktroller;
using back_end::memory::JoypadFlag;
using java_com_turbosanta_backend::getHandle;
using java_com_turbosanta_backend::setHandle;

void Java_com_turbosanta_backend_joypad_Joypad_init(JNIEnv* env, jobject obj, jobject clocktroller_obj) {
  Clocktroller* clocktroller = getHandle<Clocktroller>(env, clocktroller_obj);
  JoypadFlag* joypad = clocktroller->joypad_flag();
  setHandle<JoypadFlag>(env, obj, joypad);
}

void Java_com_turbosanta_backend_joypad_Joypad_setDown(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_down(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setUp(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_up(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setLeft(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_left(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setRight(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_right(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setStart(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_start(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setSelect(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_select(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setB(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_b(is_pushed);
}

void Java_com_turbosanta_backend_joypad_Joypad_setA(JNIEnv* env, jobject obj, jboolean is_pushed) {
  getHandle<JoypadFlag>(env, obj)->set_a(is_pushed);
}
