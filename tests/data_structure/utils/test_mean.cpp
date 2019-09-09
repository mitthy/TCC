#include "gtest/gtest.h"
#include "data_structure/utils/mean.hpp"
#include<vector>

TEST( TestMean, TestMeanFunction ) {
  std::vector<std::tuple<int, float>> input_data;
  input_data.reserve( 10 );
  for( int i = 0; i < 10; ++i ) {
    input_data.push_back( std::make_tuple( i, i * 10.0f ) );
  }
}
