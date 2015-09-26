cc_library(
  name = "logging",
  hdrs = ["logging.h"],
  srcs = ["logging.cc"],
  deps = [
    "//protobuf/backend/logging:message_cc",
    ":message_controller",
  ],
)

cc_library(
  name = "message_controller",
  hdrs = ["message_controller.h"],
  srcs = ["message_controller.cc"],
  deps = [
    "//protobuf/backend/logging:message_cc",
    ":communication_layer",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "communication_layer",
  hdrs = ["communication_layer.h"],
  srcs = ["communication_layer.cc"],
  deps = [
    "//cc/utility",
    "//cc/utility:stream",
    "//protobuf/backend/logging:message_cc",
  ],
  visibility = ["//visibility:public"],
)
