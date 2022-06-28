cc_library(
    name = "ncurses",
    hdrs = glob(["include/*curses*h"]),
    srcs = glob(["lib/*curses.so*"]),
    includes = [ "include/" ],
    visibility = ["//visibility:public"],
    deps = [
      "@usr//:unctrl",
    ],
)

cc_library(
    name = "unctrl",
    hdrs = ["include/unctrl.h"],
    includes = [ "include/" ],
    visibility = ["//visibility:public"],
)
