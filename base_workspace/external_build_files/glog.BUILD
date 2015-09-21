cc_library(
  name = "main",
  hdrs = glob(["src/**/*.h"]) + [
    ":src/config.h",
    ":src/glog/logging.h",
    ":src/glog/raw_logging.h",
    ":src/glog/stl_logging.h",
    ":src/glog/vlog_is_on.h",
  ],
  srcs = glob(
    ["src/**/*.cc"],
    exclude = [
      "src/windows/**/*",
      "src/**/*test*.cc",
      "src/**/*main.cc",
    ],
  ),
  includes = ["src"],
  linkopts = ["-pthread"],
  visibility = ["//visibility:public"],
)

genrule(
  name = "exec_configure",
  srcs = glob(["**/*"]),
  outs = [
    "src/config.h",
    "src/glog/logging.h",
    "src/glog/raw_logging.h",
    "src/glog/stl_logging.h",
    "src/glog/vlog_is_on.h",
  ],
  cmd = """STARTING_DIR=`pwd`; ROOT_DIR=external/remote_glog; DEST_DIR=$(@D)/external/remote_glog/src;
    cd $$ROOT_DIR; ./configure; cd $$STARTING_DIR;
    mv $$ROOT_DIR/src/config.h $$DEST_DIR;
    mv $$ROOT_DIR/src/glog/logging.h $$DEST_DIR/glog;
    mv $$ROOT_DIR/src/glog/raw_logging.h $$DEST_DIR/glog;
    mv $$ROOT_DIR/src/glog/stl_logging.h $$DEST_DIR/glog;
    mv $$ROOT_DIR/src/glog/vlog_is_on.h $$DEST_DIR/glog;""",
)
