cmake_minimum_required( VERSION 3.10 )
project( TCC VERSION 0.0.0
            DESCRIPTION "Advanced data structures library I'm creating for my undergraduate thesis."
          LANGUAGES CXX )

#Got this idea from EnTT project...

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you." )
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

option( BUILD_TCC_DOCS "Enable building with documentation." ON )

if( BUILD_TCC_DOCS )
    find_package( Doxygen 1.8 )

    if( DOXYGEN_FOUND )
        add_subdirectory( docs )
    endif()
endif()

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

include( CTest )
include( GoogleTest )

add_library( TCCMetaProgramming INTERFACE )
set( TCC_META_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/conditional_constructors.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/detect.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/list.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/literals.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/utils.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/meta.hpp
)
target_sources( TCCMetaProgramming INTERFACE ${TCC_META_HEADER_FILES} )
target_include_directories( TCCMetaProgramming INTERFACE include/ )
target_compile_features( TCCMetaProgramming INTERFACE cxx_std_17 )

add_library( TCCMemory INTERFACE )
set( TCC_MEMORY_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/memory/reference_count.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/memory/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/memory/allocator/malloc_allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/memory/allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/memory/allocator/type_erased_allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/memory.hpp
)
target_sources( TCCMemory INTERFACE ${TCC_MEMORY_HEADER_FILES} )
target_include_directories( TCCMemory INTERFACE include/ )
target_compile_features( TCCMemory INTERFACE cxx_std_17 )

add_library( TCCFunctional INTERFACE )
set( TCC_FUNCTIONAL_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/functional/optional.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/functional.hpp
)
target_sources( TCCFunctional INTERFACE ${TCC_FUNCTIONAL_HEADER_FILES} )
target_include_directories( TCCFunctional INTERFACE include/ )
target_compile_features( TCCFunctional INTERFACE cxx_std_17 )
target_link_libraries( TCCFunctional INTERFACE TCCMetaProgramming )

add_library( TCCAlgorithm INTERFACE )
set( TCC_ALGORITHM_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm/distance.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm/mean.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm/log.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm/partition.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm/absolute_difference.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm.hpp
)
target_sources( TCCAlgorithm INTERFACE ${TCC_ALGORITHM_HEADER_FILES} )
target_include_directories( TCCAlgorithm INTERFACE include/ )
target_compile_features( TCCAlgorithm INTERFACE cxx_std_17 )

add_library( TCCDataStructure INTERFACE )
set( TCC_DATA_STRUCTURE_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/bitmapped_vector_trie.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/functional_bitmapped_vector_trie.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/multidimensional/dimensional_traits.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/multidimensional/kd_tree.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/multidimensional/quad_tree.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/primitives/static_vector.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure/all.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/data_structure.hpp
)
target_sources( TCCDataStructure INTERFACE ${TCC_DATA_STRUCTURE_HEADER_FILES} )
target_include_directories( TCCDataStructure INTERFACE include/ )
target_compile_features( TCCDataStructure INTERFACE cxx_std_17 )
target_link_libraries( TCCDataStructure INTERFACE TCCAlgorithm TCCMetaProgramming TCCFunctional )

option( BUILD_TCC_TESTS "Build tests for thesis" OFF )
if( ${BUILD_TCC_TESTS} OR ( PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME ) )
  enable_testing()
  add_definitions(-DTEST_ENABLED)
  add_subdirectory(tests)
endif()
