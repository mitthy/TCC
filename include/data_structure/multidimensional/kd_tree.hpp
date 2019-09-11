#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_KD_TREE_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_KD_TREE_HPP

//C stdlib includes
#include <stdint.h>
#include <assert.h>

//C++ stdlib includes
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <iterator>

//Project includes
#include "memory/allocator/allocator_base.hpp"
#include "meta/utils.hpp"
#include "algorithm/algorithm.hpp"

namespace tcc {

  namespace data_structure {

    template< int Index >
    struct dimension_t {};

    namespace __detail__ {

      template< typename Underlying, size_t Dimension, typename Traits >
      struct dimensional_iterator {

      private:
        Underlying m_iterator;
      public:

        using iterator_category = typename std::iterator_traits<Underlying>::iterator_category;
        using value_type = typename Traits::template type_at<Dimension>;
        using difference_type = typename std::iterator_traits<Underlying>::difference_type;
        using pointer = typename Traits::template type_at<Dimension>*;
        using reference = typename Traits::template type_at<Dimension>&;

        dimensional_iterator(): m_iterator() {}
        explicit dimensional_iterator( const Underlying & it ): m_iterator( it ) { }
        dimensional_iterator( const dimensional_iterator& other ): m_iterator( other.m_iterator ) { }

        auto
        operator*() {
          return Traits::get( *m_iterator, dimension_t<Dimension>{} );
        }

        dimensional_iterator&
        operator++() {
          ++m_iterator;
          return *this;
        }

        dimensional_iterator operator++(int) {
          dimensional_iterator it(*this);
          m_iterator++;
          return it;
        }

        dimensional_iterator&
        operator--() {
          --m_iterator;
          return *this;
        }

        dimensional_iterator
        operator--(int) {
          dimensional_iterator it(*this);
          m_iterator--;
          return it;
        }

        bool
        operator==( const dimensional_iterator& other ) const {
          return other.m_iterator == m_iterator;
        }

        bool
        operator!=( const dimensional_iterator& other ) const {
          return !( *this == other );
        }

        auto
        operator-( const dimensional_iterator& other ) {
          return m_iterator - other.m_iterator;
        }

        friend void
        iter_swap( dimensional_iterator l, dimensional_iterator r ) {
          using std::iter_swap;
          iter_swap( l.m_iterator, r.m_iterator );
        }

        dimensional_iterator&
        operator+=( size_t sz ) {
          m_iterator += sz;
          return *this;
        }

        Underlying
        base() {
          return m_iterator;
        }

        dimensional_iterator
        operator+( size_t sz ) const {
          dimensional_iterator tmp( *this );
          tmp += sz;
          return tmp;
        }

      };

    }

    template< typename T >
    struct dimensional_traits;

    template< typename... Types >
    struct dimensional_traits< std::tuple<Types...> > {

      static constexpr int dimensions = sizeof...( Types );

      template< int Index, typename U = std::tuple<Types...> >
      static auto get( U&& object, dimension_t<Index> ) {
        return std::get<Index>( std::forward<U>( object ) );
      }

      template< typename... Types >
      auto distance( const std::tuple<Types...>& lhs, const std::tuple<Types...>& rhs ) {
        //TODO
      }

      template< int I >
      using type_at = std::decay_t< decltype( get( std::declval<std::tuple<Types...>>(), std::declval<dimension_t<I>>() ) ) >;

    };

    struct mean_split_function {

      template< typename Iterator, typename Sentinel >
      auto operator()( Iterator first, Sentinel last )  {
        algorithm::mean< typename std::iterator_traits<Iterator>::value_type > mean_calculator{};
        return mean_calculator( first, last );
      }

    };

    struct less_compare_function {

      template< typename T, typename U >
      bool operator()( const T& element, const U& split_value ) {
        return element < split_value;
      }

    };

    template< typename Traits >
    struct default_nearest_neighbour_function {
      template< typename T, typename U, size_t Index >
      auto operator()( const T& element, const U& stored, const dimension_t<Index> ) {
        return Traits::get( element, dimension_t<Index>{} ) - stored;
      }

      template< typename T >
      auto operator()( const T& lhs, const T& rhs ) {
        return Traits::distance( lhs, rhs );
      }
    };

    template< typename T, typename SplitFunction = mean_split_function, typename CompareFunction = less_compare_function, typename Traits = dimensional_traits<T> >
    struct kd_tree {

    private:
      static constexpr int Dimensions = Traits::dimensions;

      static_assert( Dimensions > 0 );

      template< size_t NodeDimension >
      struct node {

        friend class kd_tree;

        using child_t = node< NodeDimension + 1 == Dimensions ? 0 : NodeDimension + 1 >;

        node( void* storage ): m_storage( storage ), m_left( nullptr ), m_right( nullptr ) {}

        node( void* storage, child_t* left, child_t* right ): m_storage( storage ), m_left( left ), m_right( right ) {}

        void* m_storage;

        child_t* m_left;

        child_t* m_right;

      };

      template< size_t NodeDimension, typename Type, typename... Args >
      node<NodeDimension>*
      create_node( Type&& object, Args&&... args ) {
        using actual_t = std::decay_t<Type>;
        struct alloc_type {
          std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
          std::aligned_storage_t<sizeof( actual_t ), alignof( actual_t )> m_storage;
        };
        alloc_type* raw_memory = static_cast<alloc_type*>( m_allocator->allocate( sizeof( alloc_type ), alignof( alloc_type ) ) );
        new ( &raw_memory->m_node ) node<NodeDimension>{ static_cast<void*>( &raw_memory->m_storage ), std::forward<Args>( args )... };
        new ( &raw_memory->m_storage ) actual_t{ std::forward<Type>( object ) };
        return reinterpret_cast<node<NodeDimension>*>( &( raw_memory->m_node ) );
      };

      template< size_t I, typename U >
      static auto
      __get__( U&& el ) {
        return Traits::get( std::forward<U>( el ), dimension_t<I>{} );
      }

      memory::allocator_base* m_allocator;

      CompareFunction m_compare;

      SplitFunction m_split;

      node<0>* m_head;

      template< size_t NodeDimension >
      void
      __remove_node__( node<NodeDimension>* to_delete ) {
        using maybe_actual_t = std::decay_t<decltype( Traits::get( std::declval<T>(), std::declval<dimension_t<NodeDimension>>() ) )>;
        if( !to_delete->m_left && !to_delete->m_right ) {
          T* leaf = static_cast<T*>( to_delete->m_storage );
          leaf->~T();
        }
        else {
          maybe_actual_t* middle = static_cast<maybe_actual_t*>( to_delete->m_storage );
          middle->~maybe_actual_t();
          if( to_delete->m_left ) {
            __remove_node__( to_delete->m_left );
          }
          if( to_delete->m_right ) {
            __remove_node__( to_delete->m_right );
          }
        }
        m_allocator->deallocate( static_cast<void*>( to_delete ) );
      }

      template< size_t Dimension, typename InputIterator, typename Sentinel >
      node<Dimension>*
      create_kd_tree( InputIterator first, Sentinel last ) {
        if( algorithm::distance( first, last ) == 1 ) {
          return create_node<Dimension>( *first );
        }
        else {
          auto begin = __detail__::dimensional_iterator<InputIterator, Dimension, Traits>( first );
          auto end = __detail__::dimensional_iterator<Sentinel, Dimension, Traits>( last );
          auto split = m_split( begin, end );
          auto temp_function = m_compare;
          auto partition_function = [&temp_function, split]( const auto& element ) {
            return temp_function( element, split );
          };
          auto middle = tcc::algorithm::partition( begin, end, partition_function ).base();
          auto* left = create_kd_tree<Dimension + 1 == Dimensions ? 0 : Dimension + 1>( first, middle );
          auto* right = create_kd_tree<Dimension + 1 == Dimensions ? 0 : Dimension + 1>( middle, last );
          return create_node<Dimension>( split, left, right );
        }
      }

    public:

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last, memory::allocator_base* allocator = &memory::malloc_allocator ):
            m_allocator( allocator ), m_compare{}, m_split{}, m_head( create_kd_tree<0>( first, last ) ) {}

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last, CompareFunction compare, SplitFunction split = SplitFunction{}, memory::allocator_base* allocator = &memory::malloc_allocator ):
            m_allocator( allocator ), m_compare( compare ), m_split( split ), m_head( create_kd_tree<0>( first, last ) ) {}

      ~kd_tree() {
        __remove_node__( m_head );
      }

      template< typename DistanceFunction = default_nearest_neighbour_function<Traits> >
      T nearest_neighbour( const T& point, DistanceFunction f = DistanceFunction{} ) const {
        //TODO
        return T{};
      }

    };

  }

}

#endif
