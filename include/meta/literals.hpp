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

#ifndef TCC_META_LITERAL_HPP
#define TCC_META_LITERAL_HPP

//C stdlib includes
#include <stddef.h>

namespace tcc {
  namespace meta {

    namespace literals {

      template< typename T, size_t V >
      struct integer_literal {

        static constexpr T value = V;

        constexpr operator T () const {
          return V;
        }

      };

      namespace __detail__ {
        template<char Head, char... Tail>
        constexpr auto __make_integer_literal_impl__() {
          //TODO
          return integer_literal<int, 0>{};
        }
      }

      template< char... chars >
      constexpr auto operator""_i() {
        return __detail__::__make_integer_literal_impl__<chars...>();
      }

    }

  }

}

#endif
