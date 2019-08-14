cmake_minimum_required( VERSION 3.10 )
project( TCC VERSION 0.0.0
            DESCRIPTION "Advanced data structures library I'm creating for my undergraduate thesis."
          LANGUAGES CXX )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

include( CTest )
include( GoogleTest )

add_library( metaprogramming INTERFACE )
set( META_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/detect.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/list.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/literals.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/meta.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/utils.hpp
)
target_sources( metaprogramming INTERFACE ${META_HEADER_FILES} )
target_include_directories( metaprogramming INTERFACE include/ )
target_compile_features( metaprogramming INTERFACE cxx_std_17 )

add_library( functional INTERFACE )
set( FUNCTIONAL_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/functional/optional.hpp
)
target_include_directories( functional INTERFACE include/ )
target_compile_features( functional INTERFACE cxx_std_17 )
#target_link_libraries( functional INTERFACE )

option(BUILD_TCC_TESTS "Build tests for thesis" OFF)
if(${BUILD_TCC_TESTS} OR (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME))
  enable_testing()
  add_definitions(-DTEST_ENABLED)
  add_subdirectory(tests)
endif()
