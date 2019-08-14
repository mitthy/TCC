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
