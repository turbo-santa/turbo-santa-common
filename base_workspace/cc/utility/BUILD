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
  deps = [":resizable_ring_buffer"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "ring_buffer",
  hdrs = ["ring_buffer.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "resizable_ring_buffer",
  hdrs = ["resizable_ring_buffer.h"],
  deps = ["//external:glog"],
  visibility = ["//visibility:public"],
)
