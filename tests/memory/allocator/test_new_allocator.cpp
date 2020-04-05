#include "gtest/gtest.h"
#include "geometricks/memory/allocator/new_allocator.hpp"
#include <type_traits>

using namespace geometricks::memory;

TEST( TestNewAllocator, TestAllocation ) {
  int* allocated = nullptr;
  allocated = static_cast<int*>( allocate( new_allocator, sizeof( int ) ) );
  *allocated = 42;
  EXPECT_EQ( *allocated, 42 );
  deallocate( new_allocator, allocated );
}
