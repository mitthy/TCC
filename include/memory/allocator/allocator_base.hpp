#ifndef TCC_MEMORY_ALLOCATOR_ALLOCATOR_BASE_HPP
#define TCC_MEMORY_ALLOCATOR_ALLOCATOR_BASE_HPP

//C stdlib includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

namespace tcc {

  namespace memory {

    struct allocator_base {

      void*
      allocate( size_t sz, size_t align = alignof( max_align_t ) ) {
        return do_allocate( sz, align );
      }

      void
      deallocate( void* block ) {
        return do_deallocate( block );
      }

      virtual ~allocator_base() {}

    protected:

      virtual void*
      do_allocate( size_t sz, size_t align ) = 0;

      virtual void
      do_deallocate( void* block ) = 0;

    };

    struct malloc_allocator final : public allocator_base {

      virtual ~malloc_allocator() {} //Noop

    protected:

      virtual void*
      do_allocate( size_t sz, size_t align ) override {
        return malloc( sz );
      }

      virtual void
      do_deallocate( void* block ) override {
        free( block );
      }

    } malloc_allocator;

  }

}

#endif
