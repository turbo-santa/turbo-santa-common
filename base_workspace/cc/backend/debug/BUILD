cc_library(
  name = "message",
  hdrs = ["message.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "publisher",
  hdrs = ["publisher.h"],
  srcs = ["publisher.cc"],
  deps = [":message"],
  defines = ["DEBUG"], # Comment out this line to turn debugging off.
  visibility = ["//visibility:public"],
)

cc_library(
  name = "master",
  hdrs = ["master.h"],
  srcs = ["master.cc"],
  deps = [
    ":consumer",
    ":consumer_runner",
    ":message",
    ":publisher",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "filter",
  hdrs = ["filter.h"],
  deps = [
    "//cc/utility",
    "//cc/utility:stream",
    ":message",
  ],
  visibility = [":__subpackages__"],
)

cc_library(
  name = "consumer",
  hdrs = ["consumer.h"],
  deps = [":filter"],
  visibility = [":__subpackages__"],
)

cc_library(
  name = "consumer_runner",
  hdrs = ["consumer_runner.h"],
  srcs = ["consumer_runner.cc"],
  deps = [":consumer"],
  linkopts = ["-pthread"],
)
