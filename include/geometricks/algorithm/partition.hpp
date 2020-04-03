#ifndef GEOMETRICKS_ALGORITHM_PARTITION_HPP
#define GEOMETRICKS_ALGORITHM_PARTITION_HPP

//C++ stdlib includes
#include <algorithm>
#include "iter_swap.hpp"

//Reimplementation of std partition where std iter_swap is a customisation point to work with custom iterators provided by the library.
/**
* @file
* Implements partition algorithm with a custom iter_swap predicate.
*/

namespace geometricks {

  namespace algorithm {

    /**
    * @brief Partitions a range
    * @param first First element of the range.
    * @param last Last element or sentinel value.
    * @param func Compare function.
    * @pre If Sentinel is an iterator, first < last. Else, eventually first != last compares false.
    * @details Partitions a range such that every element that compares false for func is to the left of everyone who compares true.
    */
    template< typename InputIterator, typename Sentinel, typename Function >
    InputIterator partition( InputIterator first, Sentinel last, Function func ) {
      while( first != last && func( *first ) ) {
        ++first;
      }
      if( first == last ) return first;
      for( auto next = std::next( first ); next != last; ++next ) {
        if( func( *next ) ) {
          geometricks::algorithm::iter_swap( first, next );
          ++first;
        }
      }
      return first;
    }

  }

}

#endif //GEOMETRICKS_ALGORITHM_PARTITION_HPP
