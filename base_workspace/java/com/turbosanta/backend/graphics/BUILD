java_library(
  name = "screen_java",
  srcs = ["Screen.java"],
  deps = [":drawable_area"],
  data = [":libscreenjni.so"],
  visibility = ["//java/com/turbosanta/backend:__pkg__"],
)

java_library(
  name = "drawable_area",
  srcs = ["DrawableArea.java"],
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "libscreenjni.so",
  srcs = [
    "com_turbosanta_backend_graphics_Screen.h",
    "com_turbosanta_backend_graphics_Screen.cc",
  ],
  deps = [
    "//backend/graphics:screen",
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
    "//backend/graphics:screen",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  visibility = ["//java/com/turbosanta/backend:__pkg__"],
)
