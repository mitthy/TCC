add_executable( TestMallocAllocator test_malloc_allocator.cpp )
target_link_libraries( TestMallocAllocator gtest gmock gtest_main GeometricksMemory )
gtest_discover_tests( TestMallocAllocator COMMAND TestMallocAllocator )
add_executable( TestAllocator test_allocator.cpp )
target_link_libraries( TestAllocator gtest gmock gtest_main GeometricksMemory )
gtest_discover_tests( TestAllocator COMMAND TestAllocator )
add_executable( TestPoolAllocator test_pool_allocator.cpp )
target_link_libraries( TestPoolAllocator gtest gmock gtest_main GeometricksMemory )
gtest_discover_tests( TestPoolAllocator COMMAND TestPoolAllocator )
add_executable( TestNewAllocator test_new_allocator.cpp )
target_link_libraries( TestNewAllocator gtest gmock gtest_main GeometricksMemory )
gtest_discover_tests( TestNewAllocator COMMAND TestNewAllocator )
