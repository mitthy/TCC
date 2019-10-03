#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP

//C stdlib includes
#include <stdint.h>

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
      std::vector<T> m_data;

      //And we also need this additional array cause we store the elements as linked lists.
      free_list_t<node_element> m_elements;

      free_list_t<node> m_nodes;

      int32_t m_max_depth;

      int32_t m_max_element_node;

      void __insert_into_quadrants__( node* ptr, const rectangle& previous_rect, int32_t element_index, const rectangle& insert_rect, int32_t depth ) {
        auto [ x_center, y_center, half_width, half_height ] = previous_rect;
        int new_half_width = half_width >> 1;
        int new_half_height = half_height >> 1;
        rectangle top_left = rectangle{ x_center - new_half_width, y_center - new_half_height, new_half_width, new_half_height };
        rectangle top_right = rectangle{ x_center + new_half_width, y_center - new_half_height, new_half_width, new_half_height };
        rectangle bottom_left = rectangle{ x_center - new_half_width, y_center + new_half_height, new_half_width, new_half_height };
        rectangle bottom_right = rectangle{ x_center + new_half_width, y_center + new_half_height, new_half_width, new_half_height };
        if( intersects_with( insert_rect, top_left ) ) {
          node* next = &m_nodes[ ptr->m_element_index + 0 ];
          __insert__( next, top_left, element_index, insert_rect, depth + 1 );
        }
        if( intersects_with( insert_rect, top_right ) ) {
          node* next = &m_nodes[ ptr->m_element_index + 1 ];
          __insert__( next, top_right, element_index, insert_rect, depth + 1 );
        }
        if( intersects_with( insert_rect, bottom_left ) ) {
          node* next = &m_nodes[ ptr->m_element_index + 2 ];
          __insert__( next, bottom_left, element_index, insert_rect, depth + 1 );
        }
        if( intersects_with( insert_rect, bottom_right ) ) {
          node* next = &m_nodes[ ptr->m_element_index + 3 ];
          __insert__( next, bottom_right, element_index, insert_rect, depth + 1 );
        }
      }

      void __split__( node* cur_node, const rectangle& node_rect ) {
        //TODO
      }

      void
      __insert__( node* ptr, const rectangle& previous_rect, int32_t element_index, const rectangle& insert_rect, int32_t depth ) {
        if( ptr->m_size != -1 ) {
          //Leaf case
          if( depth != m_max_depth && ptr->m_size == m_max_element_node ) {
            //Split and insert if we're not on max depth yet and we're already full.
            __split__( ptr, previous_rect );
            __insert_into_quadrants__( ptr, previous_rect, element_index, insert_rect, depth );
          }
          else {
            node_element new_element = { ptr->m_element_index, element_index };
            ptr->m_element_index = m_elements.push_back( new_element );
            ++ptr->m_size;
          }
        }
        else {
          //Non leaf case. Look for which rectangles we want to branch off.
          __insert_into_quadrants__( ptr, previous_rect, element_index, insert_rect, depth );
        }
      }

    public:

      rect_quad_tree( int32_t max_el_n = 4, int32_t max_depth = 8, rectangle rect = rectangle{ 0, 0, std::numeric_limits<int>::max(), std::numeric_limits<int>::max() } ):
                        m_rect( rect ),
                        m_max_depth( max_depth ),
                        m_max_element_node( max_el_n )  {
        m_nodes.push_back( node{ -1, 0 } );
      }

      void insert( const T& value ) {
        int32_t index = m_data.size();
        m_data.push_back( value );
        __insert__( &m_nodes[ 0 ], m_rect, index, make_rectangle( value ), 0 );
      }

    };

  } //namespace data_structure

}  //namespace tcc

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP
