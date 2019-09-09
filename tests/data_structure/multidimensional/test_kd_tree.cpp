#include "gtest/gtest.h"
#include "data_structure/multidimensional/kd_tree.hpp"
#include <vector>
#include <tuple>
#include <random>

using namespace tcc::data_structure;

std::random_device dev;
std::mt19937 rng( dev() );
std::uniform_int_distribution<int> distribution( 1, 100 );

TEST( TestKDTree, TestMeanFunction ) {
  std::vector<std::tuple<int, float>> input_data;
  input_data.reserve( 10 );
  for( int i = 0; i < 10; ++i ) {
    input_data.push_back( std::make_tuple( i, i * 10.0f ) );
  }
}

TEST( TestKDTree, TestCreation ) {
  std::vector<std::tuple<int, int, int>> input_vector;
  input_vector.reserve( 100 );
  for( int i = 0; i < 100; ++i ) {
    input_vector.push_back( std::make_tuple( distribution( rng ), distribution( rng ), distribution( rng ) ) );
  }
  kd_tree<std::tuple<int, int, int>> tree{ input_vector.begin(), input_vector.end() };
}
