cc_library(
  name = "memory_message_base",
  hdrs = ["memory_message_base.h"],
  deps = ["//cc/backend/debug:message"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_access",
  hdrs = ["memory_access.h"],
  srcs = ["memory_access.cc"],
  deps = [
    "//cc/backend/debug:publisher",
    ":memory_message_base",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_access_filter",
  hdrs = ["memory_access_filter.h"],
  srcs = ["memory_access_filter.cc"],
  deps = [
    "//cc/backend/debug:filter",
    "//cc/backend/debug:message",
    "//cc/utility",
    ":memory_message_base",
    ":memory_access",
  ],
)

cc_library(
  name = "memory_profiler",
  hdrs = ["memory_profiler.h"],
  srcs = ["memory_profiler.cc"],
  deps = [
    "//cc/backend/debug:consumer",
    "//cc/backend/debug:filter",
    "//cc/utility",
    "//external:glog",
    ":memory_access",
    ":memory_access_filter",
  ],
  visibility = ["//visibility:public"],
)
