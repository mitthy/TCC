#ifndef GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_SMALL_VECTOR_HPP
#define GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_SMALL_VECTOR_HPP

#include <stdint.h>
#include <type_traits>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "utils.hpp"
#include "geometricks/memory/allocator.hpp"

namespace geometricks {

  template< typename T >
  struct vector {

    vector() = delete;

    using iterator = T*;

    using const_iterator = const T*;

    using value_type = T;

    using reference = T&;

    using const_reference = const T&;

    using size_type = int;

    void
    push_back( const T& element ) {
      if( m_size == ( int )m_capacity ) {
        __grow__( m_capacity * 2 );
      }
      if constexpr( std::is_trivially_copyable_v<T> ) {
        memcpy( ( void* )&m_data[ m_size ], ( void* )&element, sizeof(T) );
      }
      else {
        new ( &m_data[ m_size ] ) T( element );
      }
      ++m_size;
    }

    void
    pop_back() {
      m_data[ --m_size ].~T();
    }

    ~vector() {
      __destroy__( begin(), end() );
      if( !__is_stack__() ) {
        m_allocator.deallocate( m_data );
      }
    }

    iterator
    erase( const_iterator pos ) {
      pos->~T();
      iterator old = ( iterator )pos++;
      std::move( pos, static_cast<const_iterator>( end() ), old );
      --m_size;
      return old;
    }

    iterator
    erase( const_iterator begin, const_iterator last ) {
      iterator old = ( iterator )begin;
      int distance = last - begin;
      __destroy__( begin, last );
      std::move( last, static_cast<const_iterator>( end() ), old );
      m_size -= distance;
      return old;
    }

    iterator
    begin() {
      return m_data;
    }

    iterator
    end() {
      return m_data + m_size;
    }

    const_iterator
    begin() const {
      return m_data;
    }

    const_iterator
    end() const {
      return m_data + m_size;
    }

    iterator
    data() {
      return m_data;
    }

    const_iterator
    data() const {
      return m_data;
    }

    T&
    operator[]( int index ) {
      return m_data[ index ];
    }

    const T&
    operator[]( int index ) const {
      return m_data[ index ];
    }

    void
    set_size( int sz ) {
      //UNSAFE!! Only call this function if you're sure about the new size!!!!!!!
      //Should be used for [] or data() initialization!
      m_size = sz;
    }

    int
    size() const {
      return m_size;
    }

    void
    reserve( unsigned request ) {
      if( m_capacity < request ) {
        unsigned next_capacity = __detail__::next_power_of_2( request );
        __grow__( next_capacity );
      }
    }

    T&
    front() {
      return *m_data;
    }

    const T&
    front() const {
      return *m_data;
    }

    T&
    back() {
      return m_data[ m_size - 1 ];
    }

    const T&
    back() const {
      return m_data[ m_size - 1 ];
    }

    bool
    empty() const {
      return !m_size;
    }

  protected:

    vector( int capacity, geometricks::allocator alloc ): m_data( __get_stack_address__() ), m_size( 0 ), m_capacity( capacity ), m_allocator( alloc ) {}

  private:

    struct __stack_ptr_calculator__ {
      T* _1;
      int _2;
      unsigned _3;
      geometricks::allocator _4;
      union small_element_t {

        struct dummy {} _;

        T data;

      };
      small_element_t _stack[1];
    };

    T* m_data;

    int m_size;

    unsigned m_capacity;

    geometricks::allocator m_allocator;

    T*
    __get_stack_address__() const {
      return static_cast<T*>( static_cast<void*>( &( ( __stack_ptr_calculator__* ) this )->_stack ) );
    }

    void
    __destroy__( const T* begin, const T* end ) {
      if constexpr( !std::is_trivially_destructible_v<T> ) {
        while( begin != end ) {
          begin->~T();
          ++begin;
        }
      }
      else {
        ( void )begin;
        ( void )end;
      }
    }

    bool
    __is_stack__() const {
      return m_data == __get_stack_address__();
    }

    void
    __grow__( unsigned new_capacity ) {
      if constexpr( std::is_trivially_copyable_v<T> ) {
        if( __is_stack__() ) {
          T* heap_mem = static_cast<T*>( m_allocator.allocate( sizeof( T ) * new_capacity ) );
          memcpy( heap_mem, m_data, m_capacity * sizeof( T ) );
          //No need to free the buffer since it was on the stack.
          //Also, safe to leave memory as is. Trivially copyable types don't need destructor calls.
          m_data = heap_mem;
          m_capacity = new_capacity;
        }
        else {
          T* new_mem = static_cast<T*>( m_allocator.reallocate( m_data, sizeof( T ) * new_capacity ) );
          m_data = new_mem;
          m_capacity = new_capacity;
        }
      }
      else {
        T* new_mem = static_cast<T*>( m_allocator.allocate( sizeof( T ) * new_capacity ) );
        std::uninitialized_copy( std::make_move_iterator( m_data ), std::make_move_iterator( m_data + m_size ), new_mem );
        __destroy__( begin(), end() );
        m_allocator.deallocate( m_data );
        m_data = new_mem;
      }
    }

  };

  template< typename T, int SBOSize >
  class small_vector : public vector<T> {

    static_assert( SBOSize > 0 );

    union small_element_t {

      struct dummy {} _;

      T data;

      small_element_t() {
        _ = dummy{};
      }

    };

    struct small_t {

      small_element_t m_data[ SBOSize ];

    };

    small_t m_stack;

  public:

    small_vector( geometricks::allocator alloc = geometricks::allocator{} ): vector<T>::vector( SBOSize, alloc ) {
    }

  };

}

#endif //GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_SMALL_VECTOR_HPP
