#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_POOL_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_POOL_ALLOCATOR_HPP

#include <type_traits>
#include <tuple>

namespace geometricks {

  namespace memory {

    template< int PoolElementSize, int Size = 512, int Align = alignof( max_align_t ) >
    class pool_allocator {

      struct free_list_element {
        free_list_element* next;
      };

      union free_t {
        free_list_element* first_free;
        std::aligned_storage_t<PoolElementSize, Align> _; //Force size and alignment.
      };

      struct block {

        block* next_block = nullptr;

        std::aligned_storage_t<sizeof( free_t ), alignof( free_t )> data[ Size ];

        block() {
          free_t* first = ( free_t* )&data;
          free_t* last = first + ( Size - 1 );
          while( first != last ) {
            first->first_free = ( free_list_element* )( first + 1 );
            ++first;
          }
          first->first_free = nullptr;
        }

      };

      block* first_block;

      free_t data;

    public:

      pool_allocator(): first_block( new block{} ), data{ static_cast<free_list_element*>( static_cast<void*>( &first_block->data ) ) } {
      }

      ~pool_allocator() {
        while( first_block ) {
          block* to_delete = first_block;
          first_block = first_block->next_block;
          delete to_delete;
        }
      }

      void* allocate() {
        if( data.first_free != nullptr ) {
          void* ret = (void*) data.first_free;
          data.first_free = data.first_free->next;
          return ret;
        }
        else {
          block* next_first_block = new block();
          next_first_block->next_block = first_block;
          first_block = next_first_block;
          data.first_free = static_cast<free_list_element*>( static_cast<void*>( &first_block->data ) );
          void* ret = (void*) data.first_free;
          data.first_free = data.first_free->next;
          return ret;
        }
      }

      void deallocate( void* ptr ) {
        free_list_element* insert_ptr = ( free_list_element* )ptr;
        insert_ptr->next = data.first_free;
        data.first_free = insert_ptr;
      }

    };

    template< int Sz, int Align >
    struct size_align_pair {
      static constexpr int size = Sz;
      static constexpr int align = Align;
    };

    template< typename T >
    struct get_pool_allocator;

    template< int Sz, int Align >
    struct get_pool_allocator<size_align_pair<Sz, Align>> {

    };

    template< typename T >
    struct type_t {

    };

    template< typename T >
    constexpr auto type = type_t<T>{};

    template< typename... >
    struct type_list {};

    template< typename T, typename... Ts >
    constexpr bool is_in( type_t<T>, type_list< type_t<Ts>... > ) {
      return ( std::is_same_v<T, Ts> || ... );
    }

    template< typename T, typename... Ts >
    constexpr auto append_unique_single( type_list< type_t<Ts>... > list, type_t<T> new_type ) {
      if constexpr( is_in( new_type, list ) ) {
        return list;
      }
      else {
        return type_list< type_t<T>, type_t<Ts>... >{};
      }
    }

    template< typename T, typename... Ts, typename... Tss >
    constexpr auto append_unique( type_list< type_t<Tss>... > list, type_t<T> head, type_t<Ts>... tail ) {
      constexpr auto appended = append_unique_single( list, head );
      return append_unique( appended, tail... );
    }

    template< typename... T >
    constexpr auto append_unique( type_list< type_t<T>... > list ) {
      return list;
    }

    template< int BlockSize, typename... T >
    struct multipool_allocator;

    template< int BlockSize, int... Sz, int... Align >
    class multipool_allocator<BlockSize, size_align_pair<Sz, Align>...> {

      std::tuple<pool_allocator<Sz, BlockSize, Align>...> m_allocators;

    public:

      template< typename T >
      void* allocate() {
        auto& actual = std::get<pool_allocator<sizeof(T), BlockSize, alignof(T)>>( m_allocators );
        return actual.allocate();
      }

      template< typename T >
      void deallocate( T* ptr ) {
        auto& actual = std::get<pool_allocator<sizeof(T), BlockSize, alignof(T)>>( m_allocators );
        return actual.deallocate( (void*)ptr );
      }

    };

    template< int BlockSize, typename... Ts >
    class multipool_allocator<BlockSize, const type_list<type_t<Ts>...>>: public multipool_allocator<BlockSize, Ts...> {

    };

    template< int BlockSize, typename... Ts >
    auto make_multipool_allocator( type_t<Ts>... types ) {
      constexpr type_list<> list{};
      constexpr auto appended = append_unique( list, type<size_align_pair<sizeof(Ts), alignof(Ts)>>... );
      return multipool_allocator<BlockSize, decltype( appended )>{};
    }

  }

}

#endif
