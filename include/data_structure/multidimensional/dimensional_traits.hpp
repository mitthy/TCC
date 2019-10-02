#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP

#include <utility>
#include <tuple>
#include "meta/utils.hpp"

namespace tcc {

  namespace data_structure {

    template< int Index >
    struct dimension_t {};

    template< int Index >
    constexpr dimension_t<Index> dimension_v;

    template< typename T >
    struct dimensional_traits;

    template< typename... Types >
    struct dimensional_traits< std::tuple<Types...> > {

      static constexpr int dimensions = sizeof...( Types );

      template< int Index, typename U = std::tuple<Types...> >
      static constexpr auto
      get( U&& object, dimension_t<Index> ) noexcept {
        return std::get<Index>( std::forward<U>( object ) );
      }

      template< int I >
      using type_at = std::decay_t< decltype( get( std::declval<std::tuple<Types...>>(), std::declval<dimension_t<I>>() ) ) >;

    };

  } //namespace data_structure

} //namespace tcc

#endif //TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_DIMENSIONAL_TRAITS_HPP
