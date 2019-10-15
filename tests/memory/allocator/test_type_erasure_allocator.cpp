#include "gtest/gtest.h"
#include "memory/allocator/type_erased_allocator.hpp"

TEST( TestTypeErasedAllocator, TestEmptyCreation ) {
  geometricks::memory::type_erased_allocator empty{};
  int* ptr = nullptr;
  int* before = ptr;
  ptr = static_cast<int*>( allocate( empty, sizeof( int ) ) );
  EXPECT_FALSE( ptr == before );
  EXPECT_TRUE( *ptr = 42 );
  deallocate( empty, ptr );
}
