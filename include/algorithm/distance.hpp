#ifndef TCC_ALGORITHM_INTERNAL_DISTANCE_HPP
#define TCC_ALGORITHM_INTERNAL_DISTANCE_HPP

//C stdlib includes
#include <stddef.h>

//C++ stdlib includes
#include <iterator>

namespace tcc {

  namespace algorithm {

    //We reimplement distance here to allow comparisons on a sentinel value.
    //std library requires distance algorithm to match same type for begin and end.

    namespace __detail__ {

      template< typename Iterator, typename Sentinel >
      ptrdiff_t distance( Iterator first, Sentinel last, std::random_access_iterator_tag ) {
        return last - first;
      }

      template< typename Iterator, typename Sentinel >
      ptrdiff_t distance( Iterator first, Sentinel last, std::forward_iterator_tag ) {
        ptrdiff_t result = 0;
        while( first != last ) {
          ++first;
          ++result;
        }
        return result;
      }

    }

    constexpr auto distance = []( auto first, auto last ) {
      return __detail__::distance( first, last, typename std::iterator_traits<decltype( first )>::iterator_category{} );
    };

  }

}

#endif //TCC_ALGORITHM_INTERNAL_DISTANCE_HPP
