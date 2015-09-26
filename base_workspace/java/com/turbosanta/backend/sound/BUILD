java_library(
  name = "sound_player_java",
  srcs = ["NativeAudio.java"],
  deps = [":audio_controller"],
  data = select({
    "//:darwin": [":libsoundjni.dylib"],
    "//conditions:default": [":libsoundjni.so"]
  }),
  visibility = [
    "//java/com/turbosanta/backend:__subpackages__"
  ]
)

java_library(
  name = "audio_controller",
  srcs = [
    "AudioController.java",
    "SoundBuffer.java",
  ],
  visibility = ["//visibility:public"]
)

genrule(
  name = "mac-compat",
  srcs = ["libsoundjni.so"],
  outs = ["libsoundjni.dylib"],
  cmd = "cp $< $@",
)

cc_binary(
  name = "libsoundjni.so",
  srcs = [
    "com_turbosanta_backend_sound_NativeAudio.h",
    "com_turbosanta_backend_sound_NativeAudio.cc",
  ],
  deps = [
    "//cc/backend/sound:sound_player",
    "//java/com/turbosanta/backend:jni_headers",
    ":sound_player_cc"
  ],
  linkshared = 1,
)

cc_library(
  name = "sound_player_cc",
  hdrs = ["sound_player.h"],
  srcs = ["sound_player.cc"],
  deps = [
    "//cc/backend/sound:sound_player",
    "//java/com/turbosanta/backend:jni_headers"
  ],
  visibility = [
    "//java/com/turbosanta/backend:__subpackages__",
  ]
)