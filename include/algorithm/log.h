#ifndef GEOMETRICKS_ALGORITHM_LOG_H
#define GEOMETRICKS_ALGORITHM_LOG_H

//C stdlib includes
#include <stdint.h>

namespace geometricks {

  namespace algorithm {

    namespace utils {

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
