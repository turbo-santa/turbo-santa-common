java_library(
  name = "sound",
  srcs = [
    "AudioController.java",
    "AudioRenderer.java",
    "SoundBuffer.java"
    ],
  visibility = ["//java/com/turbosanta/backend:__pkg__"],
)

genrule(
  name = "mac-compat",
  srcs = ["libsoundjni.so"],
  outs = ["libsoundjni.dylib"],
  cmd = "cp $< $@",
)