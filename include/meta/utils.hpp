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
