add_subdirectory( allocator )
add_executable( TestReferenceCount test_reference_count.cpp )
target_link_libraries( TestReferenceCount gtest gmock gtest_main TCCMemory )
gtest_discover_tests( TestReferenceCount COMMAND TestReferenceCount )
