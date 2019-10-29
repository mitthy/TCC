#include <gtest/gtest.h>
#include "memory/allocator/pool_allocator.hpp"
#include <vector>
#include <set>
#include <stdlib.h>
#include <time.h>

using namespace geometricks::memory;

struct dummy {

  char buff[ 100 ];

  dummy() {

  }

};

TEST( TestPoolAllocator, TestInt ) {
  std::set<uintptr_t> freed_set, alloc_set;
  auto alloc = make_multipool_allocator<512>( type<dummy>, type<int>, type<char> );
  std::vector<dummy*> allocations;
  for( int i = 0; i < 10000; ++i ) {
    dummy* dumm = ( dummy* )alloc.allocate<dummy>();
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) dumm )  == alloc_set.end() );
    alloc_set.insert( ( uintptr_t ) dumm );
    new ( dumm ) dummy{};
    allocations.push_back( dumm );
  }
  int free_it = ( rand() % 1000 ) + 300;
  for( int i = 0; i < free_it; ++i ) {
    dummy* last = allocations.back();
    EXPECT_FALSE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    alloc_set.erase( ( uintptr_t ) last );
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    EXPECT_TRUE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    freed_set.insert( ( uintptr_t ) last );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    last->~dummy();
    alloc.deallocate( last );
    allocations.pop_back();
  }
  for( int i = 0; i < free_it; ++i ) {
    dummy* dumm = ( dummy* )alloc.allocate<dummy>();
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) dumm )  == alloc_set.end() );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) dumm )  == freed_set.end() );
    alloc_set.insert( ( uintptr_t ) dumm );
    freed_set.erase( ( uintptr_t ) dumm );
    new ( dumm ) dummy{};
    allocations.push_back( dumm );
  }
  for( int i = 0; i < 527; ++i ) {
    dummy* dumm = ( dummy* )alloc.allocate<dummy>();
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) dumm )  == alloc_set.end() );
    EXPECT_TRUE( freed_set.find( ( uintptr_t ) dumm )  == freed_set.end() );
    alloc_set.insert( ( uintptr_t ) dumm );
    new ( dumm ) dummy{};
    allocations.push_back( dumm );
  }
  for( int i = 0; i < 312; ++i ) {
    dummy* last = allocations.back();
    EXPECT_FALSE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    alloc_set.erase( ( uintptr_t ) last );
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    EXPECT_TRUE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    freed_set.insert( ( uintptr_t ) last );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    last->~dummy();
    alloc.deallocate( last );
    allocations.pop_back();
  }
  for( int i = 0; i < 300; ++i ) {
    dummy* dumm = ( dummy* )alloc.allocate<dummy>();
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) dumm )  == alloc_set.end() );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) dumm )  == freed_set.end() );
    alloc_set.insert( ( uintptr_t ) dumm );
    freed_set.erase( ( uintptr_t ) dumm );
    new ( dumm ) dummy{};
    allocations.push_back( dumm );
  }
  for( int i = 0; i < 20; ++i ) {
    dummy* last = allocations.back();
    EXPECT_FALSE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    alloc_set.erase( ( uintptr_t ) last );
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    EXPECT_TRUE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    freed_set.insert( ( uintptr_t ) last );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    last->~dummy();
    alloc.deallocate( last );
    allocations.pop_back();
  }
  for( int i = 0; i < 32; ++i ) {
    dummy* dumm = ( dummy* )alloc.allocate<dummy>();
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) dumm )  == alloc_set.end() );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) dumm )  == freed_set.end() );
    alloc_set.insert( ( uintptr_t ) dumm );
    freed_set.erase( ( uintptr_t ) dumm );
    new ( dumm ) dummy{};
    allocations.push_back( dumm );
  }
  for( int i = 0; i < 200; ++i ) {
    dummy* dumm = ( dummy* )alloc.allocate<dummy>();
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) dumm )  == alloc_set.end() );
    EXPECT_TRUE( freed_set.find( ( uintptr_t ) dumm )  == freed_set.end() );
    alloc_set.insert( ( uintptr_t ) dumm );
    new ( dumm ) dummy{};
    allocations.push_back( dumm );
  }
  auto sz = allocations.size();
  for( int i = 0; i < sz; ++i ) {
    dummy* last = allocations.back();
    EXPECT_FALSE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    alloc_set.erase( ( uintptr_t ) last );
    EXPECT_TRUE( alloc_set.find( ( uintptr_t ) last )  == alloc_set.end() );
    EXPECT_TRUE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    freed_set.insert( ( uintptr_t ) last );
    EXPECT_FALSE( freed_set.find( ( uintptr_t ) last )  == freed_set.end() );
    last->~dummy();
    alloc.deallocate( last );
    allocations.pop_back();
  }
}
