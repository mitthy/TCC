#ifndef GEOMETRICKS_DATA_STRUCTURE_QUAD_TREE_HPP
#define GEOMETRICKS_DATA_STRUCTURE_QUAD_TREE_HPP

//C stdlib includes
#include <stdint.h>
#include <assert.h>

//C++ stdlib includesca
#include <type_traits>

//Project includes
#include "shapes.hpp"
#include "geometricks/memory/allocator.hpp"
#include "internal/small_vector.hpp"

namespace geometricks {

  template< typename T,
            int MaxDepth = 10 >
  struct quad_tree {

    quad_tree( geometricks::rectangle rect, geometricks::allocator alloc = geometricks::allocator{} ):  m_root_rect( rect ),
                                                                                                        m_nodes( alloc ),
                                                                                                        m_element_ref( alloc ),
                                                                                                        m_elements( alloc ) {
      m_nodes.push_back( __make_leaf__() );
    }

    bool insert( const T& element ) {
      geometricks::rectangle bounding_rect = geometricks::make_rectangle( element );
      if( !m_root_rect.contains( bounding_rect ) ) {
        return false;
      }
      auto id = m_elements.push_back( std::make_pair( element, bounding_rect ) );
      __insert__( m_nodes[ 0 ], m_root_rect, id );
      return true;
    }

  private:

    struct node {

      //Indexes values in the quad tree. Last bit is used to represent if the value is a leaf or not.
      struct index_t {

        operator int32_t() const {
          return ( m_value & 0x7FFFFFFF ) - 1;
        }

        uint32_t m_value;

      };

      bool
      is_leaf() const {
        return m_first_child.m_value >> 31;
      }

      index_t m_first_child;

    };

    //Since each element might be on more than one quadrant of the quad tree, we store 1 index to the element for each type it appears.
    //TODO: think if we have to store the rectangle for each element. (Probably a yes).
    struct element_t {

      int32_t id;

      int32_t next;

    };

    static constexpr typename node::index_t EMPTY_LEAF = { 0x80000000 };

    geometricks::rectangle m_root_rect;

    geometricks::small_vector<node, 1> m_nodes; //Index 0 represents the root.

    geometricks::small_vector<element_t, 0> m_element_ref;

    geometricks::small_vector<std::pair<T, geometricks::rectangle>, 0> m_elements;

    static node
    __make_leaf__() {
      return node{ EMPTY_LEAF };
    }

    void __insert__( node& cur_node, const geometricks::rectangle& rect, int32_t id ) {
      ( void )cur_node;
      ( void )rect;
      ( void )id;
    }

  };

}  //namespace geometricks

#endif //GEOMETRICKS_DATA_STRUCTURE_QUAD_TREE_HPP
