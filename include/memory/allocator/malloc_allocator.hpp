#ifndef TCC_MEMORY_ALLOCATOR_MALLOC_ALLOCATOR_HPP
#define TCC_MEMORY_ALLOCATOR_MALLOC_ALLOCATOR_HPP

//C stdlib includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

namespace tcc {

  namespace memory {

    struct malloc_allocator_t final {} malloc_allocator;

    void* allocate( malloc_allocator_t&, size_t sz, size_t align ) {
      return malloc( sz );
    }

    void* allocate( malloc_allocator_t&, size_t sz ) {
      return malloc( sz );
    }

    void deallocate( malloc_allocator_t&, void* ptr, size_t align ) {
      free( ptr );
    }

    void deallocate( malloc_allocator_t&, void* ptr ) {
      free( ptr );
    }

  }

}

#endif
