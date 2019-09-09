#ifndef TCC_DATA_STRUCTURE_UTILS_LOG_H
#define TCC_DATA_STRUCTURE_UTILS_LOG_H

//C stdlib includes
#include <stdint.h>

namespace tcc {

  namespace data_structure {

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

}  //namespace tcc



#endif
