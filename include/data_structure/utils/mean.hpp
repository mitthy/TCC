#ifndef TCC_DATA_STRUCTURE_UTILS_MEAN_HPP
#define TCC_DATA_STRUCTURE_UTILS_MEAN_HPP

namespace tcc {

  namespace data_structure {

    namespace utils {

      template< typename T >
      struct mean_traits {
        static T zero() {
          return 0;
        }
      };

      template< typename T >
      struct mean {

      };

    }  //namespace utils

  }  //namespace data_structure

}  //namespace tcc

#endif
