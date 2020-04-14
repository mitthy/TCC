#ifndef GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
#define GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP

//C++ stblib includes
#include <utility>
#include <tuple>
#include <array>

//Project includes
#include "geometricks/meta/utils.hpp"
#include "geometricks/meta/detect.hpp"
#include "geometricks/algorithm/absolute_difference.hpp"

namespace geometricks {

    namespace dimension {

      /**
      * @brief struct that represents a specific dimension, with 0 being the first.
      * @see @ref geometricks::dimension::get_t "get".
      */
      template< int Index >
      struct dimension_t {};

      template< int Index >
      constexpr dimension_t<Index> dimension_v;

      /**
      * @brief Traits that, for a given type, output how many dimensions that type has.
      * @tparam T The user defined type.
      * @details Specialize this struct with your own type and expose a static constexpr int dimensions variable that should, for the given type, tell
      * how many dimensions there are in the type. This should be done so that user defined types can integrate with the library.
      * Example:
      * @code{.cpp}
        namespace thirdparty {
          struct data_t {
            int first;
            int second;
          };
        }
        namespace geometricks::dimension {
          template<>
          struct dimensional_traits<thirdparty::data_t> {
            static constexpr int dimensions = 2;
          };
        }
      * @endcode
      * @see @ref geometricks::dimension::get_t "geometricks::dimension::get".
      */
      template< typename T >
      struct dimensional_traits;

      template< typename... Types >
      struct dimensional_traits< std::tuple<Types...> > {

        static constexpr int dimensions = sizeof...( Types );

      };

      template< typename T, typename U >
      struct dimensional_traits<std::pair<T, U>> {

        static constexpr int dimensions = 2;

      };

      template< typename T, size_t N >
      struct dimensional_traits<std::array<T,N>> {
        static constexpr int dimensions = N;
      };

      namespace get_customization {

        /**
        * @brief Template struct to customize access to data type dimensions.
        * @tparam T The type.
        * @details Specialize this struct in case you don't have access to the type namespace in order to allow easy integration with the library.
        * For that, expose static methods that take const T& as the first parameter and geometricks::dimension::dimension_t as the dimension.
        * Example:
        * @code{.cpp}
          namespace thirdparty {
            struct data_t {
              int first;
              int second;
            };
          }
          namespace geometricks::dimension::get_customization {
            template<>
            struct get<thirdparty::data_t> {

              //Could return a const int&. Returning a copy for performance reasons.
              static int _( const thirdparty::data_t& data, geometricks::dimension::dimension_t<0> ) {
                return data.first;
              }

              //Could return a const int&. Returning a copy for performance reasons.
              static int _( const thirdparty::data_t& data, geometricks::dimension::dimension_t<1> ) {
                return data.second;
              }

            };

          }
        * @endcode
        * @see geometricks::dimension::dimensional_traits and geometricks::dimension::dimension_t.
        */
        template< typename T >
        struct get;

      }

      /**
      * @cond EXCLUDE_DOXYGEN
      *
      * Internal not to be documented
      */
      namespace __detail__ {

        template< int I >
        constexpr decltype( auto )
        get( int );

        template< int I >
        constexpr decltype( auto )
        get( int, dimension_t<I> );

        template< int I, typename T >
        constexpr decltype( auto )
        __get__( T&& value, geometricks::meta::priority_tag<0> ) -> decltype( get<I>( std::forward<T>( value ) ) ) {
          return get<I>( std::forward<T>( value ) );
        }

        template< int I, typename T >
        constexpr decltype( auto )
        __get__( T&& value, geometricks::meta::priority_tag<1> ) -> decltype( get( std::forward<T>( value ), dimension_v<I> ) ) {
          return get( std::forward<T>( value ), dimension_v<I> );
        }

        template< int I, typename T >
        constexpr decltype( auto )
        __get__( T&& value, geometricks::meta::priority_tag<2> ) -> decltype( get_customization::get<std::decay_t<T>>::_( std::forward<T>( value ), dimension_v<I> ) ) {
          return get_customization::get<std::decay_t<T>>::_( std::forward<T>( value ), dimension_v<I> );
        }

      }
      /**
      * @endcond
      */

      /**
      * @brief struct that extracts the data in a given dimension from a type.
      * @details The data is extracted according to the following protocol: first, it tries to access a specialization for the geometricks::dimension::get_customization::get
      * template struct for the given type and call a static function named _ that accepts the data as the first parameter and a geometricks::dimension::dimension_t as the second parameter.
      * If there is no specialization, it tries to access a
      * get free function that accepts both the data and a geometricks::dimension::dimension_t for the specific dimension. Finally, if none of the alternatives can be done, it
      * tries calling a get free function that takes an index representing the current dimension template argument and the data type as an argument. In case none of those functions are present,
      * it is a compilation error.
      * @note In case you have access to the data type namespace, you can just supply a free function taking a geometricks::dimension::dimension_t for each of the types dimension
      * to allow the library to access your type.
      * @see geometricks::dimension::get_customization::get for details on how to customize the type in case you don't have access to the type namespace.
      */
      struct get_t {

        template< typename T, int I >
        constexpr decltype( auto )
        operator() ( T&& value, dimension_t<I> ) const {
          return __detail__::__get__<I>( std::forward<T>( value ), geometricks::meta::priority_tag<2>{} );
        }

      };

      constexpr get_t get = {};

      template< typename T, int I >
      using type_at = std::decay_t<decltype( get( std::declval<T>(), dimension_v<I> ) )>;

      /**
      * @todo Move this to another file and implement other distance functions.
      */
      struct euclidean_distance {

      private:

        template< typename T >
        static auto
        element_distance( const T& lhs, const T& rhs ) noexcept {
          auto tmp = algorithm::absolute_difference( lhs, rhs );
          return tmp * tmp;
        }

        template< size_t... Index >
        auto
        distance_impl( const auto& lhs, const auto& rhs, std::index_sequence<Index...> ) const noexcept {
          return ( element_distance( dimension::get( lhs, dimension_t<Index>{} ), dimension::get( rhs, dimension_t<Index>{} ) ) + ... );
        }

      public:

        template< typename T, typename U, int Index >
        auto
        operator()( const T& element, const U& stored, dimension_t<Index> ) const noexcept {
          return element_distance( dimension::get( element, dimension_t<Index>{} ), stored );
        }

        template< typename T, int Index >
        auto
        operator()( const T& element, const T& stored, dimension_t<Index> ) const noexcept {
          return element_distance( dimension::get( element, dimension_t<Index>{} ), dimension::get( stored, dimension_t<Index>{} ) );
        }

        template< typename T >
        auto
        operator()( const T& lhs, const T& rhs ) const noexcept {
          return distance_impl( lhs, rhs, std::make_index_sequence<dimensional_traits<T>::dimensions>{} );
        }

      };

    } //namespace dimension

    /**
    * @cond EXCLUDE_DOXYGEN
    *
    * Internal not to be documented
    */
    namespace __detail__ {

      template< typename Functor, typename T, typename U, typename Dimension >
      using compare_dimension = decltype( std::declval<Functor>()( std::declval<T>(), std::declval<U>(), std::declval<Dimension>() ) );

      template< typename Functor, typename T, typename U >
      using compare_value = decltype( std::declval<Functor>()( std::declval<T>(), std::declval<U>() ) );

      template< typename Functor, typename T, typename U, int I >
      constexpr bool has_dimension_compare = meta::is_valid_expression_v<compare_dimension, Functor, T, U, dimension::dimension_t<I>>;

      template< typename Functor, typename T, typename U >
      constexpr bool has_value_compare = meta::is_valid_expression_v<compare_value, Functor, T, U>;

    }
    /**
    * @endcond
    */



} //namespace geometricks

#endif //GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
