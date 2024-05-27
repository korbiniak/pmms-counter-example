#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <fstream>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class TestCommon {
 public:
  static std::string readFile(const std::string& filePath);
};

#endif /* TEST_COMMON_H */
