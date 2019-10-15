#ifndef GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
#define GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP

#include <utility>
#include <tuple>
#include "meta/utils.hpp"
#include "algorithm/absolute_difference.hpp"

namespace geometricks {

  namespace data_structure {

    template< int Index >
    struct dimension_t {};

    template< int Index >
    constexpr dimension_t<Index> dimension_v;

    template< typename T >
    struct dimensional_traits;

    template< typename... Types >
    struct dimensional_traits< std::tuple<Types...> > {

      static constexpr int dimensions = sizeof...( Types );

    };

    namespace dimension {

      namespace get_customization {

        template< typename T >
        struct get;

      }

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
        __get__( T&& value, geometricks::meta::priority_tag<2> ) -> decltype( get_customization::get<T>::template _<I>( std::forward<T>( value ) ) ) {
          return get_customization::get<T>::template _<I>( std::forward<T>( value ) );
        }

        struct __get_t__ {

          template< typename T, int I >
          constexpr decltype( auto )
          operator() ( T&& value, dimension_t<I> ) const {
            return __get__<I>( std::forward<T>( value ), geometricks::meta::priority_tag<2>{} );
          }

        };

      }

      constexpr __detail__::__get_t__ get = {};

      template< typename T, int I >
      using type_at = std::decay_t<decltype( get( std::declval<T>(), dimension_v<I> ) )>;

    }

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
        return element_distance( dimension::get( element, dimension_t<Index>{} ), stored );
      }

      template< typename T, int Index >
      auto
      operator()( const T& element, const T& stored, const dimension_t<Index> ) const noexcept {
        return element_distance( dimension::get( element, dimension_t<Index>{} ), dimension::get( stored, dimension_t<Index>{} ) );
      }

      template< size_t... Index >
      auto
      distance_impl( const auto& lhs, const auto& rhs, std::index_sequence<Index...> ) const noexcept {
        return ( element_distance( dimension::get( lhs, dimension_t<Index>{} ), dimension::get( rhs, dimension_t<Index>{} ) ) + ... );
      }

      template< typename T >
      auto
      operator()( const T& lhs, const T& rhs ) const noexcept {
        return distance_impl( lhs, rhs, std::make_index_sequence<dimensional_traits<T>::dimensions>{} );
      }
    };

  } //namespace data_structure

} //namespace geometricks

#endif //GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
