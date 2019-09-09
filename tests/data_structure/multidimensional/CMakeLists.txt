add_executable( TestKDTree test_kd_tree.cpp )
target_link_libraries( TestKDTree gtest gmock gtest_main TCCDataStructure )
gtest_discover_tests( TestKDTree COMMAND TestKDTree )
