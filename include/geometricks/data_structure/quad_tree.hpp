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

  struct quad_tree_traits {
    static constexpr int max_depth = 10;
    static constexpr int leaf_size = 4;
  };

  template< typename T,
            typename Traits = quad_tree_traits >
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
      __insert__( m_nodes[ 0 ], m_root_rect, id, 0 );
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

      void
      set_leaf_element( int32_t leaf_index ) {
        m_first_child.m_value = 0x80000000 | ( leaf_index + 1 );
      }

      index_t m_first_child;

    };

    //Since each element might be on more than one quadrant of the quad tree, we store 1 index to the element for each type it appears.
    //TODO: think if we have to store the rectangle for each element. (Probably a yes).
    struct element_t {

      int32_t id;

      int32_t next;

    };

    //TODO: SFINAE this to set default value in case leaf_size is not found.
    static constexpr int LEAF_SIZE = Traits::leaf_size;

    //TODO: SFINAE this to set default value in case max_depth is not found.
    static constexpr int MAX_DEPTH = Traits::max_depth;

    static constexpr typename node::index_t EMPTY_LEAF = { 0x80000000 };

    geometricks::rectangle m_root_rect;

    geometricks::small_vector<node, 1> m_nodes; //Index 0 represents the root.

    geometricks::small_vector<element_t, 0> m_element_ref;

    geometricks::small_vector<std::pair<T, geometricks::rectangle>, 0> m_elements;

    static node
    __make_leaf__() {
      return node{ EMPTY_LEAF };
    }

    struct __split_ret__ {
      geometricks::rectangle first;
      geometricks::rectangle second;
      geometricks::rectangle third;
      geometricks::rectangle fourth;
    };

    __split_ret__
    __split_rect__( const geometricks::rectangle& rect ) {
      auto half_width = rect.width / 2;
      auto half_height = rect.height / 2;
      return { geometricks::rectangle{ rect.x_left, rect.y_top, half_width, half_height },
               geometricks::rectangle{},
               geometricks::rectangle{},
               geometricks::rectangle{}};
    }

    int
    __count_leaf_objects__( int32_t index ) {
      int ret = 0;
      while( index != -1 ) {
        index = m_element_ref[ index ].next;
        ++ret;
      }
      return ret;
    }

    //TODO
    void
    __split_node__( node& cur_node, const geometricks::rectangle& rect ) {
      //Create 4 new nodes. Edit small vector class to allow insertion of more than 1 object at a time.
      ( void )cur_node;
      ( void )rect;
    }

    void
    __insert__( node& cur_node, const geometricks::rectangle& rect, int32_t object_id, int depth ) {
      ( void )object_id;
      if( !cur_node.is_leaf() ) {
        auto [ first, second, third, fourth ] = __split_rect__( rect );
        std::cout << first << "\n";
        std::cout << second << "\n";
        std::cout << third << "\n";
        std::cout << fourth << "\n";
        //TODO
      }
      else {
        int32_t last_element_index = cur_node.m_first_child;
        if( depth == MAX_DEPTH ) {
          //TODO
        }
        else {
          auto count = __count_leaf_objects__( last_element_index );
          if( count == LEAF_SIZE ) {
            __split_node__( cur_node, rect );
            //TODO
          }
          else {
            element_t new_el{ object_id, last_element_index };
            auto element_id = m_element_ref.push_back( new_el );
            cur_node.set_leaf_element( element_id );
          }
        }
      }
    }

  };

}  //namespace geometricks

#endif //GEOMETRICKS_DATA_STRUCTURE_QUAD_TREE_HPP
