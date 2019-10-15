#include "gtest/gtest.h"
#include "data_structure/multidimensional/shapes.hpp"

using namespace geometricks::data_structure;

TEST( TestRectangle, TestIntersection ) {
  EXPECT_TRUE( intersects_with( rectangle{ 5, 2, 4, 10 }, rectangle{ 5, 2, 4, 10 } ) );
  EXPECT_TRUE( intersects_with( rectangle{ 5, 2, 4, 10 }, rectangle{ 9, 8, 4, 6 } ) );
  EXPECT_FALSE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 10, 8, 1, 6 } ) );
  EXPECT_FALSE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 10, 12, 2, 8 } ) );
  EXPECT_TRUE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 2, 8, 3, 6 } ) );
  EXPECT_FALSE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 2, 8, 2, 6 } ) );
  EXPECT_FALSE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 2, 13, 2, 8 } ) );
  EXPECT_TRUE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 8, 11, 4, 10 } ) );
  EXPECT_FALSE( intersects_with( rectangle{ 5, 2, 4, 10 },  rectangle{ 8, 13, 4, 8 } ) );
  EXPECT_FALSE( intersects_with( rectangle{ 5, 2, 4, 10 }, rectangle{ -1, -1, 2, 2 } ) );
}
