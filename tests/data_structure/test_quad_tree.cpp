#include "gtest/gtest.h"
#include "geometricks/data_structure/quad_tree.hpp"
#include <tuple>
#include <type_traits>

struct element_t {
  int x, y, hw, hh;
};

namespace geometricks {

    namespace rectangle_customization {

      template<>
      struct make_rectangle<element_t> {

        static constexpr geometricks::rectangle
        _( const element_t& value ) {
          return { value.x, value.y, value.hw, value.hh };
        }

      };

    }

}

TEST( TestQuadTree, TestCreation ) {
  geometricks::quad_tree<std::tuple<int,int,int,int>> tree{ geometricks::rectangle{ -1000, -1000, 2000, 2000 } };
  for( int i = 0; i < 256; ++i ) {
    tree.insert( std::make_tuple( ( i + 5 ) * 2, ( i + 5 ) * 2, 10, 10 ) );
  }
}
