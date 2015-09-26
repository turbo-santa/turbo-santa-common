java_library(
  name = "consumer_controller",
  srcs = ["ConsumerController.java"],
  deps = [
    "//external:guava",
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
    ":abstract_consumer_factory",
    ":consumer",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "abstract_consumer_factory",
  srcs = ["AbstractConsumerFactory.java"],
  deps = [
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
    ":consumer",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "consumer",
  srcs = ["Consumer.java"],
  visibility = ["//visibility:public"],
)

java_library(
  name = "communication_layer",
  srcs = ["CommunicationLayer.java"],
  deps = [
    "//external:guava",
    "//external:protobuf",
    "//protobuf/backend/logging:message_java",
  ],
  data = select({
    "//:darwin": ["libcommunication_layerjni.dylib"],
    "//conditions:default": ["libcommunication_layerjni.so"],
  }),
)

genrule(
  name = "mac-compat-communication_layer",
  srcs = ["libcommunication_layerjni.so"],
  outs = ["libcommunication_layerjni.dylib"],
  cmd = "cp $< $@",
)

cc_binary(
  name = "libcommunication_layerjni.so",
  srcs = [
    "com_turbosanta_backend_logging_CommunicationLayer.h",
    "com_turbosanta_backend_logging_CommunicationLayer.cc",
  ],
  deps = [
    "//cc/backend/logging:communication_layer",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  linkshared = 1,
)

java_library(
  name = "message_controller",
  srcs = ["MessageController.java"],
  deps = [":communication_layer"],
  data = select({
    "//:darwin": ["libmessage_controllerjni.dylib"],
    "//conditions:default": ["libmessage_controllerjni.so"],
  })
)

genrule(
  name = "mac-compat-message_controller",
  srcs = ["libmessage_controllerjni.so"],
  outs = ["libmessage_controllerjni.dylib"],
  cmd = "cp $< $@",
)

cc_binary(
  name = "libmessage_controllerjni.so",
  srcs = [
    "com_turbosanta_backend_logging_MessageController.h",
    "com_turbosanta_backend_logging_MessageController.cc",
  ],
  deps = [
    "//cc/backend/logging:message_controller",
    "//java/com/turbosanta/backend:jni_headers",
  ],
  linkshared = 1,
)
