#ifndef GEOMETRICKS_ALGORITHM_ITER_SWAP_HPP
#define GEOMETRICKS_ALGORITHM_ITER_SWAP_HPP

//Project includes
#include "geometricks/meta/utils.hpp"
#include <type_traits>

/**
* @file
* @brief Provides definitions for iter_swap function.
*/

namespace geometricks {

  namespace algorithm {

    namespace iter_swap_customization {

      /**@brief
      Struct providing meta function for calling iter_swap on a type supplied by the user.
      @tparam T Type to generate custom iter_swap function.
      @details To use this function, specialize it for your type and provide a static method named _ that takes a variable of your type.
      @warning A definition of this struct for a type shadows all other possible invocations for iter_swap on that type.
      @see iter_swap_t

      Example:
      @code{.cpp}
      namespace dummy {
        struct dummy_it{
          int* m_value;
        };
      }
      namespace geometricks {
        namespace algorithm {
          namespace iter_swap_customization {
            template<>
            struct iter_swap<dummy::dummy> {
              static constexpr void _( dummy left, dummy right ) {
                std::swap( *left.m_value, *right.m_value );
              }
            };
          }
        }
      }
      @endcode
      */
      template< typename T >
      struct iter_swap;

    }

    /**
    * @cond EXCLUDE_DOXYGEN
    *
    * Internal not to be documented
    */
    namespace __detail__ {

      using std::iter_swap;

      using std::swap;

      template< typename Iter1, typename Iter2 >
      constexpr auto
      __iter_swap__( Iter1 left, Iter2 right, meta::priority_tag<0> ) -> std::void_t<decltype( swap( *left, *right ) )> {
        swap( *left, *right );
      }

      template< typename Iter1, typename Iter2 >
      constexpr auto
      __iter_swap__( Iter1 left, Iter2 right, meta::priority_tag<1> ) -> std::void_t<decltype( iter_swap( left, right ) )> {
        iter_swap( left, right );
      }

      template< typename Iter1, typename Iter2 >
      constexpr auto
      __iter_swap__( Iter1 left, Iter2 right, meta::priority_tag<2> ) -> std::void_t<decltype( iter_swap_customization::iter_swap<Iter2>::_( left, right ) )> {
        iter_swap_customization::iter_swap<Iter2>::_( left, right );
      }

      template< typename Iter1, typename Iter2 >
      constexpr auto
      __iter_swap__( Iter1 left, Iter2 right, meta::priority_tag<3> ) -> std::void_t<decltype( iter_swap_customization::iter_swap<Iter1>::_( left, right ) )> {
        iter_swap_customization::iter_swap<Iter1>::_( left, right );
      }

    }
    /**
    * @endcond
    *
    */

    /**
    @brief Functor that swaps 2 iterators.
    *
    * Functor for iter_swap that provides automatic ADL for the user.
    */
    struct iter_swap_t {

      /**
      * @brief Swaps the value held by the iterators.
      * @tparam Iter1 The type of the left iterator.
      * @tparam Iter2 The type of the right iterator.
      * @param left First of the 2 iterators.
      * @param right Last of the 2 iterators.
      * @details To swap the values, it tries different things in order:
      *  -# It checks for a template specialization of the iter_swap struct for Iter1.
      *  -# It checks for a template specialization of the iter_swap struct for Iter2.
      *  -# It tries to call iter swap via ADL.
      *  -# It tries calling swap via ADL accessing the underlying elements with the * unary operator.
      *
      * If all fails, the call is ill-formed.
      */
      template< typename Iter1, typename Iter2 >
      constexpr auto operator()( Iter1 left, Iter2 right ) const -> std::void_t<decltype(__detail__::__iter_swap__( left, right , meta::priority_tag<2>{} ) )> {
        __detail__::__iter_swap__( left, right, meta::priority_tag<2>{} );
      }

    };

    /**
    @brief Function object for @relatealso iter_swap_t
    */
    constexpr auto iter_swap = iter_swap_t{};

  }

}

#endif //GEOMETRICKS_ALGORITHM_ITER_SWAP_HPP
