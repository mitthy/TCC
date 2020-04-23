cmake_minimum_required( VERSION 3.10 )
project( Geometricks VERSION 0.0.0
  DESCRIPTION "Geometric data structures header only library for C++."
  LANGUAGES CXX
)

#Got this idea from EnTT project...

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you." )
endif()

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif()

option( BUILD_GEOMETRICKS_DOCS "Enable building with documentation." ON )

if( BUILD_GEOMETRICKS_DOCS )
  find_package( Doxygen 1.8 )
  if( DOXYGEN_FOUND )
    add_subdirectory( docs )
  endif()
endif()

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

include( CTest )
include( GoogleTest )

add_library( GeometricksMetaProgramming INTERFACE )
set( GEOMETRICKS_META_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta/conditional_constructors.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta/detect.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta/list.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta/literals.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta/utils.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/meta.hpp
)
target_sources( GeometricksMetaProgramming INTERFACE ${GEOMETRICKS_META_HEADER_FILES} )
target_include_directories( GeometricksMetaProgramming INTERFACE include/ )
target_compile_features( GeometricksMetaProgramming INTERFACE cxx_std_17 )

add_library( GeometricksMemory INTERFACE )
set( GEOMETRICKS_MEMORY_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/memory/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/memory/allocator/malloc_allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/memory/allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/memory/allocator/pool_allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/memory/allocator/new_allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/memory.hpp
)
target_sources( GeometricksMemory INTERFACE ${GEOMETRICKS_MEMORY_HEADER_FILES} )
target_include_directories( GeometricksMemory INTERFACE include/ )
target_compile_features( GeometricksMemory INTERFACE cxx_std_17 )

add_library( GeometricksAlgorithm INTERFACE )
set( GEOMETRICKS_ALGORITHM_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm/mean.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm/log.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm/partition.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm/absolute_difference.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm/iter_swap.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/algorithm.hpp
)
target_sources( GeometricksAlgorithm INTERFACE ${GEOMETRICKS_ALGORITHM_HEADER_FILES} )
target_include_directories( GeometricksAlgorithm INTERFACE include/ )
target_compile_features( GeometricksAlgorithm INTERFACE cxx_std_17 )

add_library( GeometricksDataStructure INTERFACE )
set( GEOMETRICKS_DATA_STRUCTURE_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/dimensional_traits.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/shapes.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/quad_tree.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/internal/free_list.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/kd_tree.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/internal/static_vector.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/geometricks/data_structure.hpp
)
target_sources( GeometricksDataStructure INTERFACE ${GEOMETRICKS_DATA_STRUCTURE_HEADER_FILES} )
target_include_directories( GeometricksDataStructure INTERFACE include/ )
target_compile_features( GeometricksDataStructure INTERFACE cxx_std_17 )
target_link_libraries( GeometricksDataStructure INTERFACE GeometricksAlgorithm GeometricksMetaProgramming GeometricksMemory )

set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

option( BUILD_GEOMETRICKS_TESTS "Build tests for Geometricks" OFF )
if( ${BUILD_GEOMETRICKS_TESTS} OR ( ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME} ) )
  enable_testing()
  add_definitions(-DTEST_ENABLED)
  add_subdirectory(tests)
endif()
