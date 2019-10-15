#ifndef GEOMETRICKS_ALGORITHM_MEAN_HPP
#define GEOMETRICKS_ALGORITHM_MEAN_HPP

//C++ stdlib includes
#include <numeric>

//C++ stdlib includes
#include <algorithm>

namespace geometricks {

  namespace algorithm {

    template< typename T >
    struct mean_traits {
      static T zero() {
        return 0;
      }
    };

    struct add {

      template< typename T, typename U >
      auto operator()( const T& first, const U& second ) const {
        return first + second;
      }

    };

    template< typename T, typename AddFunction = add >
    struct mean {

      AddFunction m_function;

      mean( AddFunction function = AddFunction{} ): m_function( function ) {}

      template< typename Iterator, typename Sentinel >
      T operator()( Iterator first, Sentinel last ) const {
        auto size = std::distance( first, last );
        auto init = mean_traits<T>::zero();
        init = std::accumulate( first, last, std::move( init ), m_function );
        return init / size;
      }

    };

  }  //namespace algorithm

}  //namespace geometricks

#endif //GEOMETRICKS_ALGORITHM_MEAN_HPP
