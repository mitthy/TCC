#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP

#include <tuple>
#include "meta/utils.hpp"

namespace tcc {

  namespace data_structure {

    struct rectangle {

      int x_center;

      int y_center;

      int width;

      int height;

      rectangle() = default;

      rectangle( int x_center_, int y_center_, int width_, int height_ ): x_center(x_center_ ), y_center( y_center_ ), width( width_ ), height( height_ ) {}

      rectangle( const rectangle& ) = default;

      rectangle( rectangle&& ) = default;

      rectangle& operator=( const rectangle& ) = default;

      rectangle& operator=( rectangle&& ) = default;

    };

    namespace rectangle_customization {

      //Speciallize this struct with a static function called _ that takes an object of your type and returns a rectangle.
      template< typename T >
      struct make_rectangle;

    }

    namespace __detail__ {

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<0> ) -> decltype( rectangle( value[0], value[1], value[2], value[3] ) ) {
        return rectangle( value[0], value[1], value[2], value[3] );
      }

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<1> ) -> decltype( rectangle( std::get<0>( value ), std::get<1>( value ), std::get<2>( value ), std::get<3>( value ) ) ) {
        return rectangle( std::get<0>( value ), std::get<1>( value ), std::get<2>( value ), std::get<3>( value ) );
      }

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<2> ) -> std::enable_if_t<std::is_convertible_v<decltype( value.bounding_rect() ), rectangle>, decltype( value.bounding_rect() )> {
        return static_cast<rectangle>( value.bounding_rect() );
      }

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<3> ) -> std::enable_if_t<std::is_convertible_v<decltype( value ), rectangle>, rectangle> {
        return static_cast<rectangle>( value );
      }

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<4> ) -> decltype( rectangle_customization::make_rectangle<T>::_( value ) ) {
        return rectangle_customization::make_rectangle<T>::_( value );
      }

    }

    inline constexpr auto
    make_rectangle = []( const auto& value ) -> decltype( __detail__::__make_rect__( value, meta::priority_tag<4>{} ) ) {
      return __detail__::__make_rect__( value, meta::priority_tag<4>{} );
    };

  }

}

#endif
