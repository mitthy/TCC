#ifndef TCC_META_LIST_HPP
#define TCC_META_LIST_HPP

//C stdlib includes
#include <stddef.h>

//C++ stdlib includes
#include <type_traits>
#include <tuple>

namespace tcc {

  namespace meta {

    namespace __detail__ {

      template< typename... Ts >
      struct __head__;

      template< typename T, typename... Ts >
      struct __head__< T, Ts... > {
        using type = T;
      };

      template< typename T >
      struct __head__< T > {
        using type = T;
      };

      template< size_t Index, typename... Types >
      struct __get__ {

        static_assert( Index < sizeof...( Types ) );
        using type = decltype( std::get< Index >( std::declval< std::tuple<Types...> >() ) );

      };

    }

    template< size_t Index, typename... Types >
    using get = typename __detail__::__get__< Index, Types... >::type;

    template< typename... List >
    using head_t = typename __detail__::__head__< List... >::type;

  }

}

#endif
