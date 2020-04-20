#include "gtest/gtest.h"
#include "geometricks/data_structure/internal/small_vector.hpp"

template< typename T, int I>
using small_vector = geometricks::small_vector<T, I>;

template< typename T >
using vector = geometricks::vector<T>;

struct dummy {

  inline static int constructor_calls = 0;

  dummy() {
    ++constructor_calls;
  }

};

template< typename T >
struct retriever {

  T* _1;
  int _2;
  unsigned _3;
  geometricks::allocator _4;
  union small_element_t {

    struct dummy {} _;

    T data;

  } _stack;

};

template< typename T >
bool is_stack( const vector<T>& vec ) {
  vector<T>& non_const = const_cast<vector<T>&>( vec );
  T* stack_ptr = static_cast<T*>( static_cast<void*>( &( ( ( retriever<T>* )( &non_const ) )->_stack ) ) );
  return stack_ptr == vec.data();
}

TEST( TestSmallVector, TestConstruction ) {
  small_vector<dummy, 4> test{};
  EXPECT_EQ( dummy::constructor_calls, 0 );
  test.push_back( dummy{} );
  EXPECT_EQ( dummy::constructor_calls, 1 );
  EXPECT_TRUE( is_stack( test ) );
}

TEST( TestSmallVector, TestPushBack ) {
  small_vector<int, 4> test{};
  EXPECT_TRUE( is_stack( test ) );
  for( int i = 0; i < 64; ++i ) {
    test.push_back( i * 2 );
  }
  EXPECT_FALSE( is_stack( test ) );
  int i = 0;
  for( auto el : test ) {
    EXPECT_EQ( el, i * 2 );
    EXPECT_EQ( el, test.data()[ i ] );
    EXPECT_EQ( el, test[ i ] );
    ++i;
  }
}

TEST( TestSmallVector, TestPushBackPopBack ) {
  small_vector<int, 4> test{};
  for( int i = 0; i < 4; ++i ) {
    test.push_back( i );
  }
  test.pop_back();
  test.push_back( 42 );
  EXPECT_TRUE( is_stack( test ) );
  EXPECT_EQ( test[ 0 ], 0 );
  EXPECT_EQ( test[ 1 ], 1 );
  EXPECT_EQ( test[ 2 ], 2 );
  EXPECT_EQ( test[ 3 ], 42 );
  EXPECT_EQ( test.size(), 4 );
  test.push_back( 30 );
  EXPECT_FALSE( is_stack( test ) );
  EXPECT_EQ( test[ 0 ], 0 );
  EXPECT_EQ( test[ 1 ], 1 );
  EXPECT_EQ( test[ 2 ], 2 );
  EXPECT_EQ( test[ 3 ], 42 );
  EXPECT_EQ( test[ 4 ], 30 );
  EXPECT_EQ( test.size(), 5 );
  for( int i = 0; i < 5; ++i ) {
    test.push_back( 100 - i );
  }
  for( int i = 0; i < 3; ++i ) {
    test.pop_back();
  }
  EXPECT_EQ( test.size(), 7 );
  EXPECT_EQ( test[ test.size() - 1 ], 99 );
}

TEST( TestSmallVector, TestErase ) {
  small_vector<int, 4> test{};
  test.reserve( 10 );
  test.set_size( 10 );
  for( int i = 0; i < 10; ++i ) {
    test[ i ] = i;
  }
  auto it = test.erase( test.begin() + 1 );
  EXPECT_EQ( test[ 0 ], 0 );
  EXPECT_EQ( *it, 2 );
  EXPECT_EQ( test.size(), 9 );
  for( int i = 1; i < 9; ++i ) {
    EXPECT_EQ( test[ i ], i + 1 );
  }
  it = test.erase( test.begin() + 2, test.begin() + 5 );
  EXPECT_EQ( test.size(), 6 );
  EXPECT_EQ( test[ 0 ], 0 );
  EXPECT_EQ( test[ 1 ], 2 );
  EXPECT_EQ( *it, 6 );
  for( int i = 2; i < 6; ++i ) {
    EXPECT_EQ( test[ i ], i + 4 );
  }
}

TEST( TestSmallVector, TestCopyAssignment ) {
  small_vector<int, 0> test{};
  for( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }
  small_vector<int, 4> other;
  other = test;
  for( int i = 0; i < 10; ++i ) {
    ++other[ i ];
    EXPECT_EQ( other[ i ], i + 1 );
    EXPECT_FALSE( other[ i ] == test[ i ] );
  }
}

TEST( TestSmallVector, TestReserve ) {
  small_vector<int, 4> test{};
  EXPECT_TRUE( is_stack( test ) );
  test.reserve( 4 );
  EXPECT_TRUE( is_stack( test ) );
  test.reserve( 255 );
  EXPECT_FALSE( is_stack( test ) );
}
