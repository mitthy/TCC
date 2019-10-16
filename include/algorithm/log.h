#ifndef GEOMETRICKS_ALGORITHM_LOG_H
#define GEOMETRICKS_ALGORITHM_LOG_H

/**
* @file
* @brief Provides constexpr log2 function to be used in templates.
*/

//C stdlib includes
#include <stdint.h>

namespace geometricks {

  namespace algorithm {

    namespace utils {

      /**
      * @brief Computes the log base 2 of a number in a constexpr way.
      * @param input The input number.
      * @return log base 2 of the input rounded down to an integer.
      */
      constexpr int8_t
      log2( uint64_t input ) {
        if( !input ) {
          return -1;
        }
        int8_t result = 0;
        while( input >>= 1 ) {
          ++result;
        }
        return result;
      }

    }  //namespace utils

  }  //namespace data_structure

}  //namespace geometricks



#endif //GEOMETRICKS_ALGORITHM_LOG_H
