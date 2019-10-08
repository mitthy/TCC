#include "gtest/gtest.h"
#include "algorithm/absolute_difference.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

using namespace tcc::algorithm;

TEST( TestAbsoluteDifference, TestABS ) {
  EXPECT_EQ( tcc::algorithm::abs( 4u ), 4u );
  EXPECT_EQ( tcc::algorithm::abs( 4 ), 4u );
  EXPECT_EQ( tcc::algorithm::abs( std::numeric_limits<int>::max() ), static_cast<uint64_t>( std::numeric_limits<int>::max() ) );
}

TEST( TestAbsoluteDifference, TestSigned ) {
  EXPECT_EQ( absolute_difference( 3, 5 ), 2u );
  EXPECT_EQ( absolute_difference( 5, 3 ), 2u );
  int8_t signed_char_42 = 42;
  int8_t signed_char_7 = 7;
  EXPECT_EQ( absolute_difference( signed_char_42, signed_char_7 ), 35u );
  EXPECT_EQ( absolute_difference( signed_char_7, signed_char_42 ), 35u );
  EXPECT_EQ( absolute_difference( signed_char_42, signed_char_42 ), 0u );
  EXPECT_EQ( absolute_difference( signed_char_7, signed_char_7 ), 0u );
}

TEST( TestAbsoluteDifference, TestZero ) {
  srand( time( nullptr ) );
  int signed_value = tcc::algorithm::abs( rand() );
  EXPECT_EQ( absolute_difference( signed_value, 0 ), ( unsigned ) signed_value );
  EXPECT_EQ( absolute_difference( 0, 0 ), 0u );
  EXPECT_EQ( absolute_difference( 0, signed_value ), ( unsigned ) signed_value );
  uint64_t unsigned_value = rand();
  EXPECT_EQ( absolute_difference( 0ul, unsigned_value ), unsigned_value );
  EXPECT_EQ( absolute_difference( unsigned_value, 0ul ), unsigned_value );
}

TEST( TestAbsoluteDifference, TestUnsigned ) {
  uint8_t unsigned_char_42 = 42;
  uint8_t unsigned_char_7 = 7;
  EXPECT_EQ( absolute_difference( unsigned_char_42, unsigned_char_7 ), 35u );
  EXPECT_EQ( absolute_difference( unsigned_char_7, unsigned_char_42 ), 35u );
  EXPECT_EQ( absolute_difference( unsigned_char_42, unsigned_char_42 ), 0u );
  EXPECT_EQ( absolute_difference( unsigned_char_7, unsigned_char_7 ), 0u );
}
