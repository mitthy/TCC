#ifndef GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP
#define GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP

//C++ stblib includes
#include <iostream>

//Project includes
#include "meta/utils.hpp"
#include "algorithm/absolute_difference.hpp"

namespace geometricks {

  namespace data_structure {

    struct rectangle {

      int x_left;

      int y_top;

      int width;

      int height;

      rectangle() = default;

      constexpr rectangle( int x_left_, int y_top_, int width_, int height_ ):  x_left( x_left_ ),
                                                                                y_top( y_top_ ),
                                                                                width( width_ ),
                                                                                height( height_ ) {}

      rectangle( const rectangle& ) = default;

      rectangle( rectangle&& ) = default;

      rectangle& operator=( const rectangle& ) = default;

      rectangle& operator=( rectangle&& ) = default;

    };

    std::ostream& operator<<( std::ostream& os, const rectangle& rect ) {
      os << "Rect at -> TL: (" << rect.x_left << ':' << rect.y_top << ") ";
      os << "BR: (" << rect.x_left + rect.width << ':' << rect.y_top + rect.height << ") ";
      return os;
    }

    constexpr bool intersects_with( const rectangle& lhs, const rectangle& rhs ) {
      return  !( ( lhs.x_left > ( rhs.x_left + rhs.width ) ) || ( rhs.x_left > ( lhs.x_left + lhs.width ) ) ||
              ( lhs.y_top > ( rhs.y_top + rhs.height ) ) || ( rhs.y_top > ( lhs.y_top + lhs.height ) ) );
    }

    namespace rectangle_customization {

      //Speciallize this struct with a static function called _ that takes an object of your type and returns a rectangle.
      template< typename T >
      struct make_rectangle;

    }

    namespace __detail__ {

      template< int I, typename T >
      int get( T&& );

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<0> ) -> decltype( rectangle( value[0], value[1], value[2], value[3] ) ) {
        return rectangle( value[0], value[1], value[2], value[3] );
      }

      template< typename T >
      constexpr auto
      __make_rect__( const T& value, meta::priority_tag<1> ) -> decltype( rectangle( get<0>( value ), get<1>( value ), get<2>( value ), get<3>( value ) ) ) {
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

#endif //GEOMETRICKS_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP
