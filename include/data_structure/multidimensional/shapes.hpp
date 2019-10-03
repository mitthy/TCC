#ifndef TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP
#define TCC_DATA_STRUCTURE_MULTIDIMENSIONAL_SHAPES_HPP

#include "meta/utils.hpp"
#include "algorithm/absolute_difference.hpp"

namespace tcc {

  namespace data_structure {

    struct rectangle {

      int x_center;

      int y_center;

      int half_width;

      int half_height;

      rectangle() = default;

      constexpr rectangle( int x_center_, int y_center_, int half_width_, int half_height_ ): x_center( x_center_ ),
                                                                                    y_center( y_center_ ),
                                                                                    half_width( half_width_ ),
                                                                                    half_height( half_height_ ) {}

      rectangle( const rectangle& ) = default;

      rectangle( rectangle&& ) = default;

      rectangle& operator=( const rectangle& ) = default;

      rectangle& operator=( rectangle&& ) = default;

    };

    constexpr bool intersects_with( const rectangle& lhs, const rectangle& rhs ) {
      return algorithm::absolute_difference( lhs.x_center, rhs.x_center ) < lhs.half_width + rhs.half_width &&
             algorithm::absolute_difference( lhs.y_center, rhs.y_center ) < lhs.half_height + rhs.half_height;
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

#endif
