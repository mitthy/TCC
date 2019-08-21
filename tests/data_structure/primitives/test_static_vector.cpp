#include "gtest/gtest.h"
#include "data_structure/primitives/static_vector.hpp"

TEST( TestStaticVector, TestSafeEmpty ) {
  tcc::data_structure::safe_static_vector<int, 32> empty{};
  EXPECT_TRUE( empty.is_empty() );
  EXPECT_EQ( empty.size(), 0 );
}

TEST( TestStaticVector, TestSafeInsert ) {
  tcc::data_structure::safe_static_vector<int, 1> one_element_vec{};
  EXPECT_EQ( one_element_vec.push_back( 3 ), 0 );
  EXPECT_FALSE( one_element_vec.is_empty() );
  EXPECT_EQ( one_element_vec[0], 3 );
  EXPECT_EQ( one_element_vec.size(), 1 );
  EXPECT_EQ( one_element_vec.push_back( 5 ), -1 ); 
}
