#include "gtest/gtest.h"
#include "data_structure/multidimensional/shapes.hpp"

using namespace tcc::data_structure;

TEST( TestRectangle, TestIntersection ) {
  rectangle rect{ 7, 7, 2, 5 };
  EXPECT_TRUE( intersects_with( rect, rect ) );
  EXPECT_TRUE( intersects_with( rect, rectangle{ 10, 11, 2, 3 } ) );
  EXPECT_FALSE( intersects_with( rect,  rectangle{ 10, 11, 1, 3 } ) );
  EXPECT_FALSE( intersects_with( rect,  rectangle{ 10, 16, 1, 4 } ) );
  EXPECT_TRUE( intersects_with( rect,  rectangle{ 4, 11, 2, 3 } ) );
  EXPECT_FALSE( intersects_with( rect,  rectangle{ 4, 11, 1, 3 } ) );
  EXPECT_FALSE( intersects_with( rect,  rectangle{ 4, 16, 1, 4 } ) );
  EXPECT_TRUE( intersects_with( rect,  rectangle{ 10, 16, 2, 5 } ) );
  EXPECT_FALSE( intersects_with( rect,  rectangle{ 10, 16, 2, 4 } ) );
  EXPECT_FALSE( intersects_with( rect, rectangle{ 0, 0, 1, 1 } ) );
}
