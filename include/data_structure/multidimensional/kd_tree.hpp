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
#include <limits>

//Project includes
#include "memory/allocator/malloc_allocator.hpp"
#include "meta/utils.hpp"
#include "algorithm/absolute_difference.hpp"
#include "algorithm/mean.hpp"
#include "algorithm/partition.hpp"

namespace tcc {

  namespace data_structure {

    template< int Index >
    struct dimension_t {};

    namespace __detail__ {

      template< typename Underlying, int Dimension, typename Traits >
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
        operator--(int) const {
          dimensional_iterator it(*this);
          m_iterator--;
          return it;
        }

        bool
        operator==( const dimensional_iterator& other ) const noexcept {
          return other.m_iterator == m_iterator;
        }

        bool
        operator!=( const dimensional_iterator& other ) const noexcept {
          return !( *this == other );
        }

        auto
        operator-( const dimensional_iterator& other ) const noexcept {
          return m_iterator - other.m_iterator;
        }

        friend void
        iter_swap( dimensional_iterator l, dimensional_iterator r ) noexcept {
          using std::iter_swap;
          iter_swap( l.m_iterator, r.m_iterator );
        }

        dimensional_iterator&
        operator+=( size_t sz ) noexcept {
          m_iterator += sz;
          return *this;
        }

        Underlying
        base() noexcept {
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
    struct numeric_limits : std::numeric_limits< T > {};

    template< typename T >
    struct dimensional_traits;

    template< typename... Types >
    struct dimensional_traits< std::tuple<Types...> > {

      static constexpr int dimensions = sizeof...( Types );

      template< int Index, typename U = std::tuple<Types...> >
      static auto
      get( U&& object, dimension_t<Index> ) noexcept {
        return std::get<Index>( std::forward<U>( object ) );
      }

      template< int I >
      using type_at = std::decay_t< decltype( get( std::declval<std::tuple<Types...>>(), std::declval<dimension_t<I>>() ) ) >;

    };

    struct mean_split_function {

      template< typename Iterator, typename Sentinel >
      auto
      operator()( Iterator first, Sentinel last ) const {
        algorithm::mean< typename std::iterator_traits<Iterator>::value_type > mean_calculator{};
        return mean_calculator( first, last );
      }

    };

    struct less_compare_function {

      template< typename T, typename U >
      bool operator()( const T& element, const U& split_value ) const noexcept {
        return element < split_value;
      }

    };

    template< typename Traits >
    struct default_nearest_neighbour_function {

    private:

      template< typename T >
      static auto
      element_distance( const T& lhs, const T& rhs ) noexcept {
        auto tmp = algorithm::absolute_difference( lhs, rhs );
        return tmp * tmp;
      }

    public:

      template< typename T, typename U, int Index >
      auto
      operator()( const T& element, const U& stored, const dimension_t<Index> ) const noexcept {
        return element_distance( Traits::get( element, dimension_t<Index>{} ), stored );
      }

      template< size_t... Index >
      auto
      distance_impl( const auto& lhs, const auto& rhs, std::index_sequence<Index...> ) const noexcept {
        return (element_distance( Traits::get( lhs, dimension_t<Index>{} ), Traits::get( rhs, dimension_t<Index>{} ) ) + ... );
      }

      template< typename T >
      auto
      operator()( const T& lhs, const T& rhs ) const noexcept {
        return distance_impl( lhs, rhs, std::make_index_sequence<Traits::dimensions>{} );
      }
    };

    template< typename T,
              typename SplitFunction = mean_split_function,
              typename CompareFunction = less_compare_function,
              typename Allocator = tcc::memory::malloc_allocator_t >
    struct kd_tree {

      static_assert( std::is_same_v<T, std::decay_t<T>>, "Can only create kd_tree of a value type." );

      using Traits = dimensional_traits<T>;

    private:
      static constexpr int Dimensions = Traits::dimensions;

      static_assert( Dimensions > 0 );

      template< int NodeDimension >
      struct node {

        friend class kd_tree;

        using child_t = node< NodeDimension + 1 == Dimensions ? 0 : NodeDimension + 1 >;

        node( ): m_left( nullptr ), m_right( nullptr ) {}

        node( child_t* left, child_t* right ): m_left( left ), m_right( right ) {}

        child_t* m_left;

        child_t* m_right;

      };

      template< int NodeDimension, typename Type, typename... Args >
      node<NodeDimension>*
      create_node( Type&& object, Args&&... args ) {
        using actual_t = std::decay_t<Type>;
        struct alloc_type {
          std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
          std::aligned_storage_t<sizeof( actual_t ), alignof( actual_t )> m_storage;
        };
        alloc_type* raw_memory = static_cast<alloc_type*>( allocate( *m_allocator, sizeof( alloc_type ), alignof( alloc_type ) ) );
        new ( &raw_memory->m_node ) node<NodeDimension>{ std::forward<Args>( args )... };
        new ( &raw_memory->m_storage ) actual_t{ std::forward<Type>( object ) };
        return reinterpret_cast<node<NodeDimension>*>( &( raw_memory->m_node ) );
      }

      template< int I, typename U >
      static auto
      __get__( U&& el ) {
        return Traits::get( std::forward<U>( el ), dimension_t<I>{} );
      }

      Allocator* m_allocator;

      CompareFunction m_compare;

      SplitFunction m_split;

      node<0>* m_head;

      template< int NodeDimension >
      void
      __remove_node__( node<NodeDimension>* to_delete ) noexcept {
        using maybe_actual_t = std::decay_t<decltype( Traits::get( std::declval<T>(), std::declval<dimension_t<NodeDimension>>() ) )>;
        if( !to_delete->m_left && !to_delete->m_right ) {
          T* leaf = __get_element__<T>( to_delete );
          leaf->~T();
        }
        else {
          maybe_actual_t* middle = __get_element__<maybe_actual_t>( to_delete );
          middle->~maybe_actual_t();
          if( to_delete->m_left ) {
            __remove_node__( to_delete->m_left );
          }
          if( to_delete->m_right ) {
            __remove_node__( to_delete->m_right );
          }
        }
        deallocate( *m_allocator, static_cast<void*>( to_delete ) );
      }

      template< int Dimension, typename InputIterator, typename Sentinel >
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
          if( first == middle ) {
            middle = std::next( middle );
          }
          auto* left = create_kd_tree<Dimension + 1 == Dimensions ? 0 : Dimension + 1>( first, middle );
          auto* right = create_kd_tree<Dimension + 1 == Dimensions ? 0 : Dimension + 1>( middle, last );
          return create_node<Dimension>( split, left, right );
        }
      }

      template< int I, typename DistanceFunction >
      void
      nearest_neighbor_impl( const T& value, node<I>* node, T** best, auto& best_distance, DistanceFunction f ) const noexcept {
        using maybe_actual_t = std::decay_t<decltype( Traits::get( std::declval<T>(), std::declval<dimension_t<I>>() ) )>;
        if( !node->m_left && !node->m_right ) {
          //Leaf case. Check if the current node is a better match.
          T* stored = __get_element__<T>( node );
          auto calculated_distance = f( value, *stored );
          if( calculated_distance < best_distance ) {
            *best = stored;
            best_distance = calculated_distance;
          }
        }
        else {
          maybe_actual_t& stored_value = *__get_element__<maybe_actual_t>( node );
          if( m_compare( Traits::get( value, dimension_t<I>{} ), stored_value ) ) {
            nearest_neighbor_impl( value, node->m_left, best, best_distance, f );
            if( f( value, stored_value, dimension_t<I>{} ) < best_distance ) { //If the distance from current point to dividing point is less than the best distance
              nearest_neighbor_impl( value, node->m_right, best, best_distance, f ); //There might be a better candidate on the other side.
            }
          }
          else {
            nearest_neighbor_impl( value, node->m_right, best, best_distance, f );
            if( f( value, stored_value, dimension_t<I>{} ) < best_distance ) { //If the distance from current point to dividing point is less than the best distance
              nearest_neighbor_impl( value, node->m_left, best, best_distance, f ); //There might be a better candidate on the other side.
            }
          }
        }
      }

      template< typename StoredType, int NodeDimension >
      static StoredType*
      __get_element__( node<NodeDimension>* ptr ) noexcept {
        struct retrieve_struct {
          std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
          std::aligned_storage_t<sizeof( StoredType ), alignof( StoredType )> m_actual;
        }; //We declare this struct to make the compiler do the dirty work for us
        retrieve_struct* retriever = reinterpret_cast<retrieve_struct*>( ptr );
        return reinterpret_cast<StoredType*>( &retriever->m_actual );
      }

    public:

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last, Allocator* allocator ):
            m_allocator( allocator ), m_compare{}, m_split{}, m_head( create_kd_tree<0>( first, last ) ) {
              static_assert( std::is_same_v<std::decay_t<decltype( *first )>, T> ); //We need this line to make sure we are constructing the correct type.
                                                                                    //Otherwise we would need a work around conversion.
      }

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last ):
            m_allocator( &memory::malloc_allocator ), m_compare{}, m_split{}, m_head( create_kd_tree<0>( first, last ) ) {
              static_assert( std::is_same_v<std::decay_t<decltype( *first )>, T> ); //We need this line to make sure we are constructing the correct type.
                                                                                    //Otherwise we would need a work around conversion.
              static_assert( std::is_same_v<Allocator, memory::malloc_allocator_t> );
      }

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last, CompareFunction compare, SplitFunction split, Allocator* allocator ):
            m_allocator( allocator ), m_compare( compare ), m_split( split ), m_head( create_kd_tree<0>( first, last ) ) {
              static_assert( std::is_same_v<std::decay_t<decltype( *first )>, T> ); //We need this line to make sure we are constructing the correct type.
                                                                                    //Otherwise we would need a work around conversion.
      }

      template< typename InputIterator, typename Sentinel >
      kd_tree( InputIterator first, Sentinel last, CompareFunction compare, SplitFunction split = SplitFunction{} ):
            m_allocator( &memory::malloc_allocator ), m_compare( compare ), m_split( split ), m_head( create_kd_tree<0>( first, last ) ) {
              static_assert( std::is_same_v<std::decay_t<decltype( *first )>, T> ); //We need this line to make sure we are constructing the correct type.
                                                                                    //Otherwise we would need a work around conversion.
            static_assert( std::is_same_v<Allocator, memory::malloc_allocator_t> );
      }

      ~kd_tree() {
        __remove_node__( m_head );
      }

      template< typename DistanceFunction = default_nearest_neighbour_function<Traits> >
      auto
      nearest_neighbor( const T& point, DistanceFunction f = DistanceFunction{} ) const noexcept {
        using distance_t = std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>;
        T* ret = nullptr;
        auto best_distance = numeric_limits<distance_t>::max();
        nearest_neighbor_impl( point, m_head, &ret, best_distance, f );
        return std::make_pair( *ret, best_distance );
      }

      template< typename DistanceFunction = default_nearest_neighbour_function<Traits>, typename Collection >
      Collection& k_nearest_neighbor( const T& point, Collection& output, DistanceFunction f = DistanceFunction{} ) const noexcept {
        //TODO
        return output;
      }

    };

  }

}

#endif
