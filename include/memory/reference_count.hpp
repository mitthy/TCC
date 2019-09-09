#ifndef TCC_MEMORY_REFERENCE_COUNT_HPP
#define TCC_MEMORY_REFERENCE_COUNT_HPP

//C stdlib includes
#include <stdint.h>

//Project includes
#include "allocator/allocator_base.hpp"

namespace tcc {

  namespace memory {

    template< typename T >
    struct reference_count {

      reference_count( allocator_base* alloc = &malloc_allocator ) {

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

#endif
