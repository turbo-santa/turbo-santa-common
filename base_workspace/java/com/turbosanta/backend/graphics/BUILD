java_library(
  name = "screen_java",
  srcs = ["Screen.java"],
  deps = [":drawable_area"],
  data = select({
    "//:darwin": [":libscreenjni.dylib"],
    "//conditions:default": [":libscreenjni.so"],
  }),
  visibility = [
    "//java/com/turbosanta/backend:__subpackages__",
  ],
)

java_library(
  name = "drawable_area",
  srcs = ["DrawableArea.java"],
  visibility = ["//visibility:public"],
)

genrule(
  name = "mac-compat",
  srcs = ["libscreenjni.so"],
  outs = ["libscreenjni.dylib"],
  cmd = "cp $< $@",
)

cc_binary(
  name = "libscreenjni.so",
  srcs = [
    "com_turbosanta_backend_graphics_Screen.h",
    "com_turbosanta_backend_graphics_Screen.cc",
  ],
  deps = [
    "//cc/backend/graphics:screen",
    "//java/com/turbosanta/backend:jni_headers",
    ":screen_cc",
  ],
  linkshared = 1,
)

cc_library(
  name = "screen_cc",
  hdrs = ["screen.h"],
  srcs = ["screen.cc"],
  deps = [
    "//cc/backend/graphics:screen",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  visibility = [
    "//java/com/turbosanta/backend:__subpackages__",
  ],
)
