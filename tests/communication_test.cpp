#include <gtest/gtest.h>

#include "helpers.hpp"

class CommunicationTests : public ::testing::Test {
protected:
  void SetUp() override { ::testing::Test::SetUp(); }
  void TearDown() override { ::testing::Test::TearDown(); }
};

TEST_F(CommunicationTests, test1) {
  TestCommunication g_testCommunication{};
  auto              result = g_testCommunication.getLastValues();
  EXPECT_EQ(3, result.size());
}
