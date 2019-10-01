#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP

//C stdlib includes
#include <stdint.h>

//C++ stdlib includes
#include <vector>
#include <type_traits>

//Project includes
#include "dimensional_traits.hpp"

namespace tcc {

  namespace data_structure {

    template< typename T >
    struct rect_quad_tree {

      static_assert( std::is_same_v<decltype( make_rectangle( std::declval<T>() ) ), rectangle> );

    private:

      struct node {

        //This either points to the first of the 4 childs or to the first element we store in the linked list.
        union element_t {
          void* m_first_child;
          int32_t m_element_index;
        };

        //If we're not on a leaf node, the size is set to -1. Else, the size is the same as the number of objects the leaf contains.
        int32_t m_size = -1;

      };

      struct node_element {

        //This is a linked list. We implement it as such so we can store a variable number of elements in each leaf.
        int32_t m_next_index = -1;

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
      std::vector<node_element> m_elements;

      int32_t m_max_depth;

      int32_t m_max_element_node;

      static void __insert__( node* ptr, int32_t max_depth, int32_t max_element_node, rectangle previous_rect, int32_t element_index, rectangle insert_rect, int32_t depth ) {
        if( ptr->m_size != -1 ) {
          //Leaf case
          if( depth != max_depth && ptr->m_size == max_element_node ) {
            //Split and insert if we're not on max depth yet and we're already full.
          }
          else {

          }
        }
        else {
          //Non leaf case. Look for which rectangles we want to branch off.

        }
      }

    public:

    };

  } //namespace data_structure

}  //namespace tcc

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_QUAD_TREE_HPP
