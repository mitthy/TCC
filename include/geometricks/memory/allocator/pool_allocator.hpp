#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_POOL_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_POOL_ALLOCATOR_HPP

#include <type_traits>
#include <tuple>
#include <cstddef>

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

      void* allocate( size_t sz ) {
        ( void ) sz;
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
    struct multipool_allocator;  //This class is super awkward to use so far with node based strucutres that you can't know the node size without inspecting it beforehand.

    template< int BlockSize, int... Sz, int... Align >
    class multipool_allocator<BlockSize, size_align_pair<Sz, Align>...> {

      std::tuple<pool_allocator<Sz, BlockSize, Align>...> m_allocators;

    public:

      void* allocate( size_t size, size_t align = alignof( std::max_align_t ) ) {
        return __allocate_impl__( size, align, std::make_index_sequence<sizeof...(Sz)>() );
      }

      void deallocate( void* ptr, size_t size ) {
        __deallocate_impl__( ptr, size, std::make_index_sequence<sizeof...(Sz)>() );
      }

    private:

      template< size_t... Is >
      void*
      __allocate_impl__( size_t size, size_t align, std::index_sequence<Is...> ) {
        void* ret = nullptr;
        ( __allocate_impl_helper__( size, align, std::get<Is>( m_allocators ), &ret ), ... );
        return ret;
      }

      template< int Sz_, int Align_ >
      void
      __allocate_impl_helper__( size_t size, size_t , pool_allocator<Sz_, BlockSize, Align_>& pool, void** ptr ) {
        if( size == Sz_ ) {
          *ptr = pool.allocate( size );
        }
      }

      template< size_t... Is >
      void
      __deallocate_impl__( void* ptr, size_t size, std::index_sequence<Is...> ) {
        ( __deallocate_impl_helper__( ptr, size, std::get<Is>( m_allocators ) ), ... );
      }

      template< int Sz_, int Align_ >
      void
      __deallocate_impl_helper__( void* ptr, size_t size, pool_allocator<Sz_, BlockSize, Align_>& pool ) {
        if( size == Sz_ ) {
          pool.deallocate( ptr );
        }
      }

    };

    template< int BlockSize, typename... Ts >
    class multipool_allocator<BlockSize, const type_list<type_t<Ts>...>>: public multipool_allocator<BlockSize, Ts...> {

    };

    template< int BlockSize, typename... Ts >
    auto make_multipool_allocator( type_t<Ts>... ) {
      constexpr type_list<> list{};
      constexpr auto appended = append_unique( list, type<size_align_pair<sizeof(Ts), alignof( std::max_align_t )>>... );
      return multipool_allocator<BlockSize, decltype( appended )>{};
    }

  }

}

#endif
