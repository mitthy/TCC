#ifndef GEOMETRICKS_DATA_STRUCTURE_KD_TREE_HPP
#define GEOMETRICKS_DATA_STRUCTURE_KD_TREE_HPP

//C++ stdlib includes
#include <functional>
#include <type_traits>
#include <algorithm>
#include <queue>
#include <vector>

//Project includes
#include "dimensional_traits.hpp"
#include "geometricks/meta/utils.hpp"
#include "geometricks/memory/allocator.hpp"
#include "internal/small_vector.hpp"

/**
* @file Implements a cache friendly kd tree stored as an array.
*/

namespace geometricks {

  /**
  * @brief Cache friendly kd tree data structure
  * @tparam T The stored data type.
  * @tparam Compare Function that compares all the different data types stored in each dimension of the data so we can build the tree.
  * If the stored data type T is a std::tuple<int, std::string, float>, the function should be able to compare ( int, int ), ( std::string, std::string ),
  * ( float, float ) so we can work on all different dimensions.
  * @details This kd tree is stored as an array in memory. This gives better cache locality than node based kd trees. The elements are stored in the nodes.
  * Since it is extremely hard to balance a kd tree and it hurts performance to build a new one in each element insertion, insertion opperations are not allowed.
  * @see geometricks::dimension::dimensional_traits and @ref geometricks::dimension::get_t "geometricks::dimension::get" for a guide on how to use this struct with user defined types.
  * @see https://en.wikipedia.org/wiki/K-d_tree for a quick reference on kd tree.
  * @todo Static assert on compare so we know it can sort in all dimensions.
  * @todo noexcept and constexpr anotations.
  * @todo Add threshold neighbors to find all elements below threshold distance to efficiently implement collision detection algorithms. Maybe?
  */
  template< typename T,
            typename Compare = std::less<> >
  struct kd_tree : private Compare {

  private:

    struct __heap_compare__ {
      template< typename DistanceType >
      constexpr bool operator()( const std::pair<T*, DistanceType>& lhs, const std::pair<T*, DistanceType>& rhs ) const noexcept {
        return lhs.second < rhs.second;
      }
    };

  public:

    //Constructor

    /**
    * @brief Constructs a kd tree with a range of elements
    * @param begin Iterator to first element of the input range.
    * @param end Iterator to the last element of the input range or sentinel value.
    * @param comp Compare function to use for the kd tree. Should be able to sort objects in different dimensions. If not supplied, default constructs it.
    * @param alloc Memory allocator to use. Defaults to the default allocator. See also geometricks::allocator.
    * @pre If Sentinel is an iterator, first < last. Else, eventually first != last compares false.
    * @details Constructs a kd tree with the data supplied by the range [ begin, end ).
    *
    * @note Complexity: @b O(n log n)
    * @see https://en.wikipedia.org/wiki/K-d_tree#Complexity
    * @todo Supply paper on kd tree construction.
    */
    template< typename InputIterator, typename Sentinel >
    kd_tree( InputIterator begin, Sentinel end, Compare comp = Compare{}, geometricks::allocator alloc = geometricks::allocator{} ): Compare( comp ),
                                                                                                                  m_allocator( alloc ),
                                                                                                                  m_size( std::distance( begin, end ) ),
                                                                                                                  m_data_array( ( T* ) m_allocator.allocate( sizeof( T ) * m_size ) ) {
      __construct_kd_tree__<0>( begin, end, 0, m_size );
    }

    /**
    * @brief Constructs a kd tree with a range of elements
    * @param begin Iterator to first element of the input range.
    * @param end Iterator to the last element of the input range or sentinel value.
    * @param comp Placeholder used to call the default constructor for the Compare template parameter. See also geometricks::default_compare_t.
    * @param alloc Memory allocator to use. Defaults to the default allocator. See also geometricks::allocator.
    * @pre If Sentinel is an iterator, first < last. Else, eventually first != last compares false.
    * @details Constructs a kd tree with the data supplied by the range [ begin, end ).
    *
    * @note Complexity: @b O(n log n)
    * @see https://en.wikipedia.org/wiki/K-d_tree#Complexity
    * @todo Supply paper on kd tree construction.
    */
    template< typename InputIterator, typename Sentinel >
    kd_tree( InputIterator begin, Sentinel end, geometricks::default_compare_t comp, geometricks::allocator alloc = geometricks::allocator{} ):   m_allocator( alloc ),
                                                                                                                                      m_size( std::distance( begin, end ) ),
                                                                                                                                      m_data_array( ( T* ) m_allocator.allocate( sizeof( T ) * m_size ) ) {
      ( void ) comp; //Silence warnings and errors.
      __construct_kd_tree__<0>( begin, end, 0, m_size );
    }

    //Copy constructor

    /**
    * @brief Copy constructs a kd tree.
    * @param rhs Right hand side of the copy operation.
    * @param alloc Memory allocator to use. Defaults to the default allocator. See also geometricks::allocator
    * @details Performs a deep copy of the right hand side parameter.
    * @note Complexity: @b O(n)
    */
    kd_tree( const kd_tree& rhs, geometricks::allocator alloc = geometricks::allocator{} ):   Compare( rhs ),
                                                                                                          m_allocator( alloc ),
                                                                                                          m_size( rhs.m_size ),
                                                                                                          m_data_array( ( T* ) m_allocator.allocate( sizeof( T ) * m_size ) ) {
      std::copy( rhs.m_data_array, rhs.m_data_array + m_size, m_data_array );
    }

    //Move constructor

    /**
    * @brief Move constructs a kd tree.
    * @param rhs Right hand side of the move operation.
    * @post Invalidates rhs. Any use of rhs after move is an error.
    * @details Moves the data from rhs into this.
    * @note Complexity: @b O(1)
    */
    kd_tree( kd_tree&& rhs ): Compare( std::move( rhs ) ),
                                          m_allocator( rhs.m_allocator ),
                                          m_size( rhs.m_size ),
                                          m_data_array( rhs.m_data_array ) {
      rhs.m_data_array = nullptr;
    }

    //Copy assignment

    /**
    * @brief Copy assigns a kd tree.
    * @param rhs Right hand side of the copy operation.
    * @details Performs a deep copy of the right hand side parameter. Destroys the previous kd tree and allocates memory to construct rhs into this.
    * @note Complexity: @b O(n)
    * @todo Change this method so we only allocate if the buffer isn't large enough.
    * @todo Maybe we can get the strong exception guarantee here...
    */
    kd_tree& operator=( const kd_tree& rhs ) {
      if( &rhs != this ) {
        //TODO: optimize to only allocate new buffer in case old capacity wasn't enough.
        //TODO: exception guarantee.
        Compare::operator=( rhs );
        T* new_buff = ( T* ) m_allocator.allocate( sizeof( T ) * rhs.m_size );
        std::copy( rhs.m_data_array, rhs.m_data_array + rhs.m_size, new_buff );
        __destroy__();
        m_data_array = new_buff;
        m_size = rhs.m_size;
      }
      return *this;
    }

    //Move assignment

    /**
    * @brief Move assigns a kd tree.
    * @param rhs Right hand side of the move operation.
    * @post Invalidates rhs. Any use of rhs after move is an error.
    * @details Moves the data from rhs into this.
    * @note Complexity: @b O(1)
    */
    kd_tree& operator=( kd_tree&& rhs ) {
      if( &rhs != this ) {
        Compare::operator=( std::move( rhs ) );
        __destroy__();
        m_data_array = rhs.m_data_array;
        m_size = rhs.m_size;
        m_allocator = rhs.m_allocator;
        rhs.m_data_array = nullptr;
      }
      return *this;
    }

    ~kd_tree() {
      __destroy__();
    }

    /**
    * @brief Finds the nearest neighbor of an input point.
    * @param point The input point to query.
    * @param f Point distance function object. Should be able to compare 2 points and return a size type as well as
    * compare 2 points in a specific dimension and return a size type with the following signature: operator()( const T& left, T& right, dimension::dimension_t<Index> ) const noexcept.
    * Also, distance( point1, point2 ) should be equal to distance( point2, point1 ).
    * @details Computes the nearest neighbor of a given input point given the distance function. The default distance is the euclidean distance of the points without computing
    * the square root to save on efficiency, since if sqrt( euclid_distance_no_sqrt_root(a, b) < euclid_distance_no_sqrt_root(a, c) ), euclid_distance_no_sqrt_root(a, b) < euclid_distance_no_sqrt_root(a, c).
    *
    * Example:
    * @code{.cpp}
    * std::vector<std::tuple<int, int, int>> input_vector;
      ...
      geometricks::kd_tree<std::tuple<int, int, int>> tree{ input_vector.begin(), input_vector.end() };
      struct manhattan_distance_t {
        template< typename T, int N >
        size_t operator()( const T& lhs, const T& rhs, dimension::dimension_t<N> ) {
          return geometricks::algorithm::absolute_difference( geometricks::dimension::get( lhs, dimension::dimension_v<N> ), geometricks::dimension::get( rhs, dimension::dimension_v<N> ) );
        }
        template< typename T >
        size_t operator()( const T& lhs, const T& rhs ) {
          return distance_impl( lhs, rhs, std::make_index_sequence<dimension::dimensional_traits<T>::dimensions>() );
        }
        template< typename T, int... I >
        size_t distance_impl( const T& lhs, const T& rhs, std::index_sequence<I...> ) {
          return ( this->operator()( lhs, rhs, dimension_v<I> ) + ... );
        }
      };
      auto [nearest, distance] = tree.nearest_neighbor( std::make_tuple( 10, 10, 10 ), manhattan_distance_t{} );
    * @endcode
    * @todo Add references.
    * @todo Add complexity.
    * @todo Allow searching for threshold on nearest neighbor. Could be useful for code like collision detection.
    */
    template< typename DistanceFunction = dimension::euclidean_distance >
    auto
    nearest_neighbor( const T& point, DistanceFunction f = DistanceFunction{} ) const noexcept {
      using distance_t = std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>;
      distance_t best = meta::numeric_limits<distance_t>::max();
      T* closest = nullptr;
      __nearest_neighbor_impl__<0>( point, __root__(), &closest, best, f );
      return std::pair<const T&, distance_t>( *closest, best );
    }

    /**
    * @brief Finds the k nearest neighbors of an input point and returns a vector containing them and their distances.
    * @param point The input point to query.
    * @param K the number of desired output points.
    * @param f Point distance function object. Should be able to compare 2 points and return a size type as well as
    * compare 2 points in a specific dimension and return a size type with the following signature: operator()( const T& left, T& right, dimension::dimension_t<Index> ) const noexcept.
    * Also, distance( point1, point2 ) should be equal to distance( point2, point1 ).
    * @return A vector containing the output points as well as the distance calculated from the input point.
    * @details Computes the k nearest neighbor of a given input point given the distance function. The default distance is the euclidean distance of the points without computing
    * the square root to save on efficiency, since if sqrt( euclid_distance_no_sqrt_root(a, b) < euclid_distance_no_sqrt_root(a, c) ), euclid_distance_no_sqrt_root(a, b) < euclid_distance_no_sqrt_root(a, c).
    * The points are returned in ascending order.
    * Example:
    * @code{.cpp}
    * std::vector<std::tuple<int, int, int>> input_vector;
      ...
      geometricks::kd_tree<std::tuple<int, int, int>> tree{ input_vector.begin(), input_vector.end() };
      struct manhattan_distance_t {
        template< typename T, int N >
        size_t operator()( const T& lhs, const T& rhs, dimension::dimension_t<N> ) {
          return geometricks::algorithm::absolute_difference( geometricks::dimension::get( lhs, dimension::dimension_v<N> ), geometricks::dimension::get( rhs, dimension::dimension_v<N> ) );
        }
        template< typename T >
        size_t operator()( const T& lhs, const T& rhs ) {
          return distance_impl( lhs, rhs, std::make_index_sequence<dimension::dimensional_traits<T>::dimensions>() );
        }
        template< typename T, int... I >
        size_t distance_impl( const T& lhs, const T& rhs, std::index_sequence<I...> ) {
          return ( this->operator()( lhs, rhs, dimension_v<I> ) + ... );
        }
      };
      auto output_vector = tree.k_nearest_neighbor( std::make_tuple( 10, 10, 10 ), 4, manhattan_distance_t{} ); //output_vector now contains the 4 nearest neighbors of [10, 10, 10].
    * @endcode
    * @todo Add references.
    * @todo Add complexity.
    * @todo Allow searching for threshold on nearest neighbor. Could be useful for code like collision detection.
    * @todo Improve performance by using a stack allocated vector as the max heeap, only fallbacking to the heap in case of a big K.
    * @todo Allow alternative version of this function to receive the number of neighbors as a template parameter. Could be useful with a stack allocated vector.
    * @todo Make a new version of this function that doesn't require an output_col as a parameter but simply returns a vector.
    */
    template< typename DistanceFunction = dimension::euclidean_distance >
    auto
    k_nearest_neighbor( const T& point, uint32_t K, DistanceFunction f = DistanceFunction{} ) ->
    std::vector<std::pair<T, std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>>> {
      using distance_t = std::decay_t<decltype(f( std::declval<T>(), std::declval<T>() ))>;
      std::vector<std::pair<T, distance_t>> output_col;
      output_col.reserve( K );
      std::priority_queue<std::pair<T*, distance_t>, small_vector<std::pair<T*, distance_t>, 11>, __heap_compare__> max_heap;
      __k_nearest_neighbor_impl__<0, DistanceFunction, distance_t>( point, __root__(), K, max_heap, f );
      while( !max_heap.empty() ) {
        auto& element = max_heap.top();
        meta::add_element( std::make_pair( *element.first, element.second ), output_col );
        max_heap.pop();
      }
      std::reverse( output_col.begin(), output_col.end() );
      return output_col;
    }

    /**
    * @brief Performs a range query on the collection.
    * @param min_point Data containing the minimum values of the query.
    * @param max_point Data containing the maximum values of the query.
    * @return Vector containing all points in range.
    * @details Computes and gathers all given points that lie within the region min_point and max_point and outputs them in a vector.
    * Since the majority of the time is spent searching the tree for the output points, the algorithm first preprocess the input data so that there is no need for a precondition
    * that the minimum point contains all the minimum values. Instead, the algorithm sorts both points before processing.
    * Example:
    * @code{.cpp}
    std::vector<std::tuple<int, int, int>> input_vector;
    ...
    geometricks::kd_tree<std::tuple<int, int, int>> tree( input_vector.begin(), input_vector.end() );
    auto output_vector = tree.range_search( std::make_tuple( 0, 50, 300 ), std::make_tuple( 57, 51, 500 ) ); //output_vector now contains all points between [0-57, 50-51, 300-500] from tree.
    @endcode
    * @todo Allow the user to input don't care values into the minimum and maximum point. Would need a new data structure for that.
    */
    std::vector<T>
    range_search( T min_point, T max_point ) {
      std::vector<T> output_col;
      __organize_data__( min_point, max_point, std::make_index_sequence<DATA_DIMENSIONS>() );
      __range_search_impl__<0>( min_point, max_point, __root__(), output_col );
      return output_col;
    }

  private:

    geometricks::allocator m_allocator;

    int32_t m_size;

    T* m_data_array;

    static constexpr int DATA_DIMENSIONS = dimension::dimensional_traits<T>::dimensions;

    struct node_t {

      int32_t m_index;

      int32_t m_block_size;

      operator bool() const {
        return m_block_size;
      }

    };

    void
    __destroy__() {
      if( m_data_array != nullptr ) {
        for( int32_t i = 0; i < m_size; ++i ) {
          m_data_array[ i ].~T();
        }
        m_allocator.deallocate( m_data_array );
      }
    }

    template< int Dimension, typename DistanceFunction, typename DistanceType >
    void
    __nearest_neighbor_impl__( const T& point, const node_t& cur_node, T** closest, DistanceType& best_distance, DistanceFunction f ) const {
      constexpr size_t NextDimension = ( Dimension + 1 ) % DATA_DIMENSIONS;
      auto compare_function = [this]( const T& left, const T& right ) {
        return Compare::operator()( dimension::get( left, dimension::dimension_v<Dimension> ), dimension::get( right, dimension::dimension_v<Dimension> ) );
      };
      auto distance_function = [ &f ]( auto&& lhs, auto&& rhs ) {
        constexpr int I = Dimension;
        if constexpr( __detail__::has_dimension_compare<DistanceFunction, T, T, I> ) {
          return f( std::forward<decltype( lhs )>( lhs ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
        }
        else if constexpr( __detail__::has_dimension_compare<DistanceFunction, T, dimension::type_at<T, I>, I> ) {
          return f( std::forward<decltype( lhs )>( lhs ), dimension::get( std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> ), dimension::dimension_v<I> );
        }
        else if constexpr( __detail__::has_dimension_compare<DistanceFunction, dimension::type_at<T, I>, T, I> ) {
          return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
        }
        else if constexpr( __detail__::has_dimension_compare<DistanceFunction, dimension::type_at<T, I>, dimension::type_at<T, I>, I> ) {
          return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), dimension::get( std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> ), dimension::dimension_v<I> );
        }
        else {
          static_assert( __detail__::has_value_compare<DistanceFunction, dimension::type_at<T, I>, dimension::type_at<T, I>>, "Please supply a dimension compare, a value, value, dimension compare or a value compare." );
          return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), dimension::get( std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> ) );
        }
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
          auto distance_to_hyperplane = distance_function( point, m_data_array[ cur_node.m_index ] );
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
          auto distance_to_hyperplane = distance_function( point, m_data_array[ cur_node.m_index ] );
          if( distance_to_hyperplane < best_distance ) {
            __nearest_neighbor_impl__<NextDimension>( point, left_child, closest, best_distance, f );
          }
        }
      }
    }

    template< int Dimension,
              typename DistanceFunction,
              typename DistanceType >
    void __k_nearest_neighbor_impl__( const T& point,
                                      const node_t& node,
                                      uint32_t K,
                                      std::priority_queue<std::pair<T*, DistanceType>, small_vector<std::pair<T*, DistanceType>, 11>, __heap_compare__>& max_heap,
                                      DistanceFunction f ) {
      constexpr size_t NextDimension = ( Dimension + 1 ) % DATA_DIMENSIONS;
      auto compare_function = [this]( const T& left, const T& right ) {
        return Compare::operator()( dimension::get( left, dimension::dimension_v<Dimension> ), dimension::get( right, dimension::dimension_v<Dimension> ) );
      };

      auto distance_function = [ &f ]( auto&& lhs, auto&& rhs ) {
        constexpr int I = Dimension;
        if constexpr( __detail__::has_dimension_compare<DistanceFunction, T, T, I> ) {
          return f( std::forward<decltype( lhs )>( lhs ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
        }
        else if constexpr( __detail__::has_dimension_compare<DistanceFunction, T, dimension::type_at<T, I>, I> ) {
          return f( std::forward<decltype( lhs )>( lhs ), dimension::get( std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> ), dimension::dimension_v<I> );
        }
        else if constexpr( __detail__::has_dimension_compare<DistanceFunction, dimension::type_at<T, I>, T, I> ) {
          return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> );
        }
        else if constexpr( __detail__::has_dimension_compare<DistanceFunction, dimension::type_at<T, I>, dimension::type_at<T, I>, I> ) {
          return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), dimension::get( std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> ), dimension::dimension_v<I> );
        }
        else {
          static_assert( __detail__::has_value_compare<DistanceFunction, dimension::type_at<T, I>, dimension::type_at<T, I>>, "Please supply a dimension compare, a value, value, dimension compare or a value compare." );
          return f( dimension::get( std::forward<decltype( lhs )>( lhs ), dimension::dimension_v<I> ), dimension::get( std::forward<decltype( rhs )>( rhs ), dimension::dimension_v<I> ) );
        }
      };
      if( compare_function( point, m_data_array[ node.m_index ] ) ) {
        auto left_child = __left_child__( node );
        if( left_child ) {
          __k_nearest_neighbor_impl__<NextDimension, DistanceFunction, DistanceType>( point, left_child, K, max_heap, f );
        }
        auto distance = f( point, m_data_array[ node.m_index ] );
        auto heap_element = std::make_pair( &m_data_array[ node.m_index ], distance );
        max_heap.push( heap_element );
        if( ( uint32_t )max_heap.size() > K ) {
          max_heap.pop();
        }
        auto right_child = __right_child__( node );
        if( right_child ) {
          auto distance_to_hyperplane = distance_function( point, m_data_array[ node.m_index ] );
          if( ( uint32_t )max_heap.size() < K || distance_to_hyperplane < max_heap.top().second ) {
            __k_nearest_neighbor_impl__<NextDimension, DistanceFunction, DistanceType>( point, right_child, K, max_heap, f );
          }
        }
      }
      else {
        auto right_child = __right_child__( node );
        if( right_child ) {
          __k_nearest_neighbor_impl__<NextDimension, DistanceFunction, DistanceType>( point, right_child, K, max_heap, f );
        }
        auto distance = f( point, m_data_array[ node.m_index ] );
        auto heap_element = std::make_pair( &m_data_array[ node.m_index ], distance );
        max_heap.push( heap_element );
        if( ( uint32_t )max_heap.size() > K ) {
          max_heap.pop();
        }
        auto left_child = __left_child__( node );
        if( left_child ) {
          auto distance_to_hyperplane = distance_function( point, m_data_array[ node.m_index ] );
          if( ( uint32_t )max_heap.size() < K || distance_to_hyperplane < max_heap.top().second ) {
            __k_nearest_neighbor_impl__<NextDimension, DistanceFunction, DistanceType>( point, left_child, K, max_heap, f );
          }
        }
      }
    }

    template< int Dimension, typename InputIterator, typename Sentinel >
    void
    __construct_kd_tree__( InputIterator begin, Sentinel end, int32_t startind_index, int32_t blocksize ) {
      if( begin != end ) {
        constexpr size_t NextDimension = ( Dimension + 1 ) % DATA_DIMENSIONS;
        auto middle = begin;
        int step = blocksize >> 1;
        int32_t insert_index = startind_index + step;
        std::advance( middle, step );
        auto less_function = [this]( const T& left, const T& right ) {
          return Compare::operator()( dimension::get( left, dimension::dimension_v<Dimension> ), dimension::get( right, dimension::dimension_v<Dimension> ) );
        };
        std::nth_element( begin, middle ,end, less_function );
        new ( &m_data_array[ insert_index ] ) T{ *middle };
        __construct_kd_tree__<NextDimension>( begin, middle, startind_index, step );
        std::advance( middle, 1 );
        __construct_kd_tree__<NextDimension>( middle, end, insert_index + 1, blocksize - step - 1 );
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

    T&
    __get__( node_t node ) {
      return m_data_array[ node.m_index ];
    }

    const T&
    __get__( node_t node ) const {
      return m_data_array[ node.m_index ];
    }

    template< size_t... Is >
    void
    __organize_data__( T& first, T& second, std::index_sequence<Is...> ) {
      ( __swap_if_greater__( dimension::get( first, dimension::dimension_v<Is> ), dimension::get( second, dimension::dimension_v<Is> ) ), ... );
    }

    template< typename DataType >
    void
    __swap_if_greater__( DataType& first, DataType& second ) {
      if( !Compare::operator()( first, second ) ) {
        using std::swap;
        swap( first, second );
      }
    }

    template< int CurrentDimension, typename Collection >
    void
    __range_search_impl__( const T& min_point, const T& max_point, node_t current_node, Collection& output_collection ) {
      T& current_point = m_data_array[ current_node.m_index ];
      constexpr int NextDimension = ( CurrentDimension + 1 ) % DATA_DIMENSIONS;
      if( Compare::operator()( dimension::get( current_point, dimension::dimension_v<CurrentDimension> ), dimension::get( min_point, dimension::dimension_v<CurrentDimension> ) ) ) {
        //If we're to the "left" side of the minimum value, we can discard the left children of this node since all of them would be on the left as well.
        node_t next_node = __right_child__( current_node );
        if( next_node ) {
          __range_search_impl__<NextDimension>( min_point, max_point, next_node, output_collection );
        }
      }
      else if( Compare::operator()( dimension::get( max_point, dimension::dimension_v<CurrentDimension> ), dimension::get( current_point, dimension::dimension_v<CurrentDimension> ) ) ) {
        //If we're to the "right" side of the maximum value, we can discard the right children of this node since all of them would be on the right as well.
        node_t next_node = __left_child__( current_node );
        if( next_node ) {
          __range_search_impl__<NextDimension>( min_point, max_point, next_node, output_collection );
        }
      }
      else {
        //If we're within the actual range, we have to check both children.
        //Also, note that we only have to check other dimensions in the actual data if we're actually inside the range. If we're not in the range, it is not needed.
        if( __is_inside_bounding_box__<CurrentDimension>( current_point, min_point, max_point) ) {
          meta::add_element( m_data_array[ current_node.m_index ], output_collection );
        }
        node_t left_child = __left_child__( current_node );
        if( left_child ) {
          __range_search_impl__<NextDimension>( min_point, max_point, left_child, output_collection );
        }
        node_t right_child = __right_child__( current_node );
        if( right_child ) {
          __range_search_impl__<NextDimension>( min_point, max_point, right_child, output_collection );
        }
      }
    }

    template< int CurrentDimension >
    constexpr bool
    __is_inside_bounding_box__( const T& point, const T& min_point, const T& max_point ) {
      return __is_inside_bounding_box_helper__<CurrentDimension>( point, min_point, max_point, std::make_index_sequence<DATA_DIMENSIONS>{} );
    }

    template< int CurrentDimension, size_t... Is >
    constexpr bool
    __is_inside_bounding_box_helper__( const T& point, const T& min_point, const T& max_point, std::index_sequence<Is...> ) {
      return ( __is_inside_interval__<CurrentDimension, Is>( dimension::get( point, dimension::dimension_v<Is> ), dimension::get( min_point, dimension::dimension_v<Is> ), dimension::get( max_point, dimension::dimension_v<Is> ) ) && ... );
    }

    template< int CurrentDimension, int Index, typename DataType >
    constexpr bool
    __is_inside_interval__( const DataType& point, const DataType& min, const DataType& max ) {
      if constexpr( CurrentDimension == Index ) {
        return true;
      }
      else {
        return point >= min && point <= max;
      }
    }

  };

}

#endif //GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_kd_tree_HPP
