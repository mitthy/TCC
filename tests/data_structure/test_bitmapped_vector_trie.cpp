#include "gtest/gtest.h"
#include "data_structure/bitmapped_vector_trie.hpp"

TEST( TestBitmappedVectorTrie, TestInsertSingleElement ) {
  tcc::data_structure::bitmapped_vector_trie<int, 32> single_element{};
  single_element.push_back( 42 );
  EXPECT_EQ( single_element[ 0 ], 42 );
}

TEST( TestBitmappedVectorTrie, TestExpansion ) {
  tcc::data_structure::bitmapped_vector_trie<int, 4> expand_me{};
  for( int i = 0; i < ( 4 * 4 ) * ( 4 * 4 ) + 1; ++i ) {
    expand_me.push_back( 42 );
  }
  for( int i = 0; i < ( 4 * 4 ) * ( 4 * 4 ) + 1; ++i ) {
    EXPECT_EQ( expand_me[ i ], 42 );
  }
}
