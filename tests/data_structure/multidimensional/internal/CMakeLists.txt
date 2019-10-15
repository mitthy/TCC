add_executable( TestFreeList test_free_list.cpp )
target_link_libraries( TestFreeList gtest gmock gtest_main GeometricksDataStructure )
target_compile_options( TestFreeList PRIVATE -Werror -Wall -Wextra )
gtest_discover_tests( TestFreeList COMMAND TestFreeList )
