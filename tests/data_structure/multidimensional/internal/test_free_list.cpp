#include "gtest/gtest.h"
#include "data_structure/multidimensional/internal/free_list.hpp"
#include <utility>

using free_list_t = tcc::data_structure::__detail__::__free_list__< int >;

TEST( TestFreeList, TestPushSingleElement ) {
  free_list_t list{};
  list.push_back( 42 );
  EXPECT_EQ( list[0], 42 );
}

TEST( TestFreeList, TestPush64Elements ) {
  //Test for 64 cause it should trigger some resizes here and there.
  free_list_t list{};
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( list.push_back( i ), i );
  }
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( list[i], i );
  }
}

TEST( TestFreeList, TestRemove ) {
  free_list_t list{};
  for( int i = 0; i < 64; ++i ) {
    list.push_back( i );
  }
  //Remove some random indexes.
  int indexes[] = { -1, 0, 5, 17, 13, 60, 49, 2, 1, 8 };
  int sz = sizeof( indexes ) / sizeof( int );
  for( int i = 1; i < sz; ++i ) {
    list.remove( indexes[ i ] );
  }
  for( int i = 8, pos = sz - 2; i != -1; --pos ) {
    EXPECT_EQ( list[ i ], indexes[ pos ] );
    i = list[ i ];
  }
}

TEST( TestFreeList, TestRemovePush ) {
  free_list_t list{};
  for( int i = 0; i < 64; ++i ) {
    list.push_back( i );
  }
  //Remove some random indexes.
  int indexes[] = { -1, 0, 5, 17, 13, 60, 49, 2, 1, 8 };
  int sz = sizeof( indexes ) / sizeof( int );
  for( int i = 1; i < sz; ++i ) {
    list.remove( indexes[ i ] );
  }
  EXPECT_EQ( list.push_back( 42 ), 8 );
  EXPECT_EQ( list.push_back( 69 ), 1 );
  EXPECT_EQ( list[ 8 ], 42 );
  EXPECT_EQ( list[ 1 ], 69 );
  EXPECT_EQ( list.m_next_free, 2 );
}

TEST( TestFreeList, TestCopy ) {
  free_list_t list{};
  for( int i = 0; i < 64; ++i ) {
    list.push_back( i );
  }
  {
    //Test inside a new scope so we make sure the destructor for the copy is called on our way back to check for double frees.
    free_list_t copy{ list };
    for( int i = 0; i < 64; ++i ) {
      EXPECT_EQ( copy[ i ], i );
    }
    copy[ 0 ] = 10;
    EXPECT_EQ( copy[ 0 ], 10 );
    EXPECT_EQ( list[ 0 ], 0 );
  }
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( list[ i ], i );
  }
  {
    //Test inside a new scope so we make sure the destructor for the copy is called on our way back to check for double frees.
    free_list_t copy_smaller_than{};
    copy_smaller_than = list;
    for( int i = 0; i < 64; ++i ) {
      EXPECT_EQ( copy_smaller_than[ i ], i );
    }
    copy_smaller_than[ 0 ] = 10;
    EXPECT_EQ( copy_smaller_than[ 0 ], 10 );
    EXPECT_EQ( list[ 0 ], 0 );
  }
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( list[ i ], i );
  }
  int tmp_list_size = list.m_size;
  {
    //Test inside a new scope so we make sure the destructor for the copy is called on our way back to check for double frees.
    free_list_t copy_bigger{};
    for( int i = 0; i < 256; ++i ) {
      copy_bigger.push_back( i );
    }
    int tmp_capacity = copy_bigger.m_capacity;
    int tmp_size = copy_bigger.m_size;
    copy_bigger = list;
    for( int i = 0; i < 64; ++i ) {
      EXPECT_EQ( copy_bigger[ i ], i );
    }
    EXPECT_TRUE( copy_bigger.m_capacity == tmp_capacity );
    EXPECT_FALSE( copy_bigger.m_size == tmp_size );
    copy_bigger[ 0 ] = 10;
    EXPECT_EQ( copy_bigger[ 0 ], 10 );
    EXPECT_EQ( list[ 0 ], 0 );
    for( int i = 0; i < 512; ++i ) {
      copy_bigger.push_back( i );
    }
    EXPECT_TRUE( copy_bigger.m_capacity > tmp_capacity );
  }
  EXPECT_EQ( tmp_list_size, list.m_size );
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( list[ i ], i );
  }
}

TEST( TestFreeList, TestMove ) {
  free_list_t list{};
  {
    free_list_t other{};
    for( int i = 0; i < 64; ++i ) {
      other.push_back( i );
    }
    list = std::move( other );
    ASSERT_TRUE( other.m_data == nullptr );
  }
  EXPECT_EQ( list.m_size, 64 );
  EXPECT_TRUE( list.m_capacity >= 64 );
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( list[ i ], i );
  }
  free_list_t move_into{ std::move( list ) };
  ASSERT_TRUE( list.m_data == nullptr );
  EXPECT_EQ( move_into.m_size, 64 );
  EXPECT_TRUE( move_into.m_capacity >= 64 );
  for( int i = 0; i < 64; ++i ) {
    EXPECT_EQ( move_into[ i ], i );
  }
}
