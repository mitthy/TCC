#ifndef GEOMETRICKS_ALGORITHM_ABSOLUTE_DIFFERENCE_HPP
#define GEOMETRICKS_ALGORITHM_ABSOLUTE_DIFFERENCE_HPP

//C stdlib includes
#include <stdint.h>
#include <cmath>

//C++ stdlib includes
#include <algorithm>
#include <type_traits>

//Project includes
#include "geometricks/meta/utils.hpp"

/**
* @file
* @brief Provides abs and absolute difference functions along with customization options used within the library.
*/

namespace geometricks {

  namespace algorithm {

    namespace abs_customization {

      /**@brief
      Struct providing meta function for calling abs on a type supplied by the user.
      @tparam T Type to generate custom abs function.
      @details To use this function, specialize it for your type and provide a static method named _ that takes a variable of your type.
      @warning A definition of this struct for a type shadows all other possible invocations for abs on that type.
      @see abs_t

      Example:
      @code{.cpp}
      namespace dummy {
        struct dummy{
          int m_value;
        };
      }
      namespace geometricks {
        namespace algorithm {
          namespace abs_customization {
            template<>
            struct abs<dummy::dummy> {
              static int _( const dummy& dum ) {
                return std::abs( dum.m_value );
              }
            };
          }
        }
      }
      @endcode
      */
      template< typename T >
      struct abs;

    }

    /**
    * @cond EXCLUDE_DOXYGEN
    *
    * Internal not to be documented
    */
    namespace __detail__ {

      template< typename T >
      using is_unsigned = std::enable_if_t<std::is_unsigned_v<T>, bool>;

      template< typename T >
      using is_signed = std::enable_if_t<std::is_signed_v<T>, bool>;

      using std::abs;

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<0> ) -> decltype( value < 0 ? -value : value ) {
        return value < 0 ? -value : value;
      }

      template< typename T, is_unsigned<T> = true >
      constexpr size_t
      __abs__( const T& value, meta::priority_tag<1> ) {
        return value;
      }

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<2> ) -> decltype( static_cast<size_t>( abs( value ) ) ) {
        return static_cast<size_t>( abs( value ) );
      }

      template< typename T >
      constexpr auto
      __abs__( const T& value, meta::priority_tag<3> ) -> decltype( abs_customization::abs<T>::_( value ) ) {
        return abs_customization::abs<T>::_( value );
      }

    }
    /**
    * @endcond
    */

    /**
    @brief Functor that computes the abs of a number

    Functor for abs that provides automatic ADL for the user.
    */
    struct abs_t {

      /**
      @brief Calculates the abs of a number.
      @param value Number to calculate the absolute value.
      @return The absolute value.
      @details The abs specification goes as follows:
        -# If a specialization of abs_customization::abs is provided for that type, the specialization _ function is called.
        -# If an abs function is found via ADL for that type, the abs function is called.
        -# If the type is unsigned, it is returned as is.
        -# If a < operator for the type exists the < operator compares to an int and a - unary operator for that type exists, it returns @code{.cpp} value < 0 ? -value : value; @endcode

      * If all fails, the call is ill-formed.
      */
      template< typename T >
      constexpr auto
      operator()( const T& value ) const -> decltype( __detail__::__abs__( value, meta::priority_tag<3>{} ) ) {
        return __detail__::__abs__( value, meta::priority_tag<3>{} );
      }

    };

    /**
    @brief Function object for @relatealso abs_t
    */
    constexpr auto
    abs = abs_t{};

    namespace absolute_difference_customization {

      /**@brief
      Struct providing meta function for calling the absolute difference on a type supplied by the user.
      @tparam T Type to generate custom abs function.
      @details To use this function, specialize it for your type and provide a static method named _ that takes 2 variables of your type.
      @warning A definition of this struct for a type shadows all other possible invocations for absolute difference on that type.
      @see absolute_difference_t

      Example:
      @code{.cpp}
      namespace dummy {
        struct dummy{
          int m_value;
        };
      }
      namespace geometricks {
        namespace algorithm {
          namespace absolute_difference_customization {
            template<>
            struct absolute_difference<dummy::dummy> {
              static dummy _( const dummy& lhs, const dummy& rhs ) {
                return std::abs( lhs.m_value - rhs.m_value );
              }
            };
          }
        }
      }
      @endcode
      */
      template< typename T >
      struct absolute_difference;

    }

    /**
    * @cond EXCLUDE_DOXYGEN
    *
    * Internal not to be documented
    */
    namespace __detail__ {

      template< typename T >
      using is_signed = std::enable_if_t<std::is_signed_v<T>, bool>;

      template< typename T >
      using is_unsigned = std::enable_if_t<std::is_unsigned_v<T>, bool>;

      template< typename T >
      using is_string = std::enable_if_t<std::is_same_v<T, std::string>, bool>;

      template< typename T >
      constexpr size_t
      __absolute_difference__( T first, T second, meta::priority_tag<0> ) {
        return algorithm::abs( first - second );
      }

      template< typename T, is_unsigned<T> = true >
      constexpr size_t
      __absolute_difference__( T first, T second, meta::priority_tag<1> ) {
        return first > second ? first - second : second - first;
      }

      template< typename T, is_string<T> = true >
      constexpr size_t
      __absolute_difference__( T first, T second, meta::priority_tag<2> ) {
        return 0; //TODO
      }

      template< typename T >
      constexpr auto
      __absolute_difference__( T first, T second, meta::priority_tag<3> ) -> decltype( absolute_difference_customization::absolute_difference<T>::_( first, second ) ) {
        return absolute_difference_customization::absolute_difference<T>::_( first, second );
      }

    }  //namespace __detail__
    /**
    * @endcond
    */

    /**
    @brief Functor that computes the absolute difference of 2 number

    Functor for absolute difference that provides automatic ADL for the user.
    */
    struct absolute_difference_t {

      /**
      * @brief Computes the absolute difference of 2 numbers.
      * @tparam T Type of the first argument. Should have same decay type as U.
      * @tparam U Type of the second argument. Should have save decay type as T.
      * @param first First of the 2 numbers to calculate absolute difference.
      * @param second Second of the 2 numbers to calculate absolute difference.
      * @details The absolute difference of 2 numbers is the positive difference between those 2 numbers.
      * That is, absolute_difference( a, b ) >= 0.
      * \n To get this effect, some techniques can be employed in order:
      * -# If a specialization of absolute_difference_customization::absolute_difference is provided for that type, the specialization _ function is called.
      * -# If the type is a string, we get the edit distance.
      * -# If the type is unsigned, we get the greater of the 2 numbers and subtract the lesses number.
      * -# If it is possible to call abs on the number and there is an operator -, we call abs on the difference.
      *
      * If all fails, the call is ill-formed.
      * @todo Implement string edit distance.
      */
      template< typename T, typename U >
      constexpr auto operator()( T&& first, U&& second ) const ->
      decltype( __detail__::__absolute_difference__( std::forward<T>( first ), std::forward<T>( second ), meta::priority_tag<3>{} ) ) {
        static_assert( std::is_same_v<std::decay_t<T>, std::decay_t<U>> );
        return __detail__::__absolute_difference__( std::forward<T>( first ), std::forward<T>( second ), meta::priority_tag<3>{} );
      }

    };

    /**
    @brief Function object for @relatealso absolute_difference_t
    */
    constexpr auto
    absolute_difference = absolute_difference_t{};

  } //namespace algorithm

} //namespace geometricks

#endif //GEOMETRICKS_ALGORITHM_ABSOLUTE_DIFFERENCE_HPP
