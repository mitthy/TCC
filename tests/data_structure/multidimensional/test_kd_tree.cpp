#include "gtest/gtest.h"
#include "data_structure/multidimensional/kd_tree.hpp"
#include <vector>
#include <tuple>

using namespace tcc::data_structure;

TEST( TestKDTree, TestNearestNeighborCorrectness ) {
  std::vector<std::tuple<int, int, int>> input_vector;
  input_vector.reserve( 10 );
  input_vector.push_back( std::make_tuple( 50, 50, 50 ) );
  input_vector.push_back( std::make_tuple( 0, 49, 87 ) );
  input_vector.push_back( std::make_tuple( 13, 11, 12 ) );
  input_vector.push_back( std::make_tuple( 10, 9, 11 ) );
  input_vector.push_back( std::make_tuple( 49, 50, 53 ) );
  input_vector.push_back( std::make_tuple( 50, 50, 54 ) );
  input_vector.push_back( std::make_tuple( 48, 50, 49 ) );
  input_vector.push_back( std::make_tuple( 44, 78, 67 ) );
  input_vector.push_back( std::make_tuple( 20, 24, 23 ) );
  input_vector.push_back( std::make_tuple( 22, 22, 22 ) );
  kd_tree<std::tuple<int, int, int>> tree{ input_vector.begin(), input_vector.end() };
  {
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    EXPECT_EQ( nearest, std::make_tuple( 10, 9, 11 ) );
    EXPECT_EQ( distance, 2u );
  }
  {
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    EXPECT_EQ( nearest, std::make_tuple( 44, 78, 67 ) );
    EXPECT_EQ( distance, 1169u );
  }
  {
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 52, 52, 54 ) );
    EXPECT_EQ( nearest, std::make_tuple( 50, 50, 54 ) );
    EXPECT_EQ( distance, 8u );
  }
  {
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 49, 49, 52 ) );
    EXPECT_EQ( nearest, std::make_tuple( 49, 50, 53 )  );
    EXPECT_EQ( distance, 2u );
  }
  {
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 21, 23, 24 ) );
    EXPECT_EQ( nearest, std::make_tuple( 20, 24, 23 ) );
    EXPECT_EQ( distance, 3u );
  }
  {
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 0, 49, 87 ) );
    EXPECT_EQ( nearest, std::make_tuple( 0, 49, 87 ) );
    EXPECT_EQ( distance, 0u );
  }
}

TEST( TestKDTree, TestKNearestNeighborSingleElement ) {
  std::vector<std::tuple<int, int, int>> input_vector;
  input_vector.reserve( 10 );
  input_vector.push_back( std::make_tuple( 50, 50, 50 ) );
  input_vector.push_back( std::make_tuple( 0, 49, 87 ) );
  input_vector.push_back( std::make_tuple( 13, 11, 12 ) );
  input_vector.push_back( std::make_tuple( 10, 9, 11 ) );
  input_vector.push_back( std::make_tuple( 49, 50, 53 ) );
  input_vector.push_back( std::make_tuple( 50, 50, 54 ) );
  input_vector.push_back( std::make_tuple( 48, 50, 49 ) );
  input_vector.push_back( std::make_tuple( 44, 78, 67 ) );
  input_vector.push_back( std::make_tuple( 20, 24, 23 ) );
  input_vector.push_back( std::make_tuple( 22, 22, 22 ) );
  kd_tree<std::tuple<int, int, int>> tree{ input_vector.begin(), input_vector.end() };
  {
    std::vector<std::pair<std::tuple<int, int, int>, size_t>> output_vector;
    output_vector.reserve( 1 );
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 10, 10, 10 ), 1, output_vector )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    std::vector<std::pair<std::tuple<int, int, int>, size_t>> output_vector;
    output_vector.reserve( 1 );
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 70, 60, 80 ), 1, output_vector )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    std::vector<std::pair<std::tuple<int, int, int>, size_t>> output_vector;
    output_vector.reserve( 1 );
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 52, 52, 54 ), 1, output_vector )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 52, 52, 54 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    std::vector<std::pair<std::tuple<int, int, int>, size_t>> output_vector;
    output_vector.reserve( 1 );
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 49, 49, 52 ), 1, output_vector )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 49, 49, 52 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    std::vector<std::pair<std::tuple<int, int, int>, size_t>> output_vector;
    output_vector.reserve( 1 );
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 21, 23, 24 ), 1, output_vector )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 21, 23, 24 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    std::vector<std::pair<std::tuple<int, int, int>, size_t>> output_vector;
    output_vector.reserve( 1 );
    auto [nearestK, distanceK] = tree.k_nearest_neighbor(std::make_tuple( 0, 49, 87 ), 1, output_vector )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 0, 49, 87 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
}
