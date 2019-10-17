#ifndef GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_LEAF_KD_TREE_HPP
#define GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_LEAF_KD_TREE_HPP

//C stdlib includes
#include <stdint.h>
#include <assert.h>

//C++ stdlib includes
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>

//Project includes
#include "meta/detect.hpp"
#include "meta/utils.hpp"
#include "algorithm/mean.hpp"
#include "algorithm/partition.hpp"
#include "dimensional_traits.hpp"

namespace geometricks {

  namespace data_structure {

    namespace __detail__ {

      template< typename Underlying,
                int Dimension >
      struct dimensional_iterator {

      private:

        Underlying m_iterator;

      public:

        using iterator_category = typename std::iterator_traits<Underlying>::iterator_category;
        using value_type = dimension::type_at<typename Underlying::value_type, Dimension>;
        using difference_type = typename std::iterator_traits<Underlying>::difference_type;
        using pointer = value_type*;
        using reference = value_type&;

        dimensional_iterator(): m_iterator() {}
        explicit dimensional_iterator( const Underlying & it ): m_iterator( it ) { }
        dimensional_iterator( const dimensional_iterator& other ): m_iterator( other.m_iterator ) { }

        auto
        operator*() {
          return dimension::get( *m_iterator, dimension::dimension_v<Dimension> );
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

      template< typename T >
      struct insert_tag;

      struct push_back {};

      struct insert {};

      template< typename T >
      using push_back_expr = decltype( std::declval<T>().push_back( std::declval<typename T::value_type>() ) );

      template< typename T >
      using insert_expr = decltype( std::declval<T>().insert( std::declval<typename T::value_type>() ) );

      template< typename T >
      constexpr bool has_push_back = meta::is_valid_expression_v<push_back_expr, T>;

      template< typename T >
      constexpr bool has_insert = meta::is_valid_expression_v<insert_expr, T>;

      static_assert( has_push_back<std::vector<int>> );

      template< typename T,
                typename Container >
      void add_element( T&& element, Container& cont ) {
        if constexpr( has_push_back<Container> ) {
          cont.push_back( std::forward<T>( element ) );
        }
        else if constexpr( has_insert<Container> ) {
          cont.insert( std::forward<T>( element ) );
        }
        else {
          static_assert( geometricks::meta::always_false<T> );
        }
      }

    }

    struct mean_split_function {

      template< typename Iterator, typename Sentinel >
      auto
      operator()( Iterator first, Sentinel last ) const {
        return algorithm::mean( first, last );
      }

    };

    struct less_compare_function {

      template< typename T, typename U >
      bool operator()( const T& element, const U& split_value ) const noexcept {
        return element < split_value;
      }

    };

    template< typename T,
              typename SplitFunction = mean_split_function,
              typename CompareFunction = less_compare_function >
    struct leaf_kd_tree {

      static_assert( std::is_same_v<T, std::decay_t<T>>, "Can only create leaf_kd_tree of a value type." );

      using Traits = dimension::dimensional_traits<T>;


    public:

      //Constructors

      template< typename InputIterator,
                typename Sentinel >
      leaf_kd_tree( InputIterator first, Sentinel last ): m_compare{},
                                                          m_split{},
                                                          m_head( __create_leaf_kd_tree__<0>( first, last ) ) {
              static_assert( std::is_same_v<std::decay_t<decltype( *first )>, T> ); //We need this line to make sure we are constructing the correct type.
                                                                                    //Otherwise we would need a work around conversion.
      }

      template< typename InputIterator,
                typename Sentinel >
      leaf_kd_tree( InputIterator first, Sentinel last, CompareFunction compare, SplitFunction split = SplitFunction{} ): m_compare( compare ),
                                                                                                                          m_split( split ),
                                                                                                                          m_head( __create_leaf_kd_tree__<0>( first, last ) ) {
              static_assert( std::is_same_v<std::decay_t<decltype( *first )>, T> ); //We need this line to make sure we are constructing the correct type.
                                                                                    //Otherwise we would need a work around conversion.
      }

      //Copy constructors

      leaf_kd_tree( const leaf_kd_tree& rhs ) {
        struct dummy {};
        static_assert( meta::always_false<dummy>, "TODO: Not implemented yet." );
      }

      leaf_kd_tree( leaf_kd_tree&& rhs ): m_compare( rhs.m_compare ),
                                          m_split( rhs.m_split ),
                                          m_head( rhs.m_head ) {
        rhs.m_head = nullptr;
      }

      //Destructor

      ~leaf_kd_tree() {
        if( m_head  ) __remove_node__( m_head );
      }

      template< typename DistanceFunction = dimension::default_nearest_neighbour_function >
      decltype( auto )
      nearest_neighbor( const T& point, DistanceFunction f = DistanceFunction{} ) const noexcept {
        using distance_t = std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>;
        T* ret = nullptr;
        auto best_distance = meta::numeric_limits<distance_t>::max();
        __nearest_neighbor_impl__( point, m_head, &ret, best_distance, f );
        return std::pair<const T&, distance_t>( *ret, best_distance );
      }

      template< typename DistanceFunction = dimension::default_nearest_neighbour_function,
                typename Collection >
      Collection& k_nearest_neighbor( const T& point, uint32_t K, Collection& output, DistanceFunction f = DistanceFunction{} ) const noexcept {
        //TODO: instead of pushing objects one by one into the output collection, we can maybe think of a better way.
        using distance_t = std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>;
        static_assert( std::is_same_v<typename Collection::value_type, std::pair<T, distance_t>> );
        std::vector<std::pair<T*, distance_t>> max_heap;
        max_heap.reserve( K + 1 );
        __k_nearest_neighbor_impl__( point, m_head, K, max_heap, f );
        for( auto& element: max_heap ) {
          __detail__::add_element( std::make_pair( *element.first, element.second ), output );
        }
        return output;
      }

    private:

      static constexpr int Dimensions = Traits::dimensions;

      static_assert( Dimensions > 0 );

      template< int NodeDimension >
      struct node {

        friend class leaf_kd_tree;

        using child_t = node< NodeDimension + 1 == Dimensions ? 0 : NodeDimension + 1 >;

        node( ): m_left( nullptr ), m_right( nullptr ) {}

        node( child_t* left, child_t* right ): m_left( left ), m_right( right ) {}

        child_t* m_left;

        child_t* m_right;

      };

      struct compare_t {

        CompareFunction m_func;

        template< typename U, int I >
        constexpr bool
        operator()( const T& lhs, const U& rhs, dimension::dimension_t<I> ) const {
          if constexpr( __detail__::has_dimension_compare<CompareFunction, T, U, I> ) {
            return m_func( lhs, rhs, dimension::dimension_v<I> );
          }
          else if constexpr( __detail__::has_dimension_compare<CompareFunction, dimension::type_at<T, I>, U, I> ) {
            return m_func( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), rhs, dimension::dimension_v<I> );
          }
          else {
            static_assert( __detail__::has_value_compare<CompareFunction, dimension::type_at<T, I>, U> );
            return m_func( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), rhs );
          }
        }

        constexpr compare_t(): m_func{} {}

        constexpr compare_t( CompareFunction func ): m_func( func ) {}

      };

      compare_t m_compare;

      SplitFunction m_split;

      node<0>* m_head;

      template< int NodeDimension,
                typename Type,
                typename... Args >
      node<NodeDimension>*
      __create_node__( Type&& object, Args&&... args ) {
        using actual_t = std::decay_t<Type>;
        struct alloc_type {
          std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
          std::aligned_storage_t<sizeof( actual_t ), alignof( actual_t )> m_storage;
        };
        alloc_type* raw_memory = static_cast<alloc_type*>( ::operator new( sizeof( alloc_type ) ) );
        new ( &raw_memory->m_node ) node<NodeDimension>{ std::forward<Args>( args )... };
        new ( &raw_memory->m_storage ) actual_t{ std::forward<Type>( object ) };
        return reinterpret_cast<node<NodeDimension>*>( &( raw_memory->m_node ) );
      }

      template< int I,
                typename U >
      static auto
      __get__( U&& el ) {
        return dimension::get( std::forward<U>( el ), dimension::dimension_v<I> );
      }

      template< int NodeDimension >
      void
      __remove_node__( node<NodeDimension>* to_delete ) noexcept {
        using maybe_actual_t = std::decay_t<decltype( dimension::get( std::declval<T>(), std::declval<dimension::dimension_t<NodeDimension>>() ) )>;
        if( !to_delete->m_left && !to_delete->m_right ) {
          T* leaf = __get_element__<T>( to_delete );
          leaf->~T();
          struct alloc_type_leaf {
            std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
            std::aligned_storage_t<sizeof( T ), alignof( T )> m_storage;
          };
          ::operator delete( to_delete );
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
          struct alloc_type_root {
            std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
            std::aligned_storage_t<sizeof( maybe_actual_t ), alignof( maybe_actual_t )> m_storage;
          };
          ::operator delete( to_delete );
        }
      }

      template< int Dimension,
                typename InputIterator,
                typename Sentinel >
      node<Dimension>*
      __create_leaf_kd_tree__( InputIterator first, Sentinel last ) {
        if( std::distance( first, last ) == 1 ) {
          return __create_node__<Dimension>( *first );
        }
        else {
          auto begin = __detail__::dimensional_iterator<InputIterator, Dimension>( first );
          auto end = __detail__::dimensional_iterator<Sentinel, Dimension>( last );
          auto split = m_split( begin, end );
          auto temp_function = m_compare;
          auto partition_function = [&temp_function, split]( const auto& element ) {
            return temp_function( element, split, dimension::dimension_v<Dimension> );
          };
          auto middle = geometricks::algorithm::partition( first, last, partition_function );
          if( first == middle ) {
            middle = std::next( middle );
          }
          auto* left = __create_leaf_kd_tree__<Dimension + 1 == Dimensions ? 0 : Dimension + 1>( first, middle );
          auto* right = __create_leaf_kd_tree__<Dimension + 1 == Dimensions ? 0 : Dimension + 1>( middle, last );
          return __create_node__<Dimension>( split, left, right );
        }
      }

      template< int I,
                typename DistanceFunction >
      void
      __nearest_neighbor_impl__( const T& value, node<I>* node, T** best, auto& best_distance, DistanceFunction f ) const noexcept {
        using maybe_actual_t = std::decay_t<decltype( dimension::get( std::declval<T>(), std::declval<dimension::dimension_t<I>>() ) )>;
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
          auto dist_function = [ &f ]( auto&& lhs, auto&& rhs ) {
            if constexpr( __detail__::has_dimension_compare<DistanceFunction, T, maybe_actual_t, I> ) {
              return f( std::forward<decltype( lhs )>( lhs ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
            }
            else if constexpr( __detail__::has_dimension_compare<DistanceFunction, dimension::type_at<T, I>, maybe_actual_t, I> ) {
              return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
            }
            else {
              static_assert( __detail__::has_value_compare<DistanceFunction, dimension::type_at<T, I>, maybe_actual_t>, "Please supply a dimension compare, a value, value, dimension compare or a value compare." );
              return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), std::forward<decltype( rhs )>( rhs ) );
            }
          };
          if( m_compare( value, stored_value, dimension::dimension_v<I> ) ) {
            __nearest_neighbor_impl__( value, node->m_left, best, best_distance, f );
            if( dist_function( value, stored_value ) < best_distance ) { //If the distance from current point to dividing point is less than the best distance
              __nearest_neighbor_impl__( value, node->m_right, best, best_distance, f ); //There might be a better candidate on the other side.
            }
          }
          else {
            __nearest_neighbor_impl__( value, node->m_right, best, best_distance, f );
            if( dist_function( value, stored_value ) < best_distance ) { //If the distance from current point to dividing point is less than the best distance
              __nearest_neighbor_impl__( value, node->m_left, best, best_distance, f ); //There might be a better candidate on the other side.
            }
          }
        }
      }

      struct heap_compare {
        template< typename DistanceType >
        bool operator()( const std::pair<T*, DistanceType>& lhs, const std::pair<T*, DistanceType>& rhs ) const {
          return lhs.second < rhs.second;
        }
      };

      template< int I,
                typename DistanceFunction,
                typename DistanceType >
      void
      __k_nearest_neighbor_impl__( const T& value, node<I>* node, uint32_t K, std::vector<std::pair<T*, DistanceType>>& max_heap,  DistanceFunction f ) const noexcept {
        using maybe_actual_t = std::decay_t<decltype( dimension::get( std::declval<T>(), std::declval<dimension::dimension_t<I>>() ) )>;
        if( !node->m_left && !node->m_right ) {
          T* stored = __get_element__<T>( node );
          auto calculated_distance = f( value, *stored );
          auto heap_element = std::make_pair( stored, calculated_distance );
          max_heap.push_back( heap_element );
          std::push_heap( max_heap.begin(), max_heap.end(), heap_compare{} );
          if( max_heap.size() > K ) {
            std::pop_heap( max_heap.begin(), max_heap.end(), heap_compare{} );
            max_heap.pop_back();
          }
        }
        else {
          maybe_actual_t& stored_value = *__get_element__<maybe_actual_t>( node );
          auto dist_function = [ &f ]( auto&& lhs, auto&& rhs ) {
            if constexpr( __detail__::has_dimension_compare<DistanceFunction, T, maybe_actual_t, I> ) {
              return f( std::forward<decltype( lhs )>( lhs ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
            }
            else if constexpr( __detail__::has_dimension_compare<DistanceFunction, dimension::type_at<T, I>, maybe_actual_t, I> ) {
              return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
            }
            else {
              static_assert( __detail__::has_value_compare<DistanceFunction, dimension::type_at<T, I>, maybe_actual_t>, "Please supply a dimension compare, a value, value, dimension compare or a value compare." );
              return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), std::forward<decltype( rhs )>( rhs ) );
            }
          };
          if( m_compare( value, stored_value, dimension::dimension_v<I> ) ) {
            __k_nearest_neighbor_impl__( value, node->m_left, K, max_heap, f );
            if( max_heap.size() < K || dist_function( value, stored_value ) < max_heap.front().second ) { //If we need more points or the  split point is a better match
              __k_nearest_neighbor_impl__( value, node->m_right, K, max_heap, f ); //There might be a better candidate on the other side.
            }
          }
          else {
            __k_nearest_neighbor_impl__( value, node->m_right, K, max_heap, f );
            if( max_heap.size() < K || dist_function( value, stored_value ) < max_heap.front().second  ) { //If we need more points or the  split point is a better match
              __k_nearest_neighbor_impl__( value, node->m_left, K, max_heap, f ); //There might be a better candidate on the other side.
            }
          }
        }
      }

      template< typename StoredType,
                int NodeDimension >
      static StoredType*
      __get_element__( node<NodeDimension>* ptr ) noexcept {
        struct retrieve_struct {
          std::aligned_storage_t<sizeof( node<NodeDimension> ), alignof( node<NodeDimension> )> m_node;
          std::aligned_storage_t<sizeof( StoredType ), alignof( StoredType )> m_actual;
        }; //We declare this struct to make the compiler do the dirty work for us
        retrieve_struct* retriever = reinterpret_cast<retrieve_struct*>( ptr );
        return reinterpret_cast<StoredType*>( &retriever->m_actual );
      }

    };

  } //namespace data_structure

} //namespace geometricks

#endif //GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_LEAF_KD_TREE_HPP
