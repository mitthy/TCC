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

#ifndef TCC_META_UTILS_HPP
#define TCC_META_UTILS_HPP

namespace tcc {

  namespace meta {

    //Helper metafunction that wraps a result in a named type type.
    //Using this metafunction is preferred over typing manually to avoid potential typing errors.
    template< typename T >
    struct result_is {

      using type = T;

    };

    namespace __detail__ {

      template< typename T >
      struct __print_impl__ {
        using ReceivedType = typename T::__FORCECOMPILATIONERROR__;
      };

    }

    //Function to print a type at compile time
    //We make it raise an error to abort compilation and get useful information.
    template< typename T >
    using print = typename __detail__::__print_impl__< T >::ReceivedType;

  }

}

#endif
