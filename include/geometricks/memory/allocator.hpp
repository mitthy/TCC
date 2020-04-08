#ifndef GEOMETRICKS_MEMORY_ALLOCATOR_HPP
#define GEOMETRICKS_MEMORY_ALLOCATOR_HPP

#include <type_traits>
#include <cstddef>

#include "allocator/new_allocator.hpp"
#include "geometricks/meta/detect.hpp"
#include "geometricks/meta/utils.hpp"

/**
* @file
* @brief Implements the allocator concepts used for the data structures of this project.
*/

namespace geometricks {

  struct allocator;

  namespace memory {

    namespace allocator_customization {

      template< typename T >
      struct allocator;

    }

    /**
    * @cond EXCLUDE_DOXYGEN
    *
    * Internal not to be documented
    */
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
      auto __do_deallocate_size__( Allocator& allocator, void* ptr, size_t size, meta::priority_tag<0> ) -> decltype( dealloc( allocator, ptr, size ) ) {
        return dealloc( allocator, ptr, size );
      }

      template< typename Allocator >
      auto __do_deallocate_size__( Allocator& allocator, void* ptr, size_t size, meta::priority_tag<1> ) -> decltype( deallocate( allocator, ptr, size ) ) {
        return deallocate( allocator, ptr, size );
      }

      template< typename Allocator >
      auto __do_deallocate_size__( Allocator& allocator, void* ptr, size_t size, meta::priority_tag<2> ) -> decltype( allocator.dealloc( ptr, size ) ) {
        return allocator.dealloc( ptr, size );
      }

      template< typename Allocator >
      auto __do_deallocate_size__( Allocator& allocator, void* ptr, size_t size, meta::priority_tag<3> ) -> decltype( allocator.deallocate( ptr, size ) ) {
        return allocator.deallocate( ptr, size );
      }

      template< typename Allocator >
      auto __do_deallocate_size__( Allocator& allocator, void* ptr, size_t size, meta::priority_tag<4> ) -> decltype( allocator_customization::allocator<Allocator>::deallocate( ptr, size ) ) {
        return allocator_customization::allocator<Allocator>::deallocate( ptr, size );
      }

      template< typename T >
      using __try_deallocate_align__ = decltype( __do_deallocate_size__( std::declval<T&>(), std::declval<void*>(), std::declval<size_t>(), std::declval<meta::priority_tag<4>>() ) );

      template< typename T >
      constexpr bool __can_deallocate_size__ = meta::is_valid_expression_v<__try_deallocate_align__, T>;

      struct __test_deallocate_align__ {
        void dealloc( void*, size_t );
      };

      static_assert( __can_deallocate_size__<__test_deallocate_align__> );

      static_assert( !__can_deallocate_size__<__test_deallocate_unalign__> );

      template< typename T >
      constexpr bool __can_allocate__ = __can_allocate_align__<T> || __can_allocate_unalign__<T>;

      template< typename T >
      constexpr bool __can_deallocate__ = __can_deallocate_size__<T> || __can_deallocate_unalign__<T>;

      static_assert( __can_allocate__<new_allocator_t> );

      static_assert( __can_deallocate__<new_allocator_t> );

      constexpr auto __allocate__ = []( auto& alloc, size_t sz, size_t align ) {
        using alloc_t = std::decay_t<decltype(alloc)>;
        if constexpr( __can_allocate_align__<alloc_t> ) {
          return __do_allocate_aligned__( alloc, sz, align, meta::priority_tag<4>{} );
        }
        else {
          static_assert( __can_allocate_unalign__<alloc_t> );
          ( void ) align;
          return __do_allocate_unaligned__( alloc, sz, meta::priority_tag<4>{} );
        }
      };

      constexpr auto __deallocate__ = []( auto& alloc, void* ptr, size_t size ) {
        using alloc_t = std::decay_t<decltype(alloc)>;
        if constexpr( __can_deallocate_size__<alloc_t> ) {
          __do_deallocate_size__( alloc, ptr, size, meta::priority_tag<4>{} );
        }
        else {
          static_assert( __can_deallocate_unalign__<alloc_t> );
          ( void ) size;
          __do_deallocate_unaligned__( alloc, ptr, meta::priority_tag<4>{} );
        }
      };

    }
    /**
    * @endcond
    *
    * Internal not to be documented
    */

    template< typename T >
    constexpr bool is_allocator = __detail__::__can_allocate__<T> && __detail__::__can_deallocate__<T>;

    /**
    * @cond EXCLUDE_DOXYGEN
    *
    * Internal not to be documented
    */
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
          []( void* ptr, void* obj, size_t size ) {
            __detail__::__deallocate__( *static_cast<T*>( ptr ), obj, size );
          }
        };
        return &table;
      }

      static void* __default_allocator__ = &new_allocator;

      static __v_table_for_allocator__* __default_v_table__ = __make_v_table__<new_allocator_t>();

    }
    /**
    * @endcond
    */

    allocator get_default_allocator();

    template< typename Allocator >
    void set_default_allocator( Allocator& );

    void set_default_allocator( allocator& allocator );

  }

  /**
  * @brief Type erased view to an external allocator.
  * @details This class doesn't own the actual allocator. Instead, it is used as a view to an external allocator and stored in the data structures of this project.
  * This way, the allocator should ALWAYS have at least the same object lifetime as the data structure. Not doing so results in undefined behavior.
  * An allocator is defined as anything that can allocate raw bytes of memory with either of 2 signatures: it either allocates a size of memory or a size of aligned memory.
  * Example:
  * @code{.cpp}
    struct my_aligned_alloc {
      void* allocate( size_t size, size_t align );
    };
    struct my_alloc {
      void* allocate( size_t size );
    };
  * @endcode
  * In case both functions are supported, it always picks the aligned one.
  * For that, the type must either supply an allocate function or an alloc function or an allocate free function that takes a reference to the type as the first parameter
  * or an alloc free function that takes a reference to the type as the first parameter. If none are supplied, see geometricks::memory::allocator_customization::allocator.
  * Note that supplying an allocate function is not enough, as the type must also know how to deallocate memory for it to work.
  * For the deallocate functions, you can either supply a deallocate function taking a void* or a deallocate function taking a void* and a size_t.
  * Example:
  * @code{.cpp}
    struct my_complete_alloc {
      void* allocate( size_t size );
      void deallocate( void* );
    };
    struct my_complete_aligned_alloc_with_size {
      void* allocate( size_t size, size_t align );
      void deallocate( void* ptr, size_t size );
    };
  * @endcode
  * In case both functions are supported, it picks the size one.
  * The type must either supply a deallocate function or a dealloc function. If it doesn't, it can also work with deallocate or dealloc free functions taking a reference to
  * the type as the first parameter. If none are supplied, see geometricks::memory::allocator_customization::allocator.
  */
  struct allocator final {

    /**
    * @brief Default constructs an allocator view, using the default allocator as the allocator.
    * @see @ref geometricks::memory::set_default_allocator(Allocator&).
    * @see @ref geometricks::memory::get_default_allocator().
    */
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

    void deallocate( void* ptr, size_t size = 0 ) {
      m_table->__deallocate__( m_allocator, ptr, size );
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

  void deallocate( allocator& alloc, void* ptr, size_t size ) {
    alloc.deallocate( ptr, size );
  }

  namespace memory {

    /**
    * @brief Returns the default allocator.
    * @returns A view to the default allocator set by geometricks::memory::set_default_allocator(Allocator&). If none was set, returns the operator new allocator.
    * @details After a call to @ref geometricks::memory::set_default_allocator "set_default_allocator()", returns a geometricks::allocator view to the default allocator.
    * @note The default allocator at program startup is always set to operator new.
    * @see geometricks::memory::new_allocator_t.
    */
    allocator get_default_allocator() {
      return allocator{ __detail__::__default_allocator__, __detail__::__default_v_table__ };
    }

    /**
    * @brief TODO
    */
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
