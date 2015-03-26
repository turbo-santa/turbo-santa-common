package(default_visibility = ["//visibility:public"])

cc_library(
  name = "googletest",
  hdrs = [
    "googletest/include/gtest/gtest-death-test.h",
    "googletest/include/gtest/gtest-spi.h",
    "googletest/include/gtest/gtest-message.h",
    "googletest/include/gtest/gtest-typed-test.h",
    "googletest/include/gtest/gtest-printers.h",
    "googletest/include/gtest/gtest.h",
    "googletest/include/gtest/gtest-test-part.h",
    "googletest/include/gtest/gtest_pred_impl.h",
    "googletest/include/gtest/gtest_prod.h",
    "googletest/include/gtest/gtest-param-test.h",
  ],
  srcs = [
    "googletest/lib/.libs/libgtest_main.a",
    "googletest/lib/.libs/libgtest.a",
  ],
  includes = ["googletest/include"],
  linkopts = ["-pthread"],
  # deps = [":my-gtest-lib"],
  linkstatic = 1,
)

cc_library(
  name = "glog",
  hdrs = [
    "glog/src/glog/vlog_is_on.h",
    "glog/src/glog/stl_logging.h",
    "glog/src/glog/logging.h",
    "glog/src/glog/log_severity.h",
    "glog/src/glog/raw_logging.h",
  ],
  srcs = [
    "glog/.libs/libglog.a",
  ],
  linkstatic = 1,
)
