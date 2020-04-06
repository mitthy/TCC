#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_HPP

#include <type_traits>
#include <cstddef>

#include "allocator/new_allocator.hpp"
#include "geometricks/meta/detect.hpp"
#include "geometricks/meta/utils.hpp"

namespace geometricks {

  struct allocator;

  namespace memory {

    namespace allocator_customization {

      template< typename T >
      struct allocator;

    }

    namespace __detail__ {

      template< typename Allocator >
      auto __do_allocate_unaligned__( Allocator& allocator, size_t sz, geometricks::meta::priority_tag<0> ) -> decltype( alloc( allocator, sz ) ) {
        return alloc( allocator, sz );
      }

      template< typename Allocator >
      auto __do_allocate_unaligned__( Allocator& alloc, size_t sz, geometricks::meta::priority_tag<1> ) -> decltype( allocate( alloc, sz ) ) {
        return allocate( alloc, sz );
      }

      template< typename Allocator >
      auto __do_allocate_unaligned__( Allocator& alloc, size_t sz, geometricks::meta::priority_tag<2> ) -> decltype( alloc.alloc( sz ) ) {
        return alloc.alloc( sz );
      }

      template< typename Allocator >
      auto __do_allocate_unaligned__( Allocator& alloc, size_t sz, geometricks::meta::priority_tag<3> ) -> decltype( alloc.allocate( sz ) ) {
        return alloc.allocate( sz );
      }

      template< typename Allocator >
      auto __do_allocate_unaligned__( Allocator& alloc, size_t sz, geometricks::meta::priority_tag<4> ) -> decltype( allocator_customization::allocator<Allocator>::allocate( sz ) ) {
        return allocator_customization::allocator<Allocator>::allocate( sz );
      }

      template< typename T >
      using __try_allocate_unalign__ = decltype( __do_allocate_unaligned__( std::declval<T&>(), std::declval<size_t>(), std::declval<meta::priority_tag<4>>() ) );

      template< typename T >
      constexpr bool __can_allocate_unalign__ = meta::is_valid_expression_v<__try_allocate_unalign__, T>;

      struct __test_allocate_unalign__ {
        void* alloc( size_t );
      };

      static_assert( __can_allocate_unalign__<__test_allocate_unalign__> );

      static_assert( !__can_allocate_unalign__<int> );

      template< typename Allocator >
      auto __do_allocate_aligned__( Allocator& allocator, size_t sz, size_t align, meta::priority_tag<0> ) -> decltype( alloc( allocator, sz, align ) ) {
        return alloc( allocator, sz, align );
      }

      template< typename Allocator >
      auto __do_allocate_aligned__( Allocator& alloc, size_t sz, size_t align, meta::priority_tag<1> ) -> decltype( allocate( alloc, sz, align ) ) {
        return allocate( alloc, sz, align );
      }

      template< typename Allocator >
      auto __do_allocate_aligned__( Allocator& alloc, size_t sz, size_t align, meta::priority_tag<2> ) -> decltype( alloc.alloc( sz, align ) ) {
        return alloc.alloc( sz, align );
      }

      template< typename Allocator >
      auto __do_allocate_aligned__( Allocator& alloc, size_t sz, size_t align, meta::priority_tag<3> ) -> decltype( alloc.allocate( sz, align ) ) {
        return alloc.allocate( sz, align );
      }

      template< typename Allocator >
      auto __do_allocate_aligned__( Allocator& alloc, size_t sz, size_t align, meta::priority_tag<4> ) -> decltype( allocator_customization::allocator<Allocator>::allocate( sz, align ) ) {
        return allocator_customization::allocator<Allocator>::allocate( sz, align );
      }

      template< typename T >
      using __try_allocate_align__ = decltype( __do_allocate_aligned__( std::declval<T&>(), std::declval<size_t>(), std::declval<size_t>(), std::declval<meta::priority_tag<4>>() ) );

      template< typename T >
      constexpr bool __can_allocate_align__ = meta::is_valid_expression_v<__try_allocate_align__, T>;

      struct __test_allocate_align__ {
        void* alloc( size_t, size_t );
      };

      static_assert( __can_allocate_align__<__test_allocate_align__> );

      static_assert( !__can_allocate_align__<__test_allocate_unalign__> );

      template< typename Allocator >
      auto __do_deallocate_unaligned__( Allocator& allocator, void* ptr, meta::priority_tag<0> ) -> decltype( dealloc( allocator, ptr ) ) {
        return dealloc( allocator, ptr );
      }

      template< typename Allocator >
      auto __do_deallocate_unaligned__( Allocator& allocator, void* ptr, meta::priority_tag<1> ) -> decltype( deallocate( allocator, ptr ) ) {
        return deallocate( allocator, ptr );
      }

      template< typename Allocator >
      auto __do_deallocate_unaligned__( Allocator& allocator, void* ptr, meta::priority_tag<2> ) -> decltype( allocator.dealloc( ptr ) ) {
        return allocator.dealloc( ptr );
      }

      template< typename Allocator >
      auto __do_deallocate_unaligned__( Allocator& allocator, void* ptr, meta::priority_tag<3> ) -> decltype( allocator.deallocate( ptr ) ) {
        return allocator.deallocate( ptr );
      }

      template< typename Allocator >
      auto __do_deallocate_unaligned__( Allocator& allocator, void* ptr, meta::priority_tag<4> ) -> decltype( allocator_customization::allocator<Allocator>::deallocate( ptr ) ) {
        return allocator_customization::allocator<Allocator>::deallocate( ptr );
      }

      template< typename T >
      using __try_deallocate_unalign__ = decltype( __do_deallocate_unaligned__( std::declval<T&>(), std::declval<void*>(), std::declval<meta::priority_tag<4>>() ) );

      template< typename T >
      constexpr bool __can_deallocate_unalign__ = meta::is_valid_expression_v<__try_deallocate_unalign__, T>;

      struct __test_deallocate_unalign__ {
        void dealloc( void* );
      };

      static_assert( __can_deallocate_unalign__<__test_deallocate_unalign__> );

      static_assert( !__can_deallocate_unalign__<int> );

      template< typename Allocator >
      auto __do_deallocate_aligned__( Allocator& allocator, void* ptr, size_t align, meta::priority_tag<0> ) -> decltype( dealloc( allocator, ptr, align ) ) {
        return dealloc( allocator, ptr, align );
      }

      template< typename Allocator >
      auto __do_deallocate_aligned__( Allocator& allocator, void* ptr, size_t align, meta::priority_tag<1> ) -> decltype( deallocate( allocator, ptr, align ) ) {
        return deallocate( allocator, ptr, align );
      }

      template< typename Allocator >
      auto __do_deallocate_aligned__( Allocator& allocator, void* ptr, size_t align, meta::priority_tag<2> ) -> decltype( allocator.dealloc( ptr, align ) ) {
        return allocator.dealloc( ptr, align );
      }

      template< typename Allocator >
      auto __do_deallocate_aligned__( Allocator& allocator, void* ptr, size_t align, meta::priority_tag<3> ) -> decltype( allocator.deallocate( ptr, align ) ) {
        return allocator.deallocate( ptr, align );
      }

      template< typename Allocator >
      auto __do_deallocate_aligned__( Allocator& allocator, void* ptr, size_t align, meta::priority_tag<4> ) -> decltype( allocator_customization::allocator<Allocator>::deallocate( ptr, align ) ) {
        return allocator_customization::allocator<Allocator>::deallocate( ptr, align );
      }

      template< typename T >
      using __try_deallocate_align__ = decltype( __do_deallocate_aligned__( std::declval<T&>(), std::declval<void*>(), std::declval<size_t>(), std::declval<meta::priority_tag<4>>() ) );

      template< typename T >
      constexpr bool __can_deallocate_align__ = meta::is_valid_expression_v<__try_deallocate_align__, T>;

      struct __test_deallocate_align__ {
        void dealloc( void*, size_t );
      };

      static_assert( __can_deallocate_align__<__test_deallocate_align__> );

      static_assert( !__can_deallocate_align__<__test_deallocate_unalign__> );

      template< typename T >
      constexpr bool __can_allocate__ = __can_allocate_align__<T> || __can_allocate_unalign__<T>;

      template< typename T >
      constexpr bool __can_deallocate__ = __can_deallocate_align__<T> || __can_deallocate_unalign__<T>;

      static_assert( __can_allocate__<new_allocator_t> );

      static_assert( __can_deallocate__<new_allocator_t> );

      constexpr auto __allocate__ = []( auto& alloc, size_t sz, size_t align ) {
        using alloc_t = std::decay_t<decltype(alloc)>;
        if constexpr( __can_allocate_align__<alloc_t> ) {
          //If we can allocate aligned regions of memory, check for presence of an aligned delete. If there is none, compile error.
          static_assert( __can_deallocate_align__<alloc_t> );
          return __do_allocate_aligned__( alloc, sz, align, meta::priority_tag<4>{} );
        }
        else {
          static_assert( __can_allocate_unalign__<alloc_t> );
          ( void ) align;
          return __do_allocate_unaligned__( alloc, sz, meta::priority_tag<4>{} );
        }
      };

      constexpr auto __deallocate__ = []( auto& alloc, void* ptr, size_t align ) {
        using alloc_t = std::decay_t<decltype(alloc)>;
        if constexpr( __can_deallocate_align__<alloc_t> ) {
          //If we can deallocate aligned regions of memory, check for presence of an aligned new. If there is none, compile error.
          static_assert( __can_allocate_align__<alloc_t> );
          __do_deallocate_aligned__( alloc, ptr, align, meta::priority_tag<4>{} );
        }
        else {
          static_assert( __can_deallocate_unalign__<alloc_t> );
          ( void ) align;
          __do_deallocate_unaligned__( alloc, ptr, meta::priority_tag<4>{} );
        }
      };

    }

    template< typename T >
    constexpr bool is_allocator = __detail__::__can_allocate__<T> && __detail__::__can_deallocate__<T>;

    namespace __detail__ {

      struct __v_table_for_allocator__ final {

        using __allocate_t__    = void*( void*, size_t, size_t );

        using __deallocate_t__  = void( void*, void*, size_t );

        __allocate_t__* __allocate__;

        __deallocate_t__* __deallocate__;

      };

      template< typename T >
      __v_table_for_allocator__* __make_v_table__() noexcept {

        static __v_table_for_allocator__ table = {
          []( void* ptr, size_t sz, size_t align ) {
            return __detail__::__allocate__( *static_cast<T*>( ptr ), sz, align );
          },
          []( void* ptr, void* obj, size_t align ) {
            __detail__::__deallocate__( *static_cast<T*>( ptr ), obj, align );
          }
        };
        return &table;
      }

      static void* __default_allocator__ = &new_allocator;

      static __v_table_for_allocator__* __default_v_table__ = __make_v_table__<new_allocator_t>();

    }

    allocator get_default_allocator();

    template< typename Allocator >
    void set_default_allocator( Allocator* );

    void set_default_allocator( allocator& allocator );

  }

  struct allocator final {

    allocator(): m_allocator( memory::__detail__::__default_allocator__ ), m_table( memory::__detail__::__default_v_table__ ) {
    }

    template< typename Allocator, typename Void = std::enable_if_t< !std::is_same_v< std::decay_t<Allocator>, allocator > > >
    allocator( Allocator& alloc ): m_allocator( ( void* ) &alloc ), m_table( memory::__detail__::__make_v_table__<Allocator>() ) {
      static_assert( memory::is_allocator<Allocator> );
    }

    allocator( const allocator& other ): m_allocator( other.m_allocator ), m_table( other.m_table ) {
    }

    friend allocator memory::get_default_allocator();

    friend void memory::set_default_allocator( allocator& allocator );

    void* allocate( size_t sz, size_t align ) {
      return m_table->__allocate__( m_allocator, sz, align );
    }

    void* allocate( size_t sz ) {
      return m_table->__allocate__( m_allocator, sz, alignof( std::max_align_t ) );
    }

    void deallocate( void* ptr, size_t align ) {
      m_table->__deallocate__( m_allocator, ptr, align );
    }

    void deallocate( void* ptr ) {
      m_table->__deallocate__( m_allocator, ptr, alignof( std::max_align_t ) );
    }

    template< typename Allocator >
    bool operator==( const Allocator& alloc ) const {
      //TODO: pass it to the correct type, maybe?
      return m_allocator == ( void* )( &alloc );
    }

    bool operator==( const allocator& other ) const {
      return m_allocator == other.m_allocator;
    }

  private:

    allocator( void* ptr, memory::__detail__::__v_table_for_allocator__* table ): m_allocator( ptr ), m_table( table ) {
    }

    void* m_allocator;

    memory::__detail__::__v_table_for_allocator__* m_table;

  };

  void* allocate( allocator& alloc, size_t sz, size_t align ) {
    return alloc.allocate( sz, align );
  }

  void* allocate( allocator& alloc, size_t sz ) {
    return alloc.allocate( sz );
  }

  void deallocate( allocator& alloc, void* ptr ) {
    alloc.deallocate( ptr );
  }

  void deallocate( allocator& alloc, void* ptr, size_t align ) {
    alloc.deallocate( ptr, align );
  }

  namespace memory {

    allocator get_default_allocator() {
      return allocator{ __detail__::__default_allocator__, __detail__::__default_v_table__ };
    }

    template< typename Allocator >
    void set_default_allocator( Allocator& allocator ) {
      static_assert( is_allocator<Allocator> );
      __detail__::__default_allocator__ = ( void* ) &allocator;
      __detail__::__default_v_table__ = __detail__::__make_v_table__<Allocator>();
    }

    void set_default_allocator( allocator& allocator ) {
      __detail__::__default_allocator__ = allocator.m_allocator;
      __detail__::__default_v_table__ = allocator.m_table;
    }

  }

}

#endif //GEOMETRICKS_MEMORY_ALLOCATOR_HPP
