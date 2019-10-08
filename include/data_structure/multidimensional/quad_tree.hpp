#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP

//C stdlib includes
#include <stdint.h>
#include <assert.h>

//C++ stdlib includes
#include <vector>
#include <type_traits>

//Project includes
#include "shapes.hpp"
#include "internal/free_list.hpp"

namespace tcc {

  namespace data_structure {

    template< typename T >
    struct rect_quad_tree {

      static_assert( std::is_same_v<decltype( make_rectangle( std::declval<T>() ) ), rectangle> );

    private:

      template< typename U >
      using free_list_t = __detail__::__free_list__<U>;

      struct node {

        //This either points to the first of the 4 childs or to the first element we store in the linked list.
        //Should always initialize to -1.
        int32_t m_element_index;

        //If we're not on a leaf node, the size is set to -1. Else, the size is the same as the number of objects the leaf contains.
        int32_t m_size;

      };

      struct node_element {

        //This is a linked list. We implement it as such so we can store a variable number of elements in each leaf.
        //Should always initialize to -1.
        int32_t m_next_index;

        //Index to the actual element.
        int32_t m_node_element;

      };

      //Rect for the whole tree. We compute the child rects on the fly.
      rectangle m_rect;

      //We store the elements in this quad tree as a vector outside the nodes,
      //since an element may spam multiple regions and we don't want memory overhead inside the nodes.
      //This way, we can store only indexes to the actual elements inside the nodes, which saves us memory and gains us speed.
      std::vector<std::pair<T,rectangle>> m_data;

      //And we also need this additional array cause we store the elements as linked lists.
      free_list_t<node_element> m_elements;

      free_list_t<node> m_nodes;

      int32_t m_max_depth;

      int32_t m_max_element_node;

      void
      __insert_into_quadrants__( int32_t first_child, const rectangle& previous_rect, int32_t element_index, const rectangle& insert_rect, int32_t depth ) {
        auto [ x, y, width, height ] = previous_rect;
        int new_width = width >> 1;
        int new_height = height >> 1;
        int new_width_odd = new_width + ( width & 1 ) - 1;
        int new_height_odd = new_height + ( height & 1 ) - 1;
        rectangle top_left = rectangle{ x, y, new_width, new_height };
        rectangle top_right = rectangle{ x + new_width + 1, y, new_width_odd, new_height };
        rectangle bottom_left = rectangle{ x, y + new_height + 1, new_width, new_height_odd };
        rectangle bottom_right = rectangle{ x + new_width + 1, y + new_height + 1, new_width_odd, new_height_odd };
        assert( !intersects_with( top_left, top_right ) && "Rectangles shouldn't overlap!\n" );
        assert( !intersects_with( top_left, bottom_left ) && "Rectangles shouldn't overlap!\n" );
        assert( !intersects_with( top_left, bottom_right ) && "Rectangles shouldn't overlap!\n" );
        assert( !intersects_with( top_right, bottom_left ) && "Rectangles shouldn't overlap!\n" );
        assert( !intersects_with( top_right, bottom_right ) && "Rectangles shouldn't overlap!\n" );
        assert( !intersects_with( bottom_right, bottom_left ) && "Rectangles shouldn't overlap!\n" );
        if( intersects_with( insert_rect, top_left ) ) {
          node* next = &m_nodes[ first_child ];
          __insert__( next, top_left, element_index, insert_rect, depth + 1 );
        }
        if( intersects_with( insert_rect, top_right ) ) {
          node* next = &m_nodes[ first_child + 1 ];
          __insert__( next, top_right, element_index, insert_rect, depth + 1 );
        }
        if( intersects_with( insert_rect, bottom_left ) ) {
          node* next = &m_nodes[ first_child + 2 ];
          __insert__( next, bottom_left, element_index, insert_rect, depth + 1 );
        }
        if( intersects_with( insert_rect, bottom_right ) ) {
          node* next = &m_nodes[ first_child + 3 ];
          __insert__( next, bottom_right, element_index, insert_rect, depth + 1 );
        }
      }

      void
      __split__( node* cur_node, const rectangle& node_rect, int32_t depth ) {
        //Get 4 chunks of memory from the node free list to store the child nodes.
        int32_t first_child = m_nodes.push_back( node{ -1, 0 } );
        int32_t _1 = m_nodes.push_back( node{ -1, 0 } );
        int32_t _2 = m_nodes.push_back( node{ -1, 0 } );
        int32_t _3 = m_nodes.push_back( node{ -1, 0 } );
        assert( _1 == first_child + 1 && "Top right child should be allocated 1 after top left.\n" );
        assert( _2 == first_child + 2 && "Bottom left child should be allocated 2 after top left.\n" );
        assert( _3 == first_child + 3 && "Bottom right child should be allocated 3 after top left.\n" );
        int linked_list_element = cur_node->m_element_index;
        while( linked_list_element != -1 ) {
          assert( m_elements[ linked_list_element ].m_node_element < m_data.size() && "Node element should be a valid index\n" );
          int32_t element_index = m_elements[ linked_list_element ].m_node_element;
          auto& element_rect = m_data[ m_elements[ linked_list_element ].m_node_element ].second;
          __insert_into_quadrants__( first_child, node_rect, element_index, element_rect, depth );
          linked_list_element = m_elements[ linked_list_element ].m_next_index;
        }
        cur_node->m_element_index = first_child;
        cur_node->m_size = -1;
      }

      void
      __insert__( node* ptr, const rectangle& previous_rect, int32_t element_index, const rectangle& insert_rect, int32_t depth ) {
        if( ptr->m_size != -1 ) {
          //Leaf case
          if( depth != m_max_depth && ptr->m_size == m_max_element_node ) {
            //Split and insert if we're not on max depth yet and we're already full.
            __split__( ptr, previous_rect, depth );
            __insert_into_quadrants__( ptr->m_element_index, previous_rect, element_index, insert_rect, depth );
          }
          else {
            node_element new_element = { ptr->m_element_index, element_index };
            ptr->m_element_index = m_elements.push_back( new_element );
            ++ptr->m_size;
          }
        }
        else {
          //Non leaf case. Look for which rectangles we want to branch off.
          __insert_into_quadrants__( ptr->m_element_index, previous_rect, element_index, insert_rect, depth );
        }
      }

    public:

      rect_quad_tree( rectangle rect = rectangle{ 0, 0, std::numeric_limits<int>::max(), std::numeric_limits<int>::max() }, int32_t max_el_n = 4, int32_t max_depth = 8 ):
                        m_rect( rect ),
                        m_max_depth( max_depth ),
                        m_max_element_node( max_el_n )  {
        m_nodes.push_back( node{ -1, 0 } );
      }

      void
      insert( const T& value ) {
        int32_t index = m_data.size();
        auto rect = make_rectangle( value );
        m_data.emplace_back( value, rect );
        __insert__( &m_nodes[ 0 ], m_rect, index, rect, 0 );
      }

      void
      remove( const T& value ) {
        node* head = &m_nodes[ 0 ];
        auto rect = make_rectangle( value );
      }

    };

  } //namespace data_structure

}  //namespace tcc

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP
