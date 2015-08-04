java_library(
  name = "clocktroller",
  srcs = ["Clocktroller.java"],
  deps = [
    "//java/com/turbosanta/backend/graphics:screen_java",
  ],
  data = [":libclocktrollerjni.so"],
  visibility = ["//java/com/turbosanta/backend:__subpackages__"],
)

cc_binary(
  name = "libclocktrollerjni.so",
  srcs = [
    "com_turbosanta_backend_clocktroller_Clocktroller.cc",
    "com_turbosanta_backend_clocktroller_Clocktroller.h",
  ],
  deps = [
    "//backend/clocktroller",
    "//backend/graphics:screen",
    "//java/com/turbosanta/backend/graphics:screen_cc",
    "//submodules:glog",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  linkshared = 1,
)

