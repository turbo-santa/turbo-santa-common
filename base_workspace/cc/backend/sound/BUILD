cc_library(
  name = "sound_controller",
  srcs = [
    "sound_controller.cc"
  ],
  hdrs = [
    "sound_controller.h",
    "sound_flags.h",
  ],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//cc/backend/memory:module",
    ":sound_channel",
    ":sound_player",
    "//external:glog"
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "sound_channel",
  srcs = [
    "tone_and_sweep_channel.cc"
  ],
  hdrs = [
    "sound_channel.h",
    "tone_and_sweep_channel.h",
    "sound_utils.h"
  ],
  deps = [
    "//external:glog"
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "sound_player",
  hdrs = ["sound_player.h"],
  visibility = ["//visibility:public"],
)
