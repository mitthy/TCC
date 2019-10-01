#include "gtest/gtest.h"
#include "data_structure/multidimensional/quad_tree.hpp"

TEST( TestQuadTree, TestCreation ) {
  std::tuple<int, int, int, int> test( 3, 4, 4, 5 );
  tcc::data_structure::make_rectangle( test );
  ASSERT_TRUE( false );
}
