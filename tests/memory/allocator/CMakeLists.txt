add_executable( TestMallocAllocator test_malloc_allocator.cpp )
target_link_libraries( TestMallocAllocator gtest gmock gtest_main TCCMemory )
gtest_discover_tests( TestMallocAllocator COMMAND TestMallocAllocator )
