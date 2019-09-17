#include "gtest/gtest.h"
#include "algorithm/partition.hpp"
#include <vector>

TEST( TestPartition, TestPartitionWithLessThan ) {
  std::vector<int> test_vector = { 6, 3, 9, 5, 2, 8, 1, 7, 4 };
  auto func = []( const auto& element ) {
    return element <= 5;
  };
  auto middle = tcc::algorithm::partition( test_vector.begin(), test_vector.end(), func );
  for( auto it = test_vector.begin(); it != middle; ++it ) {
    EXPECT_TRUE( *it <= 5 );
  }
  for(; middle != test_vector.end(); ++middle ) {
    EXPECT_TRUE( *middle > 5 );
  }
}

TEST( TestPartition, TestPartitionEmptyVector ) {
  std::vector<int> empty_vector;
  auto always_true = []( const auto& ) { return true; };
  auto always_false = []( const auto& ) { return false; };
  auto begin = empty_vector.begin();
  auto end = empty_vector.end();
  EXPECT_TRUE( begin == end && end == tcc::algorithm::partition( begin, end, always_true ) );
  EXPECT_TRUE( begin == end && end == tcc::algorithm::partition( begin, end, always_false ) );
}

TEST( TestPartition, TestPartitionAllTrue ) {
  std::vector<int> test_vector = { 6, 3, 9, 5, 2, 8, 1, 7, 4 };
  auto begin = test_vector.begin();
  auto end = test_vector.end();
  auto always_true = []( const auto& ) { return true; };
  EXPECT_TRUE( begin != end && end == tcc::algorithm::partition( begin, end, always_true ) );
}

TEST( TestPartition, TestPartitionAllFalse ) {
  std::vector<int> test_vector = { 6, 3, 9, 5, 2, 8, 1, 7, 4 };
  auto begin = test_vector.begin();
  auto end = test_vector.end();
  auto always_false = []( const auto& ) { return false; };
  EXPECT_TRUE( begin != end && begin == tcc::algorithm::partition( begin, end, always_false ) );
}
