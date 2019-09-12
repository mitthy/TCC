#include "gtest/gtest.h"
#include "memory/allocator/allocator_base.hpp"
#include <type_traits>

TEST( TestMallocAllocator, TestAllocation ) {
  tcc::memory::allocator_base* access_through_pointer = &tcc::memory::malloc_allocator;
  int* allocated = nullptr;
  allocated = static_cast<int*>( access_through_pointer->allocate( sizeof( int ) ) );
  *allocated = 42;
  EXPECT_EQ( *allocated, 42 );
}
