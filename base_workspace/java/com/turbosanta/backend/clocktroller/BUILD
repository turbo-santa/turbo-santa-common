java_library(
  name = "clocktroller",
  srcs = ["Clocktroller.java"],
  deps = [
    "//java/com/turbosanta/backend/graphics:screen_java",
    "//java/com/turbosanta/backend/sound:sound_player_java"
  ],
  data = select({
    "//:darwin": ["libclocktrollerjni.dylib"],
    "//conditions:default": [":libclocktrollerjni.so"],
  }),
  visibility = ["//java/com/turbosanta/backend:__subpackages__"],
)

genrule(
  name = "mac-compat",
  srcs = ["libclocktrollerjni.so"],
  outs = ["libclocktrollerjni.dylib"],
  cmd = "cp $< $@",
)

cc_binary(
  name = "libclocktrollerjni.so",
  srcs = [
    "com_turbosanta_backend_clocktroller_Clocktroller.cc",
    "com_turbosanta_backend_clocktroller_Clocktroller.h",
  ],
  deps = [
    "//cc/backend/clocktroller",
    "//cc/backend/graphics:screen",
    "//cc/backend/sound:sound_player",
    "//external:glog",
    "//java/com/turbosanta/backend/graphics:screen_cc",
    "//java/com/turbosanta/backend/sound:sound_player_cc",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  linkshared = 1,
)

