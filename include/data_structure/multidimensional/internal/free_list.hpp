#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_INTERNAL_FREE_LIST_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_INTERNAL_FREE_LIST_HPP

//C stdlib includes
#include <stdlib.h>

//C++ stdlib includes
#include <algorithm>
#include <type_traits>

static constexpr int FREE_LIST_INITIAL_CAPACITY = 16;

namespace tcc {

  namespace data_structure {

    namespace __detail__ {

      template< typename POD >
      struct __free_list__ {

        static_assert( std::is_pod_v<POD>, "This free list structure is only to be used with Plain Old DataTypes." );

        //Constructor

        __free_list__():  m_capacity( FREE_LIST_INITIAL_CAPACITY ),
                              m_size( 0 ),
                              m_data( static_cast<data_t*>( malloc( m_capacity * sizeof( data_t ) ) ) ),
                              m_next_free( -1 ) {}

        //Copy constructors

        __free_list__( const __free_list__& rhs ):  m_capacity( rhs.m_capacity ),
                                                            m_size( rhs.m_size ),
                                                            m_data( static_cast<data_t*>( malloc( m_capacity * sizeof( data_t ) ) ) ),
                                                            m_next_free( rhs.m_next_free ) {
          std::copy( rhs.m_data, rhs.m_data + rhs.m_size, m_data );
        }

        __free_list__( __free_list__&& rhs ): m_capacity( rhs.m_capacity ),
                                                      m_size( rhs.m_size ),
                                                      m_data( rhs.m_data ),
                                                      m_next_free( rhs.m_next_free ) {
          rhs.m_data = nullptr;
        }

        //Copy assignment

        __free_list__&
        operator=( const __free_list__& rhs ) {
          if( &rhs != this ) {
            if( rhs.m_capacity > m_capacity ) {
              data_t* buff = static_cast<data_t*>( malloc( rhs.m_capacity * sizeof( int ) ) );
              free( m_data );
              m_data = buff;
              m_capacity = rhs.m_capacity;
            }
            std::copy( rhs.m_data, rhs.m_data + rhs.m_size, m_data );
            m_size = rhs.m_size;
            m_next_free = rhs.m_next_free;
          }
          return *this;
        }

        __free_list__&
        operator=( __free_list__&& rhs ) {
          if( &rhs != this ) {
            m_size = rhs.m_size;
            m_capacity = rhs.m_capacity;
            m_data = rhs.m_data;
            m_next_free = rhs.m_next_free;
            rhs.m_data = nullptr;
          }
          return *this;
        }

        //Destructor

        ~__free_list__() {
          free( m_data );
        }

        int
        push_back( const POD& value ) {
          if( m_next_free == -1 ) {
            if( m_size == m_capacity ) {
              int new_capacity = m_capacity * 2;
              data_t* buff = static_cast<data_t*>( malloc( new_capacity * sizeof( data_t ) ) );
              std::copy( m_data, m_data + m_capacity, buff );
              free( m_data );
              m_data = buff;
              m_capacity = new_capacity;
            }
            m_data[ m_size ].m_actual = value;
            return m_size++;
          }
          else {
            int temp = m_next_free;
            m_next_free = m_data[ m_next_free ].m_next;
            m_data[ temp ].m_actual = value;
            return temp;
          }
        }

        void
        remove( int pos ) {
          m_data[ pos ].m_next = m_next_free;
          m_next_free = pos;
        }

        int
        size() const {
          return m_size;
        }

        POD&
        operator[]( int pos ) {
          return m_data[ pos ].m_actual;
        }

        const POD&
        operator[]( int pos ) const {
          return m_data[ pos ].m_actual;
        }

        union data_t {
          POD m_actual;
          int m_next;
        };

        int m_capacity;

        int m_size;

        data_t* m_data;

        int m_next_free;

      };

    }

  }

}

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DETAIL_FREE_LIST_HPP
