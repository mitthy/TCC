#ifndef GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_INTERNAL_UTILS_HPP
#define GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_INTERNAL_UTILS_HPP

#include <stdint.h>

namespace geometricks {

  namespace __detail__ {

    uint64_t next_power_of_2( uint64_t input ) {
      --input;
      input |= input >> 1;
      input |= input >> 2;
      input |= input >> 4;
      input |= input >> 8;
      input |= input >> 16;
      input |= input >> 32;
      ++input;
      return input;
    }

    int64_t next_power_of_2( int64_t input ) {
      if( input < 0 ) return input;
      union signed_unsigned {
        int64_t sig;
        uint64_t unsig;
      };
      signed_unsigned tmp = { input };
      tmp.unsig = next_power_of_2( tmp.unsig );
      return tmp.sig;
    }

    uint32_t next_power_of_2( uint32_t input ) {
      --input;
      input |= input >> 1;
      input |= input >> 2;
      input |= input >> 4;
      input |= input >> 8;
      input |= input >> 16;
      ++input;
      return input;
    }

    int32_t next_power_of_2( int32_t input ) {
      if( input < 0 ) return input;
      union signed_unsigned {
        int32_t sig;
        uint32_t unsig;
      };
      signed_unsigned tmp = { input };
      tmp.unsig = next_power_of_2( tmp.unsig );
      return tmp.sig;
    }

    uint16_t next_power_of_2( uint16_t input ) {
      --input;
      input |= input >> 1;
      input |= input >> 2;
      input |= input >> 4;
      input |= input >> 8;
      ++input;
      return input;
    }

    int16_t next_power_of_2( int16_t input ) {
      if( input < 0 ) return input;
      union signed_unsigned {
        int16_t sig;
        uint16_t unsig;
      };
      signed_unsigned tmp = { input };
      tmp.unsig = next_power_of_2( tmp.unsig );
      return tmp.sig;
    }

    uint8_t next_power_of_2( uint8_t input ) {
      --input;
      input |= input >> 1;
      input |= input >> 2;
      input |= input >> 4;
      ++input;
      return input;
    }

    int8_t next_power_of_2( int8_t input ) {
      if( input < 0 ) return input;
      union signed_unsigned {
        int8_t sig;
        uint8_t unsig;
      };
      signed_unsigned tmp = { input };
      tmp.unsig = next_power_of_2( tmp.unsig );
      return tmp.sig;
    }

  }

}

#endif
