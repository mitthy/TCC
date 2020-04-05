#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_NEW_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_NEW_ALLOCATOR_HPP

#include <new>

namespace geometricks {

  namespace memory {

    struct new_allocator_t final {} new_allocator;

    void*
    allocate( new_allocator_t&, size_t sz, size_t align ) {
      return ::operator new( sz, std::align_val_t{ align } );
    }

    void*
    allocate( new_allocator_t&, size_t sz ) {
      return ::operator new( sz );
    }

    void
    deallocate( new_allocator_t&, void* ptr, size_t align ) {
      ::operator delete( ptr, std::align_val_t{ align } );
    }

    void
    deallocate( new_allocator_t&, void* ptr ) {
      ::operator delete( ptr );
    }

  }

}

#endif
