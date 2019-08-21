cmake_minimum_required( VERSION 3.10 )
project( TCC VERSION 0.0.0
            DESCRIPTION "Advanced data structures library I'm creating for my undergraduate thesis."
          LANGUAGES CXX )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

include( CTest )
include( GoogleTest )

add_library( TCCMetaProgramming INTERFACE )
set( TCC_META_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/conditional_constructors.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/detect.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/list.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/literals.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/meta.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/utils.hpp
)
target_sources( TCCMetaProgramming INTERFACE ${TCC_META_HEADER_FILES} )
target_include_directories( TCCMetaProgramming INTERFACE include/ )
target_compile_features( TCCMetaProgramming INTERFACE cxx_std_17 )

add_library( TCCFunctional INTERFACE )
set( TCC_FUNCTIONAL_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/functional/optional.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/functional/traits.hpp
)
target_sources( TCCFunctional INTERFACE ${TCC_FUNCTIONAL_HEADER_FILES} )
target_include_directories( TCCFunctional INTERFACE include/ )
target_compile_features( TCCFunctional INTERFACE cxx_std_17 )
target_link_libraries( TCCFunctional INTERFACE TCCMetaProgramming )

add_library( TCCDataStructure INTERFACE )
set( TCC_DATA_STRUCTURE_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/utils/log.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/bitmapped_vector_trie.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/primitives/static_vector.hpp
)
target_sources( TCCDataStructure INTERFACE ${TCC_DATA_STRUCTURE_HEADER_FILES} )
target_include_directories( TCCDataStructure INTERFACE include/ )
target_compile_features( TCCDataStructure INTERFACE cxx_std_17 )

option(BUILD_TCC_TESTS "Build tests for thesis" OFF)
if(${BUILD_TCC_TESTS} OR (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME))
  enable_testing()
  add_definitions(-DTEST_ENABLED)
  add_subdirectory(tests)
endif()
