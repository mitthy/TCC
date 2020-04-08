#ifndef GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_RTREE_HPP
#define GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_RTREE_HPP

#include "shapes.hpp"
#include <stdint.h>
#include <vector>

namespace geometricks {

  namespace __detail__ {

    struct __2d_rtree__ {

      struct node {

        //Contains either the index of the first children of this node or the object index in case of a leaf node.
        //First 31 bits are used to store the actual index, while the most significant bit is a flag that is set to 1 whenever this is a childnode
        int32_t m_first_children;

        geometricks::data_structure::rectangle m_bounding_rect;

        int16_t m_size;

      };

      std::vector<node> m_nodes;  //Change this to raw array so we can customize with library defined allocator.

      __2d_rtree__(): m_nodes{ __create_root__() } {}

    private:

      static node __create_root__() {
        return node{};
      }

    };

  }

}

#endif
