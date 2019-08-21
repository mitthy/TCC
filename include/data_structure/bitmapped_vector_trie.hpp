#ifndef TCC_DATA_STRUCTURE_BITMAPPED_VECTOR_TRIE_HPP
#define TCC_DATA_STRUCTURE_BITMAPPED_VECTOR_TRIE_HPP

//C stdlib includes
#include <stdint.h>

//C++ stdlib includes
#include <type_traits>

//Project includes
#include "utils/log.h"

namespace tcc {

  namespace data_structure {

    template< typename T, size_t NodeSize = 32 >
    struct bitmapped_vector_trie {

      //Size must be a power of 2 so that we can efficiently calculate the index.
      static_assert( ( ( NodeSize & ( NodeSize - 1 ) ) == 0 ) && ( NodeSize > 0 ), "Size must be a power of 2");

      //TODO: make this class able to use different allocators.

    private:

      static constexpr uint8_t MaskStep = utils::log2( NodeSize ) + 1;

      static constexpr size_t Mask = NodeSize - 1;

      static_assert( MaskStep > 0 );

      struct node {

      };

      //We use the rank as how many steps we want the incoming value to take before masking it out.
      //Each new level, we increase the rank by MaskStep
      uint64_t m_rank;

    };

  }

}

#endif
