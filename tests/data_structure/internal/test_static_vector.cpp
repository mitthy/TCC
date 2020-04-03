#include "gtest/gtest.h"
#include "geometricks/data_structure/internal/static_vector.hpp"

TEST( TestStaticVector, TestSafeEmpty ) {
  geometricks::data_structure::safe_static_vector<int, 32> empty{};
  EXPECT_TRUE( empty.is_empty() );
  EXPECT_EQ( empty.size(), 0 );
}

TEST( TestStaticVector, TestSafeInsert ) {
  geometricks::data_structure::safe_static_vector<int, 1> one_element_vec{};
  EXPECT_EQ( one_element_vec.push_back( 3 ), 0 );
  EXPECT_FALSE( one_element_vec.is_empty() );
  EXPECT_EQ( one_element_vec[0], 3 );
  EXPECT_EQ( one_element_vec.size(), 1 );
  EXPECT_EQ( one_element_vec.push_back( 5 ), -1 );
}

TEST( TestStaticVector, TestSafePopEmptyVector ) {
  geometricks::data_structure::safe_static_vector<int, 32> empty{};
  EXPECT_EQ( empty.pop(), -1 );
  EXPECT_EQ( empty.push_back( 666 ), 0 );
  EXPECT_EQ( empty[0], 666 ); //Ensure no side effects.
  EXPECT_EQ( empty.pop(), 0 );
}

TEST( TestStaticVector, TestSafePopPopulatedVector ) {
  geometricks::data_structure::safe_static_vector<int, 4> vec{};
  EXPECT_EQ( vec.push_back( 1, 2, 3, 4 ), 0 );
  EXPECT_EQ( vec.push_back( 5 ), -1 );
  EXPECT_EQ( vec.pop(), 0 );
  EXPECT_EQ( vec.push_back( 5 ), 0 );
  EXPECT_EQ( vec[3], 5 );
}

struct not_trivially_destructible {
  static int m_calls;

  ~not_trivially_destructible() {
    ++not_trivially_destructible::m_calls;
  }
};

int not_trivially_destructible::m_calls = 0;

TEST( TestStaticVector, TestDestructorCalls ) {
  geometricks::data_structure::safe_static_vector<not_trivially_destructible, 2> outer;
  not_trivially_destructible copy_me;
  outer.push_back(copy_me, copy_me);
  EXPECT_EQ( not_trivially_destructible::m_calls, 0 );
  {
    geometricks::data_structure::safe_static_vector<not_trivially_destructible, 42> inner;
    for( int i = 0; i < 38; ++i ) {
      EXPECT_EQ( inner.push_back( copy_me ), 0 );
    }
    EXPECT_EQ( not_trivially_destructible::m_calls, 0 );
  }
  EXPECT_EQ( not_trivially_destructible::m_calls, 38 );
}

TEST( TestStaticVector, TestCopyConstructor ) {
  geometricks::data_structure::safe_static_vector<int, 8> original;
  original.push_back( 1, 2, 3, 4 );
  geometricks::data_structure::safe_static_vector<int, 8> copy{ original };
  for( int i = 0; i < 4; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( original[ i ], copy[ i ] );
    copy[ i ]++;
  }
  for( int i = 0; i < 4; ++i ) {
    //After modifying the copy, it shouldn't be equal to the original anymore...
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
    EXPECT_FALSE( original[ i ] == copy[ i ] );
  }
  //Assert we copied the correct number of elements. 4 remaining...
  EXPECT_EQ( copy.push_back( 6, 7, 8, 9 ), 0 );
  EXPECT_EQ( copy.push_back( -1 ), -1 );
  //Changing the copy shouldn't change the original vector and vice-versa. Check the original vector pointers.
  EXPECT_EQ( original.push_back( 5, 6, 7, 8 ), 0 );
  EXPECT_EQ( original.push_back( -1 ), -1 );
  //Compare elements. Should all be different.
  for( int i = 0; i < 8; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
  }

}

TEST( TestStaticVector, TestCopyAssignment ) {
  geometricks::data_structure::safe_static_vector<int, 8> original;
  original.push_back( 1, 2, 3, 4 );
  geometricks::data_structure::safe_static_vector<int, 8> copy;
  copy = original;
  for( int i = 0; i < 4; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( original[ i ], copy[ i ] );
    copy[ i ]++;
  }
  for( int i = 0; i < 4; ++i ) {
    //After modifying the copy, it shouldn't be equal to the original anymore...
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
    EXPECT_FALSE( original[ i ] == copy[ i ] );
  }
  //Assert we copied the correct number of elements. 4 remaining...
  EXPECT_EQ( copy.push_back( 6, 7, 8, 9 ), 0 );
  EXPECT_EQ( copy.push_back( -1 ), -1 );
  //Changing the copy shouldn't change the original vector and vice-versa. Check the original vector pointers.
  EXPECT_EQ( original.push_back( 5, 6, 7, 8 ), 0 );
  EXPECT_EQ( original.push_back( -1 ), -1 );
  //Compare elements. Should all be different.
  for( int i = 0; i < 8; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
  }
}

TEST( TestStaticVector, TestCopySelfAssignment ) {
  geometricks::data_structure::safe_static_vector<int, 8> original;
  original.push_back( 1, 2, 3, 4 );
  EXPECT_EQ( original.size(), 4 );
  for( int i = 0; i < 4; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
  }
  original = original;
  EXPECT_EQ( original.size(), 4 );
  for( int i = 0; i < 4; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
  }
}

TEST( TestStaticVector, TestMoveConstructor ) {
  //Should trigger a copy here. Just repeat copy test.
  geometricks::data_structure::safe_static_vector<int, 8> original;
  original.push_back( 1, 2, 3, 4 );
  geometricks::data_structure::safe_static_vector<int, 8> copy{ std::move( original ) };
  for( int i = 0; i < 4; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( original[ i ], copy[ i ] );
    copy[ i ]++;
  }
  for( int i = 0; i < 4; ++i ) {
    //After modifying the copy, it shouldn't be equal to the original anymore...
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
    EXPECT_FALSE( original[ i ] == copy[ i ] );
  }
  //Assert we copied the correct number of elements. 4 remaining...
  EXPECT_EQ( copy.push_back( 6, 7, 8, 9 ), 0 );
  EXPECT_EQ( copy.push_back( -1 ), -1 );
  //Changing the copy shouldn't change the original vector and vice-versa. Check the original vector pointers.
  EXPECT_EQ( original.push_back( 5, 6, 7, 8 ), 0 );
  EXPECT_EQ( original.push_back( -1 ), -1 );
  //Compare elements. Should all be different.
  for( int i = 0; i < 8; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
  }
}

TEST( TestStaticVector, TestMoveAssignment ) {
  //Should trigger a copy here. Just repeat copy test.
  geometricks::data_structure::safe_static_vector<int, 8> original;
  original.push_back( 1, 2, 3, 4 );
  geometricks::data_structure::safe_static_vector<int, 8> copy;
  copy = std::move( original );
  for( int i = 0; i < 4; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( original[ i ], copy[ i ] );
    copy[ i ]++;
  }
  for( int i = 0; i < 4; ++i ) {
    //After modifying the copy, it shouldn't be equal to the original anymore...
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
    EXPECT_FALSE( original[ i ] == copy[ i ] );
  }
  //Assert we copied the correct number of elements. 4 remaining...
  EXPECT_EQ( copy.push_back( 6, 7, 8, 9 ), 0 );
  EXPECT_EQ( copy.push_back( -1 ), -1 );
  //Changing the copy shouldn't change the original vector and vice-versa. Check the original vector pointers.
  EXPECT_EQ( original.push_back( 5, 6, 7, 8 ), 0 );
  EXPECT_EQ( original.push_back( -1 ), -1 );
  //Compare elements. Should all be different.
  for( int i = 0; i < 8; ++i ) {
    EXPECT_EQ( original[ i ], i + 1 );
    EXPECT_EQ( copy[ i ], i + 2 );
  }
}
