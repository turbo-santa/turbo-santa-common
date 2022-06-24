cc_library(
  name = "graphics_controller",
  hdrs = ["graphics_controller.h"],
  srcs = ["graphics_controller.cc"],
  deps = [
    "//cc/backend/memory/interrupt:interrupt_flag",
    "//cc/backend/memory/interrupt:primary_flags",
    "//cc/backend/memory:module",
    "@com_github_google_glog//:glog",
    ":graphics_flags",
    ":screen",
    ":vram_segment",
    "@ncurses.dev//:ncurses"
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "screen",
  hdrs = ["screen.h"],
  deps = ["@com_github_google_glog//:glog"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "graphics_flags",
  hdrs = ["graphics_flags.h"],
  deps = ["@com_github_google_glog//:glog"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "vram_segment",
  hdrs = ["vram_segment.h"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "@com_github_google_glog//:glog",
  ],
)

