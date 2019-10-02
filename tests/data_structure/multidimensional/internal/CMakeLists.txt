add_executable( TestFreeList test_free_list.cpp )
target_link_libraries( TestFreeList gtest gmock gtest_main TCCDataStructure )
gtest_discover_tests( TestFreeList COMMAND TestFreeList )
