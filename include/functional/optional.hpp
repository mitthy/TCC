#ifndef TCC_FUNCTIONAL_OPTIONAL_HPP
#define TCC_FUNCTIONAL_OPTIONAL_HPP

//C stdlib includes
#include <stdint.h>

//C++ stdlib includes
#include <type_traits>

//Project includes
#include "meta/detect.hpp"
#include "meta/list.hpp"

namespace tcc {

  namespace functional {

    namespace __detail__ {

      struct __optional_impl__ {

      };

    }

    struct nullopt_t {} nullopt;

    template< typename T >
    struct optional final {

      //TODO
      T placeholder = T{};
      //

      optional() = default;

      optional( nullopt_t ) {}

      template< typename... ParamTypes,
        typename = std::enable_if_t< !( sizeof...( ParamTypes ) == 1 && std::is_same_v< std::decay_t< meta::head_t< ParamTypes... > >, optional > ) > >
      optional( ParamTypes&&... params ) {
        static_assert( meta::is_constructible_with_v< T, ParamTypes... > );

      }

      optional( const optional& other ) = default;

      optional( optional&& other ) = default;

      optional& operator=( const optional& other )  & = default;

      optional& operator=( optional&& other)  & = default;

      optional& operator=( const T& value )  & {
        //TODO
        return *this;
      }

      optional& operator=( T&& value )  & {
        //TODO
        return *this;
      }

      optional& operator=( nullopt_t ) & {
        //TODO
        return *this;
      }

      ~optional() {
        //TODO
      }

      operator bool() const {
        return false;
      }

      T& operator*() & {
        //TODO
        return placeholder;
      }

      const T& operator*() const & {
        //TODO
        return placeholder;
      }

      T&& operator*() && {
        return placeholder;
      }

      const T&& operator*() const && {
        return placeholder;
      }

      T* operator->() {
        return &placeholder;
      }

      const T* operator->() const {
        return &placeholder;
      }

    };

  }

}

#endif
