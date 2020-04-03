#include "gtest/gtest.h"
#include "geometricks/memory/allocator/malloc_allocator.hpp"
#include <type_traits>

TEST( TestMallocAllocator, TestAllocation ) {
  geometricks::memory::malloc_allocator_t* access_through_pointer = &geometricks::memory::malloc_allocator;
  int* allocated = nullptr;
  allocated = static_cast<int*>( allocate( *access_through_pointer, sizeof( int ) ) );
  *allocated = 42;
  EXPECT_EQ( *allocated, 42 );
}
