#ifndef GEOMETRICKS_ALGORITHM_ABSOLUTE_DIFFERENCE_HPP
#define GEOMETRICKS_ALGORITHM_ABSOLUTE_DIFFERENCE_HPP

//C stdlib includes
#include <stdint.h>
#include <cmath>

//C++ stdlib includes
#include <algorithm>
#include <type_traits>

//Project includes
#include "meta/utils.hpp"

namespace geometricks {

  namespace algorithm {

    struct signed_t {};

    struct unsigned_t {};

    struct string_t {};

    namespace abs_customization {

      //Implement static method _ to customize for your type.
      template< typename T >
      struct abs;

    }

    namespace __detail__ {

      template< typename T >
      using is_unsigned = std::enable_if_t<std::is_unsigned_v<T>, bool>;

      template< typename T >
      using is_signed = std::enable_if_t<std::is_signed_v<T>, bool>;

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<0> ) -> decltype( value < 0 ? -value : value ) {
        return value < 0 ? -value : value;
      }

      template< typename T, is_signed<T> = true >
      constexpr size_t
      __abs__( const T& value, meta::priority_tag<1> ) {
        return value < 0 ? -value : value;
      }

      template< typename T, is_unsigned<T> = true >
      constexpr size_t
      __abs__( const T& value, meta::priority_tag<2> ) {
        return value;
      }

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<3> ) -> decltype( static_cast<size_t>( std::abs( value ) ) ) {
        return static_cast<size_t>( std::abs( value ) );
      }

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<4> ) -> decltype( abs_customization::abs<T>::_( value ) ) {
        return abs_customization::abs<T>::_( value );
      }

    }

    constexpr auto
    abs = []( const auto& value ) -> decltype( __detail__::__abs__( value, meta::priority_tag<4>{} ) ) {
      return __detail__::__abs__( value, meta::priority_tag<2>{} );
    };

    namespace absolute_difference_customization {

      //Implement static method _ to customize for your type.
      template< typename T >
      struct absolute_difference;

    }

    namespace __detail__ {

      template< typename T >
      using is_signed = std::enable_if_t<std::is_signed_v<T>, bool>;

      template< typename T >
      using is_unsigned = std::enable_if_t<std::is_unsigned_v<T>, bool>;

      template< typename T >
      using is_string = std::enable_if_t<std::is_same_v<T, std::string>, bool>;

      template< typename T, is_signed<T> = true >
      constexpr size_t
      __absolute_difference__( T first, T second, meta::priority_tag<0> ) {
        return algorithm::abs( first - second );
      }

      template< typename T, is_unsigned<T> = true >
      constexpr size_t
      __absolute_difference__( T first, T second, meta::priority_tag<1> ) {
        return first > second ? first - second : second - first;
      }

      template< typename T, is_string<T> = true >
      constexpr size_t
      __absolute_difference__( T first, T second, meta::priority_tag<2> ) {
        return 0; //TODO
      }

      template< typename T >
      constexpr auto
      __absolute_difference__( T first, T second, meta::priority_tag<3> ) -> decltype( absolute_difference_customization::absolute_difference<T>::_( first, second ) ) {
        return absolute_difference_customization::absolute_difference<T>::_( first, second );
      }

    }  //namespace __detail__

    constexpr auto
    absolute_difference = []( auto&& first, auto&& second ) ->
    decltype( __detail__::__absolute_difference__( std::forward<decltype( first )>( first ), std::forward<decltype( second )>( second ), meta::priority_tag<3>{} ) ) {
      static_assert( std::is_same_v<std::decay_t<decltype( first )>, std::decay_t<decltype( second )>> );
      return __detail__::__absolute_difference__( std::forward<decltype( first )>( first ), std::forward<decltype( second )>( second ), meta::priority_tag<3>{} );
    };

  } //namespace algorithm

} //namespace geometricks

#endif //GEOMETRICKS_ALGORITHM_ABSOLUTE_DIFFERENCE_HPP
