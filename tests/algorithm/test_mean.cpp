#include "gtest/gtest.h"
#include "geometricks/algorithm/mean.hpp"
#include<vector>

TEST( TestMean, TestMeanFunction ) {
  std::vector<int> input_data;
  input_data.reserve( 9 );
  for( int i = 1; i < 10; ++i ) {
    input_data.push_back( i );
  }
  EXPECT_EQ( geometricks::algorithm::mean( input_data.begin(), input_data.end() ), 5 );
}
