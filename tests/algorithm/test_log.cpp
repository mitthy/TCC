#include "gtest/gtest.h"
#include "algorithm/log.h"

TEST( TestLog, TestZero ) {
  EXPECT_EQ( geometricks::algorithm::utils::log2( 0 ), -1 );
}

TEST( TestLog, TestOne ) {
  EXPECT_EQ( geometricks::algorithm::utils::log2( 1 ), 0 );
}

TEST( TestLog, TestPowerOfTwoGreaterThanOne ) {
  EXPECT_EQ( geometricks::algorithm::utils::log2( 2 ), 1 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 4 ), 2 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 8 ), 3 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 16 ), 4 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 32 ), 5 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 64 ), 6 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 128 ), 7 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 256 ), 8 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 512 ), 9 );
  EXPECT_EQ( geometricks::algorithm::utils::log2( 1024 ), 10 );
}
