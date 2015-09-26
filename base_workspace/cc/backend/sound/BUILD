cc_library(
  name = "sound_controller",
  srcs = [
    "sound_controller.cc"
  ],
  hdrs = [
    "sound_controller.h"
  ],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//cc/backend/memory:module",
    ":sound_player"
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "sound_player",
  hdrs = ["sound_player.h"],
  visibility = ["//visibility:public"],
)