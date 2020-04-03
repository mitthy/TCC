#ifndef GEOMETRICKS_META_CONDITIONAL_CONSTRUCTORS_HPP
#define GEOMETRICKS_META_CONDITIONAL_CONSTRUCTORS_HPP

//TODO: See if this is all working...

namespace geometricks {

  namespace meta {

      namespace __detail__ {

        template< bool >
        struct __disable_copy_constructor__ {

          __disable_copy_constructor__() = default;

          __disable_copy_constructor__(const __disable_copy_constructor__&) = delete;

          __disable_copy_constructor__( __disable_copy_constructor__&& ) = default;

          __disable_copy_constructor__& operator=( const __disable_copy_constructor__& ) = default;

          __disable_copy_constructor__& operator=( __disable_copy_constructor__&& ) = default;

          ~__disable_copy_constructor__() = default;

        };

        template<>
        struct __disable_copy_constructor__<false> {};

        template< bool >
        struct __disable_copy_assign__ {

          __disable_copy_assign__() = default;

          __disable_copy_assign__( const __disable_copy_assign__& ) = default;

          __disable_copy_assign__( __disable_copy_assign__&& ) = default;

          __disable_copy_assign__& operator=( const __disable_copy_assign__& ) = delete;

          __disable_copy_assign__& operator=( __disable_copy_assign__&& ) = default;

          ~__disable_copy_assign__() = default;

        };

        template<>
        struct __disable_copy_assign__<false> {};

        template< bool >
        struct __disable_move_constructor__ {
          __disable_move_constructor__() = default;

          __disable_move_constructor__( const __disable_move_constructor__& ) = default;

          __disable_move_constructor__( __disable_move_constructor__&& ) = delete;

          __disable_move_constructor__& operator=( const __disable_move_constructor__& ) = default;

          __disable_move_constructor__& operator=( __disable_move_constructor__&& ) = default;

          ~__disable_move_constructor__() = default;
        };

        template<>
        struct __disable_move_constructor__<false> {};

        template< bool >
        struct __disable_move_assign__ {
          __disable_move_assign__() = default;

          __disable_move_assign__( const __disable_move_assign__& ) = default;

          __disable_move_assign__( __disable_move_assign__&& ) = default;

          __disable_move_assign__& operator=( const __disable_move_assign__& ) = default;

          __disable_move_assign__& operator=( __disable_move_assign__&& ) = delete;

          ~__disable_move_assign__() = default;
        };

      }

      template< bool Cond >
      using disable_copy_constructor_if = __detail__::__disable_copy_constructor__<Cond>;

      template< bool Cond >
      using disable_copy_assign_if = __detail__::__disable_copy_assign__<Cond>;

      template< bool Cond >
      using disable_move_constructor_if = __detail__::__disable_move_constructor__<Cond>;

      template< bool Cond >
      using disable_move_assign_if = __detail__::__disable_move_assign__<Cond>;

  }

}

#endif //GEOMETRICKS_META_CONDITIONAL_CONSTRUCTORS_HPP
