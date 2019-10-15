#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_ARRAY_KD_TREE_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_ARRAY_KD_TREE_HPP

//C++ stdlib includes
#include <functional>
#include <type_traits>
#include <algorithm>

//Project includes
#include "dimensional_traits.hpp"
#include "algorithm/distance.hpp"
#include "meta/utils.hpp"

namespace tcc {

  namespace data_structure {

    template< typename T,
              typename Compare = std::less<> >
    struct array_kd_tree : Compare {

      //TODO: static assert on compare so we know it can sort in all dimensions.

      //Constructor

      template< typename InputIterator, typename Sentinel >
      array_kd_tree( InputIterator begin, Sentinel end ): m_size( algorithm::distance( begin, end ) ), m_data_array( ( T* ) ::operator new( sizeof( T ) * m_size ) ) {
        __construct_array_kd_tree__<0>( begin, end, 0, m_size );
      }

      //Copy constructor

      array_kd_tree( const array_kd_tree& rhs ):  m_size( rhs.m_size ),
                                                  m_data_array( ( T* ) ::operator new( sizeof( T ) * m_size ) ) {
        std::copy( rhs.m_data_array, rhs.m_data_array + m_size, m_data_array );
      }

      //Move constructor

      array_kd_tree( array_kd_tree&& rhs ): m_size( rhs.m_size ),
                                            m_data_array( rhs.m_data_array ) {
        rhs.m_data_array = nullptr;
      }

      //Copy assignment

      array_kd_tree& operator=( const array_kd_tree& rhs ) {
        if( &rhs != this ) {
          //TODO: optimize to only allocate new buffer in case old capacity wasn't enough.
          //TODO: exception guarantee.
          T* new_buff = ( T* ) ::operator new( sizeof( T ) * rhs.m_size );
          std::copy( rhs.m_data_array, rhs.m_data_array + rhs.m_size, new_buff );
          __destroy__();
          m_data_array = new_buff;
          m_size = rhs.m_size;
        }
        return *this;
      }

      //Move assignment

      array_kd_tree& operator=( array_kd_tree&& rhs ) {
        if( &rhs != this ) {
          m_data_array = rhs.m_data_array;
          m_size = rhs.m_size;
          rhs.m_data_array = nullptr;
        }
        return *this;
      }

      ~array_kd_tree() {
        __destroy__();
      }

      template< typename DistanceFunction = default_nearest_neighbour_function >
      decltype( auto )
      nearest_neighbor( const T& point, DistanceFunction f = DistanceFunction{} ) const noexcept {
        using distance_t = std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>;
        distance_t best = meta::numeric_limits<distance_t>::max();
        T* closest = nullptr;
        __nearest_neighbor_impl__<0>( point, __root__(), &closest, best, f );
        return std::pair<const T&, distance_t>( *closest, best );
      }

    private:

      int32_t m_size;

      T* m_data_array;

      struct node_t {
        int32_t m_index;
        int32_t m_block_size;
        operator bool() const {
          return m_block_size;
        }
      };

      void
      __destroy__() {
        for( int32_t i = 0; i < m_size; ++i ) {
          m_data_array[ i ].~T();
        }
        ::operator delete( m_data_array );
      }

      template< int Dimension, typename DistanceFunction, typename DistanceType >
      void
      __nearest_neighbor_impl__( const T& point, const node_t& cur_node, T** closest, DistanceType& best_distance, DistanceFunction f ) const {
        constexpr size_t NextDimension = ( Dimension + 1 ) % dimensional_traits<T>::dimensions;
        auto compare_function = []( const T& left, const T& right ) {
          Compare comp{};
          return comp( dimension::get( left, dimension_v<Dimension> ), dimension::get( right, dimension_v<Dimension> ) );
        };
        if( compare_function( point, m_data_array[ cur_node.m_index ] ) ) {
          //The point is to the left of the current axis.
          //Recurse left...
          auto left_child = __left_child__( cur_node );
          if( left_child ) {
            __nearest_neighbor_impl__<NextDimension>( point, left_child, closest, best_distance, f );
          }
          //Now we get the distance from the point to the current node.
          auto distance = f( point, m_data_array[ cur_node.m_index ] );
          //If the distance is better than our current best distance, update it.
          if( distance < best_distance ) {
            best_distance = distance;
            *closest = &m_data_array[ cur_node.m_index ];
          }
          //If we have another branch to search...
          auto right_child = __right_child__( cur_node );
          if( right_child ) {
            //Finally, check the distance to the hyperplane.
            auto distance_to_hyperplane = f( point, m_data_array[ cur_node.m_index ], dimension_v<Dimension> );
            if( distance_to_hyperplane < best_distance ) {
              __nearest_neighbor_impl__<NextDimension>( point, right_child, closest, best_distance, f );
            }
          }

        }
        else {
          //The point is to the right of the current axis.
          //Recurse right..
          auto right_child = __right_child__( cur_node );
          if( right_child ) {
            __nearest_neighbor_impl__<NextDimension>( point, right_child, closest, best_distance, f );
          }
          //Now we get the distance from the point to the current node.
          auto distance = f( point, m_data_array[ cur_node.m_index ] );
          //If the distance is better than our current best distance, update it.
          if( distance < best_distance ) {
            best_distance = distance;
            *closest = &m_data_array[ cur_node.m_index ];
          }
          //If we have another branch to search...
          auto left_child = __left_child__( cur_node );
          if( left_child ) {
            //Finally, check the distance to the hyperplane.
            auto distance_to_hyperplane = f( point, m_data_array[ cur_node.m_index ], dimension_v<Dimension> );
            if( distance_to_hyperplane < best_distance ) {
              __nearest_neighbor_impl__<NextDimension>( point, left_child, closest, best_distance, f );
            }
          }
        }
      }

      template< int Dimension, typename InputIterator, typename Sentinel >
      void
      __construct_array_kd_tree__( InputIterator begin, Sentinel end, int32_t startind_index, int32_t blocksize ) {
        if( begin != end ) {
          constexpr size_t NextDimension = ( Dimension + 1 ) % dimensional_traits<T>::dimensions;
          auto middle = begin;
          int step = blocksize >> 1;
          int32_t insert_index = startind_index + step;
          std::advance( middle, step );
          auto less_function = []( const T& left, const T& right ) {
            Compare comp{};
            return comp( dimension::get( left, dimension_v<Dimension> ), dimension::get( right, dimension_v<Dimension> ) );
          };
          std::nth_element( begin, middle ,end, less_function );
          new ( &m_data_array[ insert_index ] ) T{ *middle };
          __construct_array_kd_tree__<NextDimension>( begin, middle, startind_index, step );
          std::advance( middle, 1 );
          __construct_array_kd_tree__<NextDimension>( middle, end, insert_index + 1, blocksize - step - 1 );
        }
      }

      template< typename _T >
      constexpr bool
      __compare__( const _T& first, const _T& second ) const {
        return Compare::operator()( first, second );
      }

      node_t
      __root__() const {
        return { m_size >> 1, m_size };
      }

      static node_t
      __left_child__( node_t node ) {
        int32_t count_left = node.m_block_size >> 1;
        int32_t index_left = ( node.m_index - ( count_left >> 1 ) - ( count_left & 1 ) );
        return { index_left, count_left };
      }

      static node_t
      __right_child__( node_t node ) {
        int32_t count_right = ( node.m_block_size >> 1 ) - !( node.m_block_size & 1 );
        int32_t index_right = ( node.m_index + ( count_right >> 1 ) + 1 );
        return { index_right, count_right };
      }

      T& __get__( node_t node ) {
        return m_data_array[ node.m_index ];
      }

      const T& __get__( node_t node ) const {
        return m_data_array[ node.m_index ];
      }

    };

  }

}

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_ARRAY_KD_TREE_HPP
