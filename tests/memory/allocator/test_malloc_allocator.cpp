#include "gtest/gtest.h"
#include "geometricks/memory/allocator/malloc_allocator.hpp"
#include <type_traits>

using namespace geometricks::memory;

TEST( TestMallocAllocator, TestAllocation ) {
  int* allocated = nullptr;
  allocated = static_cast<int*>( allocate( malloc_allocator, sizeof( int ) ) );
  *allocated = 42;
  EXPECT_EQ( *allocated, 42 );
  deallocate( malloc_allocator, allocated );
}
