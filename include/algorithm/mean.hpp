#ifndef GEOMETRICKS_ALGORITHM_MEAN_HPP
#define GEOMETRICKS_ALGORITHM_MEAN_HPP

//C++ stdlib includes
#include <numeric>
#include <algorithm>
#include <iterator>

/**
* @file
* @brief Provides mean statistic function
*
* @details some geometric data structures can be optimized if we know the mean of the input data in advance.
* This file provides the mean functor along with helper traits.
*/

namespace geometricks {

  namespace algorithm {

    /**
    * @brief Type trait to get the number 0
    * @tparam T Numeric like type.
    * @details This trait is used by the mean function to init its internal value before additions.
    *
    * Example:
    * @code{.cpp}
    * struct BigInt {
        constexpr BigInt( const char * value );
        ...
      };
      ...
    * template< >
    * struct zero_traits<BigInt> {
        static constexpr BigInt zero() {
          return BigInt( "0" );
        }
      };
      @endcode
    */
    template< typename T >
    struct zero_traits {
      static constexpr T zero() {
        return 0;
      }
    };

    /**
    @brief Functor that computes the mean of a range of numbers.
    */
    struct mean_t {

      /**
      * @brief Computes the mean of a range of number.
      * @param first The begin of a range of elements.
      * @param last Another iterator pointing to the end of the range or a sentinel value that eventually compares false with first.
      * @pre If first and last are both iterators, first < last. Else, first != last eventually compares true.
      * @details Computes the mean of the elements in the range [first, last).
      */
      template< typename Iterator, typename Sentinel >
      auto operator()( Iterator first, Sentinel last ) const {
        using return_type = typename std::iterator_traits<Iterator>::value_type;
        auto size = std::distance( first, last );
        auto init = zero_traits<return_type>::zero();
        init = std::accumulate( first, last, std::move( init ), std::plus<>{} );
        return init / size;
      }

    };

    /**
    @brief Function object for @relatealso mean_t
    */
    constexpr auto mean = mean_t{};

  }  //namespace algorithm

}  //namespace geometricks

#endif //GEOMETRICKS_ALGORITHM_MEAN_HPP
