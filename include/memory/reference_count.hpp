#ifndef GEOMETRICKS_MEMORY_REFERENCE_COUNT_HPP
#define GEOMETRICKS_MEMORY_REFERENCE_COUNT_HPP

//C stdlib includes
#include <stdint.h>

//Project includes
#include "allocator.hpp"

namespace geometricks {

  namespace memory {

    template< typename T, typename Allocator = malloc_allocator_t >
    struct reference_count {

      reference_count( Allocator* alloc = &malloc_allocator ) {

      }

      reference_count( const reference_count& rhs ) {

      }

      reference_count&
      operator=( const reference_count& rhs ) {
        return *this;
      }

      reference_count( reference_count&& rhs ) {

      }

      reference_count&
      operator=( reference_count&& rhs ) {

      }

      ~reference_count() {

      }

    private:

      size_t* m_count;

    };

  }

}

#endif //GEOMETRICKS_MEMORY_REFERENCE_COUNT_HPP
