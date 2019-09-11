#ifndef TCC_DATA_STRUCTURE_UTILS_MEAN_HPP
#define TCC_DATA_STRUCTURE_UTILS_MEAN_HPP

#include "distance.hpp"
#include <numeric>

namespace tcc {

  namespace algorithm {

    template< typename T >
    struct mean_traits {
      static T zero() {
        return 0;
      }
    };

    struct add {

      template< typename T >
      auto operator()( const T& first, const T& second ) {
        return first + second;
      }

    };

    template< typename T, typename AddFunction = add >
    struct mean {

      AddFunction m_function;

      mean( AddFunction function = AddFunction{} ): m_function( function ) {}

      template< typename Iterator, typename Sentinel >
      T operator()( Iterator first, Sentinel last ) {
        auto size = distance( first, last );
        auto init = mean_traits<T>::zero();
        init = std::accumulate( first, last, std::move( init ), m_function );
        return init / size;
      }

    };

  }  //namespace algorithm

}  //namespace tcc

#endif
