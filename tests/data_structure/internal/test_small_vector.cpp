#include "gtest/gtest.h"
#include "geometricks/data_structure/internal/small_vector.hpp"

template< typename T, int I>
using small_vector = geometricks::data_structure::small_vector<T, I>;

template< typename T >
using vector = geometricks::data_structure::vector<T>;

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

TEST( TestSmallVector, TestReserve ) {
  small_vector<int, 4> test{};
  EXPECT_TRUE( is_stack( test ) );
  test.reserve( 4 );
  EXPECT_TRUE( is_stack( test ) );
  test.reserve( 255 );
  EXPECT_FALSE( is_stack( test ) );
}
