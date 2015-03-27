cc_library(
  name = "config",
  hdrs = ["config.h"],
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "turbo",
  srcs = ["main.cc"],
  deps = [
    "//backend:config",
    "//backend/clocktroller",
  ],
  linkopts = [
    "-L/usr/local/lib",
    "-lncurses",
  ],
)
