#include "gtest/gtest.h"
#include "data_structure/multidimensional/quad_tree.hpp"
#include <tuple>

struct element_t {
  int x, y, hw, hh;
};

namespace tcc {

  namespace data_structure {

    namespace rectangle_customization {

      template<>
      struct make_rectangle<element_t> {

        static constexpr tcc::data_structure::rectangle
        _( const element_t& value ) {
          return { value.x, value.y, value.hw, value.hh };
        }

      };

    }

  }

}

using quad_tree_t = tcc::data_structure::rect_quad_tree<std::tuple<int,int,int,int>>;

TEST( TestQuadTree, TestCreation ) {
  quad_tree_t tree{};
  for( int i = 0; i < 256; ++i ) {
    tree.insert( std::make_tuple( ( i + 5 ) * 2, ( i + 5 ) * 2, 10, 10 ) );
  }
}
