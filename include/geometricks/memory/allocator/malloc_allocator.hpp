#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_MALLOC_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_MALLOC_ALLOCATOR_HPP

//C stdlib includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

namespace geometricks {

  namespace memory {

    struct malloc_allocator_t final {} malloc_allocator;

    void*
    allocate( malloc_allocator_t&, size_t sz ) {
      return malloc( sz );
    }

    void
    deallocate( malloc_allocator_t&, void* ptr ) {
      free( ptr );
    }

  }

}

#endif //GEOMETRICKS_MEMORY_ALLOCATOR_MALLOC_ALLOCATOR_HPP
