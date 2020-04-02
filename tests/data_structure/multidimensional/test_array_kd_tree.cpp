#include "gtest/gtest.h"
#include "data_structure/multidimensional/array_kd_tree.hpp"
#include <vector>
#include <tuple>

template< typename T >
using kd_tree = geometricks::data_structure::array_kd_tree<T>;

using namespace geometricks;

TEST( TestArrayKDTree, TestNearestNeighborCorrectness ) {
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
  for( int i = 0; i < 200000; ++i ) {
    int randx = rand() % 20000 ;
    int randy = rand() % 20000 ;
    int randz = rand() % 20000 ;
    input_vector.push_back( std::make_tuple( 1000 + i * randx, 1000 + i * randy, 1000 + i * randz ) );
  }
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
  for( int i = 0; i < 20000; ++i ) {
    int randx = rand() % 500000;
    int randy = rand() % 500000;
    int randz = rand() % 500000;
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( randx, randy, randz ) );
    ( void ) nearest;
    ( void ) distance;
  }
}

TEST( TestArrayKDTree, TestKNearestNeighborSingleElement ) {
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

struct custom_nearest_neghbor_function {

  size_t operator()( const std::tuple<int,int,int>& left, const std::tuple<int,int,int>& right ) {
    return  std::sqrt(
              algorithm::absolute_difference( std::get<0>( left ), std::get<0>( right ) ) * algorithm::absolute_difference( std::get<0>( left ), std::get<0>( right ) ) +
              algorithm::absolute_difference( std::get<1>( left ), std::get<1>( right ) ) * algorithm::absolute_difference( std::get<1>( left ), std::get<1>( right ) ) +
              algorithm::absolute_difference( std::get<2>( left ), std::get<2>( right ) ) * algorithm::absolute_difference( std::get<2>( left ), std::get<2>( right ) )
            );
  }

  static int32_t calls_int_int;

  static int32_t calls_int_int_dim1;

  static int32_t calls_int_int_dim2;

  static int32_t calls_tuple_int_dim2;

  size_t operator()( int left, int right ) {
    calls_int_int = 1;
    return algorithm::absolute_difference( left, right );
  }

  size_t operator()( int left, int right, dimension::dimension_t<1> ) {
    calls_int_int_dim1 = 1;
    return algorithm::absolute_difference( left, right );
  }

  size_t operator()( int left, int right, dimension::dimension_t<2> ) {
    calls_int_int_dim2 = 1;
    return algorithm::absolute_difference( left, right );
  }

  size_t operator()( const std::tuple<int, int, int>& left, int right, dimension::dimension_t<2> ) {
    calls_tuple_int_dim2 = 1;
    return algorithm::absolute_difference( std::get<2>( left ), right );
  }

};

int custom_nearest_neghbor_function::calls_int_int = 0;
int custom_nearest_neghbor_function::calls_int_int_dim1 = 0;
int custom_nearest_neghbor_function::calls_int_int_dim2 = 0;
int custom_nearest_neghbor_function::calls_tuple_int_dim2 = 0;

TEST( TestArrayKDTree, TestNearestNeighborCustomFunction ) {
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
  for( int i = 0; i < 20000; ++i ) {
    int randx = rand() % 20000 ;
    int randy = rand() % 20000 ;
    int randz = rand() % 20000 ;
    input_vector.push_back( std::make_tuple( 1000 + i * randx, 1000 + i * randy, 1000 + i * randz ) );
  }
  kd_tree<std::tuple<int, int, int>> tree{ input_vector.begin(), input_vector.end() };
  auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ), custom_nearest_neghbor_function{} );
  EXPECT_EQ( nearest, std::make_tuple( 10, 9, 11 ) );
  EXPECT_EQ( custom_nearest_neghbor_function::calls_int_int, 1 );
  EXPECT_EQ( custom_nearest_neghbor_function::calls_int_int_dim1, 1 );
  EXPECT_EQ( custom_nearest_neghbor_function::calls_int_int_dim2, 0 );
  EXPECT_EQ( custom_nearest_neghbor_function::calls_tuple_int_dim2, 1 );
  ( void )distance;
}
