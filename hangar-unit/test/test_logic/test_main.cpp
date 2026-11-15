// test/test_logic/test_main.cpp
#include <unity.h>
#include "ConsecutiveFilter.h"

void setUp() {}
void tearDown() {}

void test_single_glitch_is_ignored() {
  ConsecutiveFilter f(false, 3, 3);
  f.update(true);
  TEST_ASSERT_FALSE(f.update(false));
}

void test_changes_after_three_readings() {
  ConsecutiveFilter f(false, 3, 3);
  f.update(true);
  f.update(true);
  TEST_ASSERT_TRUE(f.update(true));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_single_glitch_is_ignored);
  RUN_TEST(test_changes_after_three_readings);
  return UNITY_END();
}