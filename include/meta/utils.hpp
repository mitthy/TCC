#ifndef GEOMETRICKS_META_UTILS_HPP
#define GEOMETRICKS_META_UTILS_HPP

#include "detect.hpp"

namespace geometricks {

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

    template< typename T >
    constexpr bool always_false = false;

    template< typename T >
    struct numeric_limits : std::numeric_limits< T > {};

    template< int I >
    struct priority_tag : public priority_tag<I - 1> {};

    template<>
    struct priority_tag<0> {};

    template< typename T >
    struct insert_tag;

    struct push_back {};

    struct insert {};

    template< typename T >
    using push_back_expr = decltype( std::declval<T>().push_back( std::declval<typename T::value_type>() ) );

    template< typename T >
    using insert_expr = decltype( std::declval<T>().insert( std::declval<typename T::value_type>() ) );

    template< typename T >
    constexpr bool has_push_back = is_valid_expression_v<push_back_expr, T>;

    template< typename T >
    constexpr bool has_insert = is_valid_expression_v<insert_expr, T>;

    static_assert( has_push_back<std::vector<int>> );

    template< typename T,
              typename Container >
    void add_element( T&& element, Container& cont ) {
      if constexpr( has_push_back<Container> ) {
        cont.push_back( std::forward<T>( element ) );
      }
      else if constexpr( has_insert<Container> ) {
        cont.insert( std::forward<T>( element ) );
      }
      else {
        static_assert( geometricks::meta::always_false<T> );
      }
    }

  }

}

#endif //GEOMETRICKS_META_UTILS_HPP
