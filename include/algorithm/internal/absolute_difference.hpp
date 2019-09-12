#ifndef TCC_ALGORITHM_INTERNAL_ABSOLUTE_DIFFERENCE_HPP
#define TCC_ALGORITHM_INTERNAL_ABSOLUTE_DIFFERENCE_HPP

//C stdlib includes
#include <stdint.h>
#include <cmath>

//C++ stdlib includes
#include <algorithm>
#include <type_traits>

namespace tcc {

  namespace algorithm {

    struct signed_t {};

    struct unsigned_t {};

    struct string_t {};

    namespace __detail__ {

      template< typename T >
      size_t absolute_difference( T first, T second, signed_t ) {
        using std::abs;
        return abs( first - second );
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
