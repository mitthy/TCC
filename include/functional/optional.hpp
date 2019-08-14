/*
Copyright (c) 2019 Lucas Finger Roman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
