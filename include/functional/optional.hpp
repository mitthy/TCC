#ifndef TCC_FUNCTIONAL_OPTIONAL_HPP
#define TCC_FUNCTIONAL_OPTIONAL_HPP

//C stdlib includes
#include <stdint.h>

//C++ stdlib includes
#include <type_traits>

//Project includes
#include "meta/detect.hpp"
#include "meta/list.hpp"
#include "meta/conditional_constructors.hpp"

namespace tcc {

  namespace functional {

    namespace __detail__ {

      template< typename T >
      static constexpr bool
      __enable_trivial_copy__ =   std::is_trivially_copy_assignable_v< T > &&
                                  std::is_trivially_copy_constructible_v< T > &&
                                  std::is_trivially_destructible_v< T >;

      template< typename T >
      static constexpr bool
      __enable_trivial_move__ =   std::is_trivially_move_assignable_v< T > &&
                                  std::is_trivially_move_constructible_v< T > &&
                                  std::is_trivially_destructible_v< T >;

      template< typename T >
      static constexpr bool
      __enable_trivial_destr__ =  std::is_trivially_destructible_v< T >;

      template< typename T >
      struct __optional_storage__ {

        std::aligned_storage_t<sizeof(T), alignof(T)> m_data;

        bool m_valid;

        static constexpr void
        __copy_construct__(__optional_storage__<T>& self, const __optional_storage__<T>& other) {
          self.m_valid = other.m_valid;
          if( other.m_valid ) {
            new ( &self.m_data ) T( *reinterpret_cast<const T*>( &other.m_data ) );
          }
        }

        static constexpr void
        __copy_assign__(__optional_storage__<T>& self, const __optional_storage__<T>& other) {
          if( self.m_valid && other.m_valid ) {
            *reinterpret_cast<T*>( &self.m_data ) = *reinterpret_cast<const T*>( &other.m_data );
          }
          else {
            if( other.m_valid ) {
              new ( &self.m_data ) T( *reinterpret_cast<const T*>( &other.m_data ) );
              self.m_valid = true;
            }
            else {
              if( self.m_valid ) {
                *reinterpret_cast<T*>( &self.m_data ).~T();
                self.m_valid = false;
              }
            }
          }
        }

        static constexpr void
        __move_construct__(__optional_storage__<T>& self, __optional_storage__<T>&& other) {
          self.m_valid = other.m_valid;
          if( other.m_valid ) {
            new ( &self.m_data ) T( std::move( *reinterpret_cast<T*>( &other.m_data ) ) );
          }
        }

        static constexpr void
        __move_assign__(__optional_storage__<T>& self, __optional_storage__<T>&& other) {
          if( self.m_valid && other.m_valid ) {
            *reinterpret_cast<T*>( &self.m_data ) = std::move( *reinterpret_cast<T*>( &other.m_data ) );
          }
          else {
            if( other.m_valid ) {
              new ( &self.m_data ) T( std::move( *reinterpret_cast<T*>( &other.m_data ) ) );
              self.m_valid = true;
            }
            else {
              if( self.m_valid ) {
                *reinterpret_cast<T*>( &self.m_data ).~T();
                self.m_valid = false;
              }
            }
          }
        }

      };

      template< template<typename, bool...> typename Down, typename T>
      struct __optional_base__ {
        constexpr Down<T>&
        self() {
          return static_cast<Down<T>&>(*this);
        }

        constexpr const
        Down<T>& self() const {
          return static_cast<const Down<T>&>(*this);
        }

        constexpr T&
        __get__() {
          return *reinterpret_cast< T* >( &self().m_storage.m_data );
        }

        constexpr const T&
        __get__() const {
          return *reinterpret_cast< const T* >( &self().m_storage.m_data );
        }

        constexpr bool
        __valid__() const {
          return self().m_storage.m_valid;
        }

        constexpr void __reset__() {
          if constexpr( !std::is_trivially_destructible_v< T > ) {
            if( self().m_storage.m_valid ) {
              __get__().~T();
            }
          }
        }

        constexpr void
        __reset_if__() {
          __reset__();
          self().m_storage.m_valid = false;
        }

        constexpr void
        __move_into__( T&& obj ) {
          if( self().m_storage.m_valid ) {
            __get__() = std::move( obj );
          }
          else {
            new ( &__get__() ) T( std::move( obj ) );
            self().m_storage.m_valid = true;
          }
        }

        constexpr void
        __set_invalid__() {
          self().m_storage.m_valid = false;
        }

        constexpr void
        __set_valid__() {
          self().m_storage.m_valid = true;
        }

        constexpr void
        __copy_into__( const T& obj ) {
          if( self().m_storage.m_valid ) {
            __get__() = obj;
          }
          else {
            new ( &__get__() ) T( obj );
            self().m_storage.m_valid = true;
          }
        }

      };

      template< typename T, bool = __enable_trivial_copy__<T>, bool = __enable_trivial_move__<T>, bool = __enable_trivial_destr__<T> >
      struct __optional_impl__ : public __optional_base__<__optional_impl__, T>{

        __optional_storage__< T > m_storage;

      };

      template< typename T >
      struct __optional_impl__<T,false,true,true> : public __optional_base__<__optional_impl__, T>{

        __optional_storage__<T> m_storage;

        constexpr __optional_impl__() = default;

        constexpr __optional_impl__(const __optional_impl__& other) {
          __optional_storage__<T>::__copy_construct__(m_storage, other.m_storage);
        }

        constexpr __optional_impl__&
        operator=(const __optional_impl__& other) {
          __optional_storage__<T>::__copy_assign__(m_storage, other.m_storage);
          return *this;
        }

        constexpr __optional_impl__( __optional_impl__&& ) = default;

        constexpr __optional_impl__&
        operator=( __optional_impl__&& ) = default;

        ~__optional_impl__() = default;


      };

      template< typename T >
      struct __optional_impl__<T,true,false,true> : public __optional_base__<__optional_impl__, T>{

        __optional_storage__< T > m_storage;

        constexpr __optional_impl__() = default;

        constexpr __optional_impl__( const __optional_impl__& ) = default;

        constexpr __optional_impl__&
        operator=( const __optional_impl__& ) = default;

        constexpr __optional_impl__( __optional_impl__&& other ) {
          __optional_storage__<T>::__move_construct__( m_storage, std::move( other.m_storage ) );
        }

        constexpr __optional_impl__&
        operator=( __optional_impl__&& other) {
          __optional_storage__<T>::__move_assign__( m_storage, std::move( other.m_storage ) );
          return *this;
        }

      };

      template< typename T >
      struct __optional_impl__<T,false,false,true> : public __optional_base__<__optional_impl__, T>{

        __optional_storage__< T > m_storage;

        constexpr __optional_impl__() = default;

        constexpr __optional_impl__( const __optional_impl__& other ) {
          __optional_storage__<T>::__copy_construct__( m_storage, other.m_storage );
        }

        constexpr __optional_impl__&
        operator=( const __optional_impl__& other ) {
          __optional_storage__<T>::__copy_assign__( m_storage, other.m_storage );
          return *this;
        }

        constexpr __optional_impl__( __optional_impl__&& other ) {
          __optional_storage__<T>::__move_construct__( m_storage, std::move( other.m_storage ) );
        }

        constexpr __optional_impl__&
        operator=( __optional_impl__&& other) {
          __optional_storage__<T>::__move_assign__( m_storage, std::move( other.m_storage ) );
          return *this;
        }

        ~__optional_impl__() = default;

      };

      template< typename T, bool any, bool any2 >
      struct __optional_impl__<T,any,any2,false> : public __optional_base__<__optional_impl__, T> {

        __optional_storage__< T > m_storage;

        constexpr __optional_impl__() = default;

        constexpr __optional_impl__( const __optional_impl__& other ) {
          __optional_storage__<T>::__copy_construct__( m_storage, other.m_storage );
        }

        constexpr __optional_impl__& operator=( const __optional_impl__& other ) {
          __optional_storage__<T>::__copy_assign__( m_storage, other.m_storage );
          return *this;
        }

        constexpr __optional_impl__( __optional_impl__&& other ) {
          __optional_storage__<T>::__move_construct__( m_storage, std::move( other.m_storage ) );
        }

        constexpr __optional_impl__& operator=( __optional_impl__&& other) {
          __optional_storage__<T>::__move_assign__( m_storage, std::move( other.m_storage ) );
          return *this;
        }

        ~__optional_impl__() {
          this->__reset_if__();
        }

      };

    }

    struct nullopt_t {} nullopt;

    template< typename T >
    struct optional final : private __detail__::__optional_impl__<T> {
    private:

      //TODO
      //Change interface to not return the underlying value, but instead accept functions that should match both cases (value|!value)
      //Document the class
      //Make it copyable/non copyable on the right conditions (currently, it just triggers a compile time error...)
      //Implement monads
      //Allow constexpr (using unions: see https://akrzemi1.wordpress.com/2012/12/13/constexpr-unions/)
      //Noexcept specifications

      using super = __detail__::__optional_impl__< T >;

    public:

      constexpr optional() {
        super::__set_invalid__();
      }

      constexpr optional( nullopt_t ) {
        super::__set_invalid__();
      }

      template< typename... ParamTypes,
        typename = std::enable_if_t< !( sizeof...( ParamTypes ) == 1 && std::is_same_v< std::decay_t< meta::head_t< ParamTypes... > >, optional > ) > >
      constexpr optional( ParamTypes&&... params ) {
        static_assert( meta::is_constructible_with_v< T, ParamTypes... > );
        new ( &super::__get__() ) T( std::forward<ParamTypes>(params)... );
        super::__set_valid__();
      }

      constexpr optional&
      operator=( const T& value )  & {
        super::__copy_into__( value );
        return *this;
      }

      constexpr optional&
      operator=( T&& value )  & {
        super::__move_into__( std::move( value ) );
        return *this;
      }

      constexpr optional&
      operator=( nullopt_t ) & {
        super::__reset_if__();
        return *this;
      }

      constexpr
      operator bool() const {
        return super::__valid__();
      }

      constexpr T&
      operator*() & {
        return super::__get__();
      }

      constexpr const T&
      operator*() const & {
        return super::__get__();
      }

      constexpr T&&
      operator*() && {
        return std::move( super::__get__() );
      }

      constexpr const T&&
      operator*() const && {
        return std::move( super::__get__() );
      }

      constexpr T*
      operator->() {
        return &super::__get__();
      }

      constexpr const T*
      operator->() const {
        return &super::__get__();
      }

    };

  }

}

#endif
