#include "gtest/gtest.h"
#include "algorithm/internal/mean.hpp"
#include<vector>

TEST( TestMean, TestMeanFunction ) {
  std::vector<int> input_data;
  input_data.reserve( 9 );
  tcc::algorithm::mean<int> mean_calculator;
  for( int i = 1; i < 10; ++i ) {
    input_data.push_back( i );
  }
  EXPECT_EQ( mean_calculator( input_data.begin(), input_data.end() ), 5 );
}
