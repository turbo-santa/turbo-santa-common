cc_library(
  name = "utility",
  hdrs = ["option.h"],
  deps = ["//external:glog"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "queue",
  hdrs = ["queue.h"],
  deps = ["//external:glog"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "stream",
  hdrs = ["stream.h"],
  deps = [":queue"],
  visibility = ["//visibility:public"],
)
