#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_KD_TREE_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_KD_TREE_HPP

//C++ stdlib includes
#include <tuple>
#include <type_traits>

//Project includes
#include "memory/allocator/allocator_base.hpp"
#include "meta/utils.hpp"

namespace tcc {

  namespace data_structure {

    template< typename T >
    struct dimensional_traits;

    template< typename... Types >
    struct dimensional_traits< std::tuple<Types...> > {

      static constexpr int dimensions = sizeof...( Types );

      template< int Index, typename U = std::tuple<Types...> >
      static auto get( U&& object ) {
        return std::get<Index>( std::forward<U>( object ) );
      }

    };

    struct default_kd_tree_function {


    };

    template< typename T, typename Function = default_kd_tree_function, typename Traits = dimensional_traits<T> >
    struct kd_tree {

    private:
      static constexpr int Dimensions = Traits::dimensions;

      static_assert( Dimensions > 0 );

      template< size_t NodeDimension >
      struct node {

        using child_t = node< NodeDimension + 1 == Dimensions ? 0 : NodeDimension + 1 >;

        node( void* storage ): m_storage( storage ) {}

        ~node() {
          using maybe_actual_t = std::decay_t<decltype( Traits::template get<NodeDimension>( std::declval<T>() ) )>;
          if( !m_left && !m_right ) {
            T* leaf = static_cast<T*>( m_storage );
            leaf->~T();
          }
          else {
            maybe_actual_t* middle = static_cast<maybe_actual_t*>( m_storage );
            middle->~maybe_actual_t();
            if( m_left ) {
              //We need to pull in the allocator from the class that owns this...
              //Leave as delete for now.
              delete m_left;
            }
            if( m_right ) {
              //We need to pull in the allocator from the class that owns this...
              //Leave as delete for now.
              delete m_right;
            }
          }
        }

        void* m_storage;

        child_t* m_left;

        child_t* m_right;

      };

      template< size_t NodeDimension, typename Type >
      node<NodeDimension>* create_node( Type&& object ) {
        using actual_t = std::decay_t<Type>;
        struct alloc_type {
          std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
          std::aligned_storage_t<sizeof( actual_t ), alignof( actual_t )> m_storage;
        };
        alloc_type* raw_memory = static_cast<alloc_type*>( m_allocator->allocate( sizeof( alloc_type ), alignof( alloc_type ) ) );
        new ( &raw_memory->m_node ) node<NodeDimension>{ static_cast<void*>( &raw_memory->m_storage ) };
        new ( &raw_memory->m_storage ) actual_t{ std::forward<Type>( object ) };
        return reinterpret_cast<node<NodeDimension>*>( &( raw_memory->m_node ) );
      };

      template< size_t I, typename U >
      static auto __get__( U&& el ) {
        return Traits::template get<I>( std::forward<U>( el ) );
      }

      memory::allocator_base* m_allocator;

    public:

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last, memory::allocator_base* allocator = &memory::malloc_allocator ): m_allocator( allocator ) {
        create_node<0>( *first );
      }

    };

  }

}

#endif
