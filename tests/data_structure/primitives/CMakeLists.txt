add_executable( TestStaticVector test_static_vector.cpp )
target_link_libraries( TestStaticVector gtest gmock gtest_main GeometricksDataStructure )
gtest_discover_tests( TestStaticVector COMMAND TestStaticVector )
