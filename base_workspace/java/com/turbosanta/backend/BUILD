java_binary(
  name = "main",
  srcs = ["Main.java"],
  deps = [
    "//java/com/turbosanta/backend/graphics:drawable_area",
    "//java/com/turbosanta/backend/graphics:screen_java",
    "//java/com/turbosanta/backend/clocktroller",
  ],
  main_class = "com.turbosanta.backend.Main",
  jvm_flags = ["-Xss515m"],
)

genrule(
  name = "copy_link_jni_header",
  srcs = ["//tools/jdk:jni_header"],
  outs = ["jni.h"],
  cmd = "cp -f $< $@",
)

genrule(
  name = "copy_link_jni_md_header",
  srcs = select({
    # "//src:darwin": ["//tools/jdk:jni_md_header-darwin"],
    "//conditions:default": ["//tools/jdk:jni_md_header-linux"],
  }),
  outs = ["jni_md.h"],
  cmd = "cp -f $< $@",
)

cc_library(
  name = "jni_headers",
  srcs = [
    "handle.h",
    ":jni.h",
    ":jni_md.h",
  ],
  includes = ["."],
  visibility = ["//:__subpackages__"],
)
