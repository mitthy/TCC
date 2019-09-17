#include "gtest/gtest.h"
#include "algorithm/absolute_difference.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

using namespace tcc::algorithm;

TEST( TestAbsoluteDifference, TestSigned ) {
  EXPECT_EQ( absolute_difference( 3, 5 ), 2 );
  EXPECT_EQ( absolute_difference( 5, 3 ), 2 );
  int8_t signed_char_42 = 42;
  int8_t signed_char_7 = 7;
  EXPECT_EQ( absolute_difference( signed_char_42, signed_char_7 ), 35 );
  EXPECT_EQ( absolute_difference( signed_char_7, signed_char_42 ), 35 );
  EXPECT_EQ( absolute_difference( signed_char_42, signed_char_42 ), 0 );
  EXPECT_EQ( absolute_difference( signed_char_7, signed_char_7 ), 0 );
}

TEST( TestAbsoluteDifference, TestZero ) {
  srand( time( nullptr ) );
  int signed_value = rand();
  EXPECT_EQ( absolute_difference( signed_value, 0 ), signed_value );
  EXPECT_EQ( absolute_difference( 0, 0 ), 0 );
  EXPECT_EQ( absolute_difference( 0, signed_value ), signed_value );
  uint64_t unsigned_value = rand();
  EXPECT_EQ( absolute_difference( 0, unsigned_value ), unsigned_value );
  EXPECT_EQ( absolute_difference( unsigned_value, 0 ), unsigned_value );
}

TEST( TestAbsoluteDifference, TestSignedUnsigned ) {
  srand( time( nullptr ) );
  int32_t first = rand();
  uint32_t second = rand();
  EXPECT_EQ( absolute_difference( second, first ), absolute_difference( first, second ) );
}

TEST( TestAbsoluteDifference, TestUnsigned ) {
  uint8_t unsigned_char_42 = 42;
  uint8_t unsigned_char_7 = 7;
  EXPECT_EQ( absolute_difference( unsigned_char_42, unsigned_char_7 ), 35 );
  EXPECT_EQ( absolute_difference( unsigned_char_7, unsigned_char_42 ), 35 );
  EXPECT_EQ( absolute_difference( unsigned_char_42, unsigned_char_42 ), 0 );
  EXPECT_EQ( absolute_difference( unsigned_char_7, unsigned_char_7 ), 0 );
}
