#include <stdio.h>

#include "submodules/glog/src/glog/logging.h"
#include "submodules/googletest/include/gtest/gtest.h"

int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  google::InitGoogleLogging(argv[0]);
  return RUN_ALL_TESTS();
}
