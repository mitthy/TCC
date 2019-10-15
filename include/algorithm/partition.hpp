#ifndef GEOMETRICKS_ALGORITHM_PARTITION_HPP
#define GEOMETRICKS_ALGORITHM_PARTITION_HPP

//C++ stdlib includes
#include <algorithm>

//Reimplementation of std partition where std iter_swap is a customisation point to work with custom iterators provided by the library.

namespace geometricks {

  namespace algorithm {

    template< typename InputIterator, typename Sentinel, typename Function >
    InputIterator partition( InputIterator first, Sentinel last, Function func ) {
      using std::iter_swap;
      while( first != last && func( *first ) ) {
        ++first;
      }
      if( first == last ) return first;
      for( auto next = std::next( first ); next != last; ++next ) {
        if( func( *next ) ) {
          iter_swap( first, next );
          ++first;
        }
      }
      return first;
    }

  }

}

#endif //GEOMETRICKS_ALGORITHM_PARTITION_HPP
