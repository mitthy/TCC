#include "gtest/gtest.h"
#include "geometricks/data_structure/kd_tree.hpp"
#include <vector>
#include <tuple>
#include <array>

using namespace geometricks;

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

namespace dummy {

  struct dummy {

    int first;

    int second;

    int third;

    constexpr bool operator==( const dummy& other ) const {
      return first == other.first && second == other.second && third == other.third;
    }

  };

}

namespace geometricks::dimension::get_customization {

  template<>
  struct get<dummy::dummy> {

    static int _( const dummy::dummy& d, geometricks::dimension::dimension_t<0> ) {
      return d.first;
    }

    static int _( const dummy::dummy& d, geometricks::dimension::dimension_t<1> ) {
      return d.second;
    }

    static int _( const dummy::dummy& d, geometricks::dimension::dimension_t<2> ) {
      return d.third;
    }
  };

}

namespace geometricks::dimension {

  template<>
  struct dimensional_traits<dummy::dummy> {
    static constexpr int dimensions = 3;
  };

}

TEST( TestKDTree, TestDataCustomization ) {
  std::vector<dummy::dummy> input_vector;
  input_vector.reserve( 10 );
  input_vector.push_back( { 50, 50, 50 } );
  input_vector.push_back( { 0, 49, 87 } );
  input_vector.push_back( { 13, 11, 12 } );
  input_vector.push_back( { 10, 9, 11 } );
  input_vector.push_back( { 49, 50, 53 } );
  input_vector.push_back( { 50, 50, 54 } );
  input_vector.push_back( { 48, 50, 49 } );
  input_vector.push_back( { 44, 78, 67 } );
  input_vector.push_back( { 20, 24, 23 } );
  input_vector.push_back( { 22, 22, 22 } );
  kd_tree<dummy::dummy> tree( input_vector.begin(), input_vector.end() );
  {
    auto [nearest, distance] = tree.nearest_neighbor( { 10, 10, 10 } );
    auto expected = dummy::dummy{ 10, 9, 11 };
    EXPECT_EQ( nearest, expected );
    EXPECT_EQ( distance, 2u );
  }
}

TEST( TestKDTree, TestRangeSearch ) {
  std::vector<std::tuple<int, int, int>> input_vector;
  input_vector.reserve( 10 );
  input_vector.push_back( std::make_tuple( 50, 50, 30 ) );
  input_vector.push_back( std::make_tuple( 0, 49, 87 ) );
  input_vector.push_back( std::make_tuple( 13, 11, 12 ) );
  input_vector.push_back( std::make_tuple( 10, 9, 11 ) );
  input_vector.push_back( std::make_tuple( 49, 50, 53 ) );
  input_vector.push_back( std::make_tuple( 50, 50, 54 ) );
  input_vector.push_back( std::make_tuple( 48, 50, 90 ) );
  input_vector.push_back( std::make_tuple( 44, 78, 67 ) );
  input_vector.push_back( std::make_tuple( 20, 24, 23 ) );
  input_vector.push_back( std::make_tuple( 22, 22, 22 ) );
  for( int i = 0; i < 200000; ++i ) {
    int randx = rand() % 20000 ;
    int randy = rand() % 20000 ;
    int randz = rand() % 20000 ;
    input_vector.push_back( std::make_tuple( 1000 + i * randx, 1000 + i * randy, 1000 + i * randz ) );
  }
  kd_tree<std::tuple<int,int,int>> tree( input_vector.begin(), input_vector.end(), geometricks::default_compare, geometricks::memory::malloc_allocator );
  auto output_vector = tree.range_search( std::make_tuple( 0, 60, 20 ), std::make_tuple( 100, 40, 53 ) );
  //TODO: make this test more robust.
  EXPECT_EQ( output_vector.size(), 2u );
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
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 10, 10, 10 ), 5 )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 70, 60, 80 ), 5 )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 52, 52, 54 ), 5 )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 52, 52, 54 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    auto [nearestK, distanceK] = tree.k_nearest_neighbor( std::make_tuple( 49, 49, 52 ), 5 )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 49, 49, 52 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
  {
    auto output_vector = tree.k_nearest_neighbor( std::make_tuple( 21, 23, 24 ), 5 );
    auto [nearestK, distanceK] = output_vector[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 21, 23, 24 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
    EXPECT_EQ( output_vector.size(), 5u );
  }
  {
    auto [nearestK, distanceK] = tree.k_nearest_neighbor(std::make_tuple( 0, 49, 87 ), 5 )[ 0 ];
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 0, 49, 87 ) );
    ( void ) nearestK;
    ( void ) nearest;
    EXPECT_EQ( distanceK, distance );
  }
}

TEST( TestKDTree, TestCopyConstructor ) {
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
  kd_tree<std::tuple<int, int, int>> copy_constructed{ tree };
  {
    auto [nearestCopy, distanceCopy] = copy_constructed.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    EXPECT_EQ( distanceCopy, distance );
    EXPECT_EQ( nearestCopy, nearest );
  }
  {
    auto [nearestCopy, distanceCopy] = copy_constructed.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    EXPECT_EQ( distanceCopy, distance );
    EXPECT_EQ( nearestCopy, nearest );
  }
}

TEST( TestKDTree, TestMoveConstructor ) {
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
  kd_tree<std::tuple<int, int, int>> move_constructed{ std::move( tree ) };
  {
    auto [nearest, distance] = move_constructed.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    EXPECT_EQ( nearest, std::make_tuple( 10, 9, 11 ) );
    EXPECT_EQ( distance, 2u );
  }
  {
    auto [nearest, distance] = move_constructed.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    EXPECT_EQ( nearest, std::make_tuple( 44, 78, 67 ) );
    EXPECT_EQ( distance, 1169u );
  }
}

TEST( TestKDTree, TestCopyAssignment ) {
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
  tree = tree; //Write this stupid line cause we also gotta test self assignment.
  std::vector<std::tuple<int,int,int>> input_vector_2;
  input_vector_2.push_back( std::make_tuple( 0, 0, 0 ) );
  kd_tree<std::tuple<int, int, int>> copy_assigned( input_vector_2.begin(), input_vector_2.end() );
  copy_assigned = tree;
  {
    auto [nearestCopy, distanceCopy] = copy_assigned.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    EXPECT_EQ( distanceCopy, distance );
    EXPECT_EQ( nearestCopy, nearest );
  }
  {
    auto [nearestCopy, distanceCopy] = copy_assigned.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    EXPECT_EQ( distanceCopy, distance );
    EXPECT_EQ( nearestCopy, nearest );
  }
}

TEST( TestKDTree, TestMoveAssignment ) {
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
  tree = std::move( tree ); //Write this stupid line cause we also gotta test self assignment.
  std::vector<std::tuple<int,int,int>> input_vector_2;
  input_vector_2.push_back( std::make_tuple( 0, 0, 0 ) );
  kd_tree<std::tuple<int, int, int>> move_assigned( input_vector_2.begin(), input_vector_2.end() );
  move_assigned = std::move( tree );
  {
    auto [nearest, distance] = move_assigned.nearest_neighbor( std::make_tuple( 10, 10, 10 ) );
    EXPECT_EQ( nearest, std::make_tuple( 10, 9, 11 ) );
    EXPECT_EQ( distance, 2u );
  }
  {
    auto [nearest, distance] = move_assigned.nearest_neighbor( std::make_tuple( 70, 60, 80 ) );
    EXPECT_EQ( nearest, std::make_tuple( 44, 78, 67 ) );
    EXPECT_EQ( distance, 1169u );
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

TEST( TestKDTree, TestNearestNeighborCustomFunction ) {
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
