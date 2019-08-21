add_executable( TestLog test_log.cpp )
target_link_libraries( TestLog gtest gmock gtest_main TCCDataStructure )
gtest_discover_tests( TestLog COMMAND TestLog )
