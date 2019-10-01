#ifndef TCC_ALGORITHM_INTERNAL_ABSOLUTE_DIFFERENCE_HPP
#define TCC_ALGORITHM_INTERNAL_ABSOLUTE_DIFFERENCE_HPP

//C stdlib includes
#include <stdint.h>
#include <cmath>

//C++ stdlib includes
#include <algorithm>
#include <type_traits>

//Project includes
#include "meta/utils.hpp"

namespace tcc {

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
      constexpr auto
      __abs__( const T& value, meta::priority_tag<0> ) -> decltype( value < 0 ? -value : value ) {
        return value < 0 ? -value : value;
      }

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<1> ) -> decltype( std::abs( value ) ) {
        return std::abs( value );
      }

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<2> ) -> decltype( abs_customization::abs<T>::_( value ) ) {
        return abs_customization::abs<T>::_( value );
      }

    }

    constexpr auto
    abs = []( const auto& value ) -> decltype( __detail__::__abs__( value, meta::priority_tag<2>{} ) ) {
      return __detail__::__abs__( value, meta::priority_tag<2>{} );
    };

    namespace __detail__ {

      template< typename T >
      size_t absolute_difference( T first, T second, signed_t ) {
        return algorithm::abs( first - second );
      }

      template< typename T >
      size_t absolute_difference( T first, T second, unsigned_t ) {
        return first > second ? first - second : second - first;
      }

      template< typename T >
      size_t absolute_difference( T first, T second, string_t ) {
        return 0; //TODO
      }

    }  //namespace __detail__

    template< typename T >
    struct difference_traits {
      using tag = signed_t;
    };

    template<>
    struct difference_traits< unsigned char > {
      using tag = unsigned_t;
    };

    template<>
    struct difference_traits< unsigned int > {
      using tag = unsigned_t;
    };

    template<>
    struct difference_traits< unsigned long > {
      using tag = unsigned_t;
    };

    template< typename T >
    struct difference_traits< const T > : difference_traits< T > {};

    template< typename T >
    struct difference_traits< volatile T > : difference_traits< T > {};

    template< typename T >
    struct difference_traits< const volatile T > : difference_traits< T > {};

    template< typename T >
    using difference_tag_t = typename difference_traits< T >::tag;

    template< typename T >
    size_t absolute_difference( T first, T second ) {
      return __detail__::absolute_difference( first, second, difference_tag_t<T>{} );
    }

    template< typename U, typename T >
    size_t absolute_difference( T first, U second ) {
      if( first > second ) {
        return first - second;
      }
      else {
        return second - first;
      }
    }

  } //namespace algorithm

} //namespace tcc

#endif
