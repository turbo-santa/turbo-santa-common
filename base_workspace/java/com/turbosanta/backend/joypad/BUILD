java_library(
  name = "joypad",
  srcs = ["Joypad.java"],
  deps = ["//java/com/turbosanta/backend/clocktroller"],
  data = select({
    "//:darwin": [":libjoypadjni.dylib"],
    "//conditions:default": [":libjoypadjni.so"],
  }),
  visibility = ["//java/com/turbosanta/backend:__pkg__"],
)

genrule(
  name = "mac-compat",
  srcs = ["libjoypadjni.so"],
  outs = ["libjoypadjni.dylib"],
  cmd = "cp $< $@",
)

cc_binary(
  name = "libjoypadjni.so",
  srcs = [
    "com_turbosanta_backend_joypad_Joypad.cc",
    "com_turbosanta_backend_joypad_Joypad.h",
  ],
  deps = [
    "//cc/backend/clocktroller",
    "//cc/backend/memory/joypad:joypad_module",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  linkshared = 1,
)
