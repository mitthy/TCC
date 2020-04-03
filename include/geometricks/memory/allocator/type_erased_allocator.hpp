#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_TYPE_ERASED_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_TYPE_ERASED_ALLOCATOR_HPP

#include <type_traits>
#include "malloc_allocator.hpp"

namespace geometricks {

  namespace memory {

    struct type_erased_allocator final {

    private:

      using allocate_function_t = void*( void*, size_t );

      using align_allocate_function_t = void*( void*, size_t, size_t );

      using free_function_t = void( void*, void* );

      using align_free_function_t = void( void*, void*, size_t );

      using destructor_t = void( void* );

      struct v_table_t {

        allocate_function_t* m_alloc;

        align_allocate_function_t* m_align_alloc;

        free_function_t* m_free;

        align_free_function_t* m_align_free;

        destructor_t* m_destruct;

      } * m_table;

      void* m_allocator;

      template< typename T >
      static v_table_t* construct( T* obj ) {
        static v_table_t table {
          []( void* inner_obj, size_t size ) {  //Alloc
            return allocate( *static_cast<T*>( inner_obj ), size );
          },
          []( void* inner_obj, size_t size, size_t align ) {  //Align alloc
            return allocate( *static_cast<T*>( inner_obj ), size, align );
          },
          []( void* inner_obj, void* ptr ) {  //Free
            return deallocate( *static_cast<T*>( inner_obj ), ptr );
          },
          []( void* inner_obj, void* ptr, size_t align ) {  //Align free
            return deallocate( *static_cast<T*>( inner_obj ), ptr, align );
          },
          []( void* inner_obj ) {  //Destructor
            static_cast<T*>( inner_obj )->~T();
          }
        };
        return &table;
      }

    public:

      type_erased_allocator(): m_table( construct( &malloc_allocator ) ), m_allocator( &malloc_allocator ) {
      }

      ~type_erased_allocator() {
        m_table->m_destruct( m_allocator );
      }

      friend void* allocate( type_erased_allocator& allocator, size_t sz ) {
        return allocator.m_table->m_alloc( allocator.m_allocator, sz );
      }

      friend void* allocate( type_erased_allocator& allocator, size_t sz, size_t align ) {
        return allocator.m_table->m_align_alloc( allocator.m_allocator, sz, align );
      }

      friend void deallocate( type_erased_allocator& allocator, void* ptr ) {
        allocator.m_table->m_free( allocator.m_allocator, ptr );
      }

      friend void deallocate( type_erased_allocator& allocator, void* ptr, size_t align ) {
        allocator.m_table->m_align_free( allocator.m_allocator, ptr, align );
      }

    };

  }

}

#endif //GEOMETRICKS_MEMORY_ALLOCATOR_TYPE_ERASED_ALLOCATOR_HPP
