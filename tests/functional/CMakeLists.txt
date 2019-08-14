add_executable( TestOptional test_optional.cpp )
target_link_libraries( TestOptional gtest gmock gtest_main functional )
gtest_discover_tests( TestOptional COMMAND TestOptional )
