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

#ifndef TCC_META_DETECT_HPP
#define TCC_META_DETECT_HPP

//C++ stdlib includes
#include <type_traits>

namespace tcc {

  namespace meta {

    namespace __detail__ {

      template< template< typename... >  typename Expression, typename AlwaysVoid = void, typename... Types >
      struct __is_valid_expression_impl__ : std::false_type {};

      template< template< typename... >  typename Expression, typename... Types >
      struct __is_valid_expression_impl__< Expression, std::void_t< Expression< Types... > >, Types... > : std::true_type { };

      template< typename T, typename... Params >
      using __counstruct_with__ = decltype( T( std::declval<Params>()... ) );

    }


    template< template< typename... > typename Expression, typename... Types >
    using is_valid_expression = __detail__::__is_valid_expression_impl__< Expression, void, Types... >;

    template< template< typename... > typename Expression, typename... Types >
    constexpr bool is_valid_expression_v = is_valid_expression< Expression, Types... >::value;

    template< typename T, typename... Params >
    using is_constructible_with = is_valid_expression< __detail__::__counstruct_with__, T, Params... >;

    template< typename T, typename... Params >
    constexpr bool is_constructible_with_v = is_constructible_with< T, Params... >::value;

  }

}

#endif
