#ifndef GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_SMALL_VECTOR_HPP
#define GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_SMALL_VECTOR_HPP

#include <stdint.h>
#include <type_traits>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "utils.hpp"

namespace geometricks {

  namespace data_structure {

    template< typename T >
    class vector {

      struct stack_ptr_calculator {
        T* _1;
        int _2;
        unsigned _3;
        union small_element_t {

          struct dummy {} _;

          T data;

        };
        small_element_t _stack[1];
      };

      T* m_data;

      int m_size;

      unsigned m_capacity;

      T* get_stack_address() const {
        return static_cast<T*>( static_cast<void*>( &( ( stack_ptr_calculator* ) this )->_stack ) );
      }

      void
      destroy( T* begin, T* end ) {
        if constexpr( !std::is_trivially_destructible_v<T> ) {
          while( begin != end ) {
            begin->~T();
            ++begin;
          }
        }
      }

      bool
      is_stack() const {
        return m_data == get_stack_address();
      }

      void
      grow( unsigned new_capacity ) {
        if constexpr( std::is_trivially_copyable_v<T> ) {
          if( is_stack() ) {
            T* heap_mem = static_cast<T*>( malloc( sizeof( T ) * new_capacity ) );
            memcpy( heap_mem, m_data, m_capacity * sizeof( T ) );
            //No need to free the buffer since it was on the stack.
            //Also, safe to leave memory as is. Trivially copyable types don't need destructor calls.
            m_data = heap_mem;
            m_capacity = new_capacity;
          }
          else {
            T* new_mem = static_cast<T*>( realloc( m_data, sizeof( T ) * new_capacity ) );
            m_data = new_mem;
            m_capacity = new_capacity;
          }
        }
        else {
          T* new_mem = static_cast<T*>( malloc( sizeof( T ) * new_capacity ) );
          std::uninitialized_copy( std::make_move_iterator( m_data ), std::make_move_iterator( m_data + m_size ), new_mem );
          destroy( begin(), end() );
          free( m_data );
          m_data = new_mem;
        }
      }

    protected:

      vector( int capacity ): m_data( get_stack_address() ), m_size( 0 ), m_capacity( capacity ) {}

    public:

      using iterator = T*;

      using const_iterator = const iterator;

      template< typename... Args >
      void
      push_back( Args&&... args ) {
        if( m_size == m_capacity ) {
          grow( m_capacity * 2 );
        }
        new ( &m_data[ m_size++ ] ) T( std::forward<Args>( args )... );
      }

      ~vector() {
        destroy( begin(), end() );
        if( !is_stack() ) {
          free( m_data );
        }
      }

      iterator begin() {
        return m_data;
      }

      iterator end() {
        return m_data + m_size;
      }

      const_iterator begin() const {
        return m_data;
      }

      const_iterator end() const {
        return m_data + m_size;
      }

      iterator data() {
        return m_data;
      }

      const_iterator data() const {
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

      void set_size( int sz ) {
        //UNSAFE!! Only call this function if you're sure about the new size!!!!!!!
        //Should be used for [] or data() initialization!
        m_size = sz;
      }

      void
      reserve( unsigned request ) {
        if( m_capacity < request ) {
          unsigned next_capacity = __detail__::next_power_of_2( request );
          grow( next_capacity );
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

      small_vector(): vector<T>::vector( SBOSize ) {
      }

    };

  }

}

#endif //GEOMETRICKS_DATA_STRUCTURE_PRIMITIVES_SMALL_VECTOR_HPP
