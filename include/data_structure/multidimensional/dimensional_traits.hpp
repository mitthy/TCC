#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP

#include <utility>
#include <tuple>
#include "meta/utils.hpp"
#include "meta/detect.hpp"

namespace tcc {

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
        __get__( T&& value, tcc::meta::priority_tag<0> ) -> decltype( get<I>( std::forward<T>( value ) ) ) {
          return get<I>( std::forward<T>( value ) );
        }

        template< int I, typename T >
        constexpr decltype( auto )
        __get__( T&& value, tcc::meta::priority_tag<1> ) -> decltype( get( std::forward<T>( value ), dimension_v<I> ) ) {
          return get( std::forward<T>( value ), dimension_v<I> );
        }

        template< int I, typename T >
        constexpr decltype( auto )
        __get__( T&& value, tcc::meta::priority_tag<2> ) -> decltype( get_customization::get<T>::template _<I>( std::forward<T>( value ) ) ) {
          return get_customization::get<T>::template _<I>( std::forward<T>( value ) );
        }

        struct __get_t__ {

          template< typename T, int I >
          constexpr decltype( auto )
          operator() ( T&& value, dimension_t<I> ) const {
            return __get__<I>( std::forward<T>( value ), tcc::meta::priority_tag<2>{} );
          }

        };

      }

      constexpr __detail__::__get_t__ get = {};

      template< typename T, int I >
      using type_at = std::decay_t<decltype( get( std::declval<T>(), dimension_v<I> ) )>;

    }

  } //namespace data_structure

} //namespace tcc

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
