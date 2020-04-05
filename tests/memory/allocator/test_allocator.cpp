#include "gtest/gtest.h"
#include "geometricks/memory/allocator.hpp"
#include "geometricks/memory/allocator/malloc_allocator.hpp"

TEST( TestAllocator, TestDefaultAllocator ) {
  geometricks::allocator default_allocator = geometricks::memory::get_default_allocator();
  EXPECT_EQ( default_allocator, geometricks::memory::new_allocator );
  int* allocated = nullptr;
  allocated = static_cast<int*>( allocate( default_allocator, sizeof( int ) ) );
  *allocated = 42;
  EXPECT_EQ( *allocated, 42 );
  deallocate( default_allocator, allocated );
  geometricks::memory::set_default_allocator( geometricks::memory::malloc_allocator );
  EXPECT_FALSE( geometricks::memory::get_default_allocator() == geometricks::memory::new_allocator );
  EXPECT_TRUE( geometricks::memory::get_default_allocator() == geometricks::memory::malloc_allocator );
  geometricks::memory::set_default_allocator( default_allocator );
  EXPECT_TRUE( geometricks::memory::get_default_allocator() == geometricks::memory::new_allocator );
}

TEST( TestAllocator, TestDefaultConstructor ) {
  geometricks::memory::set_default_allocator( geometricks::memory::new_allocator );
  geometricks::allocator default_constructed_allocator;
  EXPECT_EQ( default_constructed_allocator, geometricks::memory::new_allocator );
  int* allocated = nullptr;
  allocated = static_cast<int*>( allocate( default_constructed_allocator, sizeof( int ) ) );
  *allocated = 42;
  EXPECT_EQ( *allocated, 42 );
  deallocate( default_constructed_allocator, allocated );
  geometricks::memory::set_default_allocator( geometricks::memory::malloc_allocator );
  geometricks::allocator other;
  EXPECT_FALSE( default_constructed_allocator == other );
  geometricks::allocator other2;
  EXPECT_EQ( other2, other );
}

namespace dummy {

  struct dummy {} final;

  char buffer[1000];

  void* allocate( dummy&, size_t ) {
    return (void*)&buffer;
  }

  void deallocate( dummy&, void* ) {

  }

}

static_assert( geometricks::memory::is_allocator<dummy::dummy> );

TEST( TestAllocator, TestDummyAllocator ) {
  dummy::dummy dummy_alloc{};
  geometricks::allocator alloc{ dummy_alloc };
  EXPECT_EQ( alloc.allocate( 100 ), &dummy::buffer );
}
