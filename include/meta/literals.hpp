#ifndef GEOMETRICKS_META_LITERAL_HPP
#define GEOMETRICKS_META_LIST_HPP

//C stdlib includes
#include <stddef.h>

namespace geometricks {
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

#endif //GEOMETRICKS_META_LIST_HPP
