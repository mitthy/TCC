#ifndef TCC_DATA_STRUCTURE_BITMAPPED_VECTOR_TRIE_HPP
#define TCC_DATA_STRUCTURE_BITMAPPED_VECTOR_TRIE_HPP

//C stdlib includes
#include <stdint.h>

//C++ stdlib includes
#include <type_traits>
#include <bitset>

//Project includes
#include "utils/log.h"
#include "primitives/static_vector.hpp"
#include "memory/allocator/allocator_base.hpp"
#include "functional/optional.hpp"

namespace tcc {

  namespace data_structure {

    template< typename T, size_t Size >
    struct OptionalPolicy {
    };

    template< typename T, size_t NodeSize = 32, typename Policy = OptionalPolicy<T, NodeSize> >
    struct bitmapped_vector_trie {

      //Size must be a power of 2 so that we can efficiently calculate the index.
      static_assert( ( ( NodeSize & ( NodeSize - 1 ) ) == 0 ) && ( NodeSize > 0 ), "Size must be a power of 2");

      //TODO: make this class able to use different allocators.

    private:

      static constexpr uint8_t MaskStep = utils::log2( NodeSize );

      static constexpr size_t Mask = NodeSize - 1;

      static_assert( MaskStep > 0 );

      union node  {
        unsafe_static_vector< T,NodeSize >* m_leaf;
        unsafe_static_vector< node, NodeSize >* m_tree;
      };

      tcc::memory::allocator_base* m_allocator;

      node m_root;

      //We use the rank as how many steps we want the incoming value to take before masking it out.
      //Each new level, we increase the rank by MaskStep
      uint64_t m_base_mask;

      uint8_t m_rank;

      size_t m_size;

      static node
      create_leaf_node( tcc::memory::allocator_base& alloc ) {
        node ret{};
        using actual = unsafe_static_vector< T,NodeSize >;
        void* memory = alloc.allocate( sizeof( actual ), alignof( actual ) );
        actual* ptr = static_cast<actual*>( memory );
        new ( ptr ) actual {};
        ret.m_leaf = ptr;
        return ret;
      }

      unsafe_static_vector< T,NodeSize >*
      get_node_for_index( size_t index ) const {
        node cur = m_root;
        auto mask = m_base_mask;
        auto rank = m_rank;
        while( rank ) {
          unsafe_static_vector< node, NodeSize >& next = *cur.m_tree;
          auto next_index = ( index & mask ) >> rank;
          cur = next[ next_index ];
          mask >>= MaskStep;
          rank -= MaskStep;
        }
        return cur.m_leaf;
      }

      unsafe_static_vector< T,NodeSize >*
      try_get_node( node* prev, node* cur, size_t new_size, uint64_t mask, uint8_t rank ) {
        if( rank ) {
          unsafe_static_vector< node, NodeSize >& next = *cur->m_tree;
          auto next_index = ( new_size & mask ) >> rank;
          mask >>= MaskStep;
          rank -= MaskStep;
          auto* next_node = try_get_node( cur, &next[ next_index ], new_size, mask, rank );
          if( !next_node ) { //If children is full...
            if( !( new_size & mask ) ) {
              //If we are full...
              if( !prev ) {
                //Create previous node and build down to a leaf...
                //TODO
              }
              else {
                return nullptr;
              }
            }
            else {
              //We gotta build down to a leaf here ourselves...

            }
          }
        }
        else {
          if( !( new_size & mask ) && new_size ) {
            if( !prev ) {
              //If we're full and there's no one before us...
              node new_root{};
              void* raw_memory = m_allocator->allocate( sizeof( unsafe_static_vector<node, NodeSize> ), alignof( unsafe_static_vector<node, NodeSize> ) );
              new ( raw_memory ) unsafe_static_vector< node, NodeSize > {};
              new_root.m_tree = static_cast< unsafe_static_vector< node, NodeSize >* >( raw_memory );
              new_root.m_tree->push_back( *cur );
              node next_vector = create_leaf_node( *m_allocator );
              new_root.m_tree->push_back( next_vector );
              m_root = new_root;
              m_rank += MaskStep;
              m_base_mask <<= MaskStep;
              return next_vector.m_leaf;
            }
            else {
              return nullptr;
            }
          }
          return cur->m_leaf;
        }
      }

    public:

      bitmapped_vector_trie( tcc::memory::allocator_base* alloc = &tcc::memory::malloc_allocator ):
                            m_allocator( alloc ), m_root( create_leaf_node( *alloc ) ), m_base_mask( Mask ), m_rank( 0 ), m_size( 0 ) {}

      void
      push_back( const T& element ) {
        auto* node = try_get_node( nullptr, &m_root, m_size, m_base_mask, m_rank );
        //TODO: make case 2 ( element expansion )
        node->push_back( element );
        ++m_size;
      }

      const T&
      operator[] ( size_t index ) const {
        auto& node = *get_node_for_index( index );
        return node[ index & Mask ];
      }

      T&
      operator[] ( size_t index ) {
        auto& node = *get_node_for_index( index );
        return node[ index & Mask ];
      }

    };

  }

}

#endif
