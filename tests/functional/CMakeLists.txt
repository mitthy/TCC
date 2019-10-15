add_executable( TestOptional test_optional.cpp )
target_link_libraries( TestOptional gtest gmock gtest_main GeometricksFunctional )
gtest_discover_tests( TestOptional COMMAND TestOptional )
