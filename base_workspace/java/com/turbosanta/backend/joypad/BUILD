java_library(
  name = "joypad",
  srcs = ["Joypad.java"],
  deps = ["//java/com/turbosanta/backend/clocktroller"],
  data = [":libjoypadjni.so"],
  visibility = ["//java/com/turbosanta/backend:__pkg__"],
)

cc_binary(
  name = "libjoypadjni.so",
  srcs = [
    "com_turbosanta_backend_joypad_Joypad.cc",
    "com_turbosanta_backend_joypad_Joypad.h",
  ],
  deps = [
    "//cc/backend/clocktroller",
    "//cc/backend/memory:joypad_module",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  linkshared = 1,
)
