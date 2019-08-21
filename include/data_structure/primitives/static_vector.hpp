#ifndef TCC_DATA_STRUCTURE_STATIC_VECTOR_HPP
#define TCC_DATA_STRUCTURE_STATIC_VECTOR_HPP

//C stdlib includes
#include <stdint.h>

//C++ stdlib includes
#include <type_traits>
#include <memory>

namespace tcc {

  namespace data_structure {

    namespace __detail__ {

      template<typename T>
      void __init_mem_with_list__(std::initializer_list<T> args, T* data) {
        std::uninitialized_copy( std::begin( args ), std::end( args ), data );
      }

      template< typename Sub, typename T, size_t Size >
      struct __safe_insert_op__ {

        template<typename... Args>
        int push_back( Args&&... args ) {
          static_assert( sizeof...( args ) <= Size, "Can't push back more objects than maximum size!" );
          static_assert( (std::is_same_v<T, Args> && ...), "Args and T should be the same type." );
          auto& self = *static_cast<Sub*>( this );
          if( self.size() + sizeof...( args ) > Size ) {
            return -1;
          }
          __init_mem_with_list__( { std::forward<Args>(args)... }, self.m_end );
          self.m_end += sizeof...( args );
          return 0;
        }

      };

      template< typename Sub, typename T, size_t Size >
      struct __unsafe_insert_op__ {

        template<typename... Args>
        void push_back( Args&&... args ) {
          static_assert( sizeof...( args ) <= Size, "Can't push back more objects than maximum size!" );
          static_assert( (std::is_same_v<T, Args> && ...), "Args and T should be the same type." );
          auto& self = *static_cast<Sub*>( this );
          __init_mem_with_list__( { std::forward<Args>(args)... }, self.m_end );
          self.m_end += sizeof...( args );
        }
        
      };

    }



    //We can't use std array cause type may not be default initialised...
    template< typename T, size_t Size, template< typename, typename, size_t > class OpPolicy >
    struct static_vector final : public OpPolicy<static_vector<T, Size, OpPolicy>, T, Size> {

      static_assert( Size > 0 );

private:

      friend class OpPolicy<static_vector<T, Size, OpPolicy>, T, Size>;

      std::aligned_storage_t<sizeof(T), alignof(T)> m_data[ Size ];

      T* m_end;

public:

      static_vector(): m_data(), m_end(reinterpret_cast<T*>(&m_data[0])) {}

      constexpr bool is_empty() const {
        return reinterpret_cast<const T*>( &m_data[ 0 ] ) == m_end;
      }

      constexpr size_t size() const {
        return m_end - reinterpret_cast<const T*>( &m_data[ 0 ] );
      }

      T& operator[]( size_t pos ) {
        return *reinterpret_cast<T*>( &m_data[ pos ] );
      }

      const T& operator[]( size_t pos ) const {
        return *reinterpret_cast<T*>( &m_data[ pos ] );
      }

    };

    template< typename T, size_t Size>
    using safe_static_vector = static_vector<T, Size, __detail__::__safe_insert_op__>;

    template< typename T, size_t Size>
    using unsafe_static_vector = static_vector<T, Size, __detail__::__unsafe_insert_op__>;

  }

}

#endif
