#include "gtest/gtest.h"
#include "functional/optional.hpp"
#include <utility>
#include <type_traits>

static_assert( std::is_trivially_copyable_v< tcc::functional::optional< int > > );
static_assert( std::is_trivially_copy_constructible_v< tcc::functional::optional< int > > );
static_assert( std::is_trivially_destructible_v< tcc::functional::optional< int > > );
static_assert( std::is_trivially_move_assignable_v< tcc::functional::optional< int > > );
static_assert( std::is_trivially_move_constructible_v< tcc::functional::optional< int > > );

struct not_trivially_copyable {
  not_trivially_copyable() = default;
  not_trivially_copyable( const not_trivially_copyable& ) {}
  not_trivially_copyable( not_trivially_copyable&& ) = default;
  not_trivially_copyable& operator=( const not_trivially_copyable& ) & = default;
  not_trivially_copyable& operator=( not_trivially_copyable&& ) & = default;
};

static_assert( std::is_trivially_copy_assignable_v< not_trivially_copyable > );
static_assert( !std::is_trivially_copy_constructible_v< not_trivially_copyable > );
static_assert( std::is_trivially_destructible_v< not_trivially_copyable > );
static_assert( std::is_trivially_move_assignable_v< not_trivially_copyable > );
static_assert( std::is_trivially_move_constructible_v< not_trivially_copyable > );

static_assert( !std::is_trivially_copy_assignable_v< tcc::functional::optional< not_trivially_copyable > > );
static_assert( !std::is_trivially_copy_constructible_v< tcc::functional::optional< not_trivially_copyable > > );
static_assert( std::is_trivially_destructible_v< tcc::functional::optional< not_trivially_copyable > > );
static_assert( std::is_trivially_move_assignable_v< tcc::functional::optional< not_trivially_copyable > > );
static_assert( std::is_trivially_move_constructible_v< tcc::functional::optional< not_trivially_copyable > > );

struct not_trivially_copyable_assignable {
  not_trivially_copyable_assignable() = default;
  not_trivially_copyable_assignable( const not_trivially_copyable_assignable& ) = default;
  not_trivially_copyable_assignable( not_trivially_copyable_assignable&& ) = default;
  not_trivially_copyable_assignable& operator=( const not_trivially_copyable_assignable& ) & { return *this; };
  not_trivially_copyable_assignable& operator=( not_trivially_copyable_assignable&& ) & = default;
};

static_assert( !std::is_trivially_copy_assignable_v< not_trivially_copyable_assignable > );
static_assert( std::is_trivially_copy_constructible_v< not_trivially_copyable_assignable > );
static_assert( std::is_trivially_destructible_v< not_trivially_copyable_assignable > );
static_assert( std::is_trivially_move_assignable_v< not_trivially_copyable_assignable > );
static_assert( std::is_trivially_move_constructible_v< not_trivially_copyable_assignable > );

static_assert( !std::is_trivially_copy_assignable_v< tcc::functional::optional< not_trivially_copyable_assignable > > );
static_assert( !std::is_trivially_copy_constructible_v< tcc::functional::optional< not_trivially_copyable_assignable > > );
static_assert( std::is_trivially_destructible_v< tcc::functional::optional< not_trivially_copyable_assignable > > );
static_assert( std::is_trivially_move_assignable_v< tcc::functional::optional< not_trivially_copyable_assignable > > );
static_assert( std::is_trivially_move_constructible_v< tcc::functional::optional< not_trivially_copyable_assignable > > );

struct not_trivially_move_constructible {
  not_trivially_move_constructible() = default;
  not_trivially_move_constructible( const not_trivially_move_constructible& ) = default;
  not_trivially_move_constructible( not_trivially_move_constructible&& ) {}
  not_trivially_move_constructible& operator=( const not_trivially_move_constructible& ) & = default;
  not_trivially_move_constructible& operator=( not_trivially_move_constructible&& ) & = default;
};

static_assert( std::is_trivially_copy_assignable_v< not_trivially_move_constructible > );
static_assert( std::is_trivially_copy_constructible_v< not_trivially_move_constructible > );
static_assert( std::is_trivially_destructible_v< not_trivially_move_constructible > );
static_assert( std::is_trivially_move_assignable_v< not_trivially_move_constructible > );
static_assert( !std::is_trivially_move_constructible_v< not_trivially_move_constructible > );

static_assert( std::is_trivially_copy_assignable_v< tcc::functional::optional< not_trivially_move_constructible > > );
static_assert( std::is_trivially_copy_constructible_v< tcc::functional::optional< not_trivially_move_constructible > > );
static_assert( std::is_trivially_destructible_v< tcc::functional::optional< not_trivially_move_constructible > > );
static_assert( !std::is_trivially_move_assignable_v< tcc::functional::optional< not_trivially_move_constructible > > );
static_assert( !std::is_trivially_move_constructible_v< tcc::functional::optional< not_trivially_move_constructible > > );

struct not_trivially_move_assignable {
  not_trivially_move_assignable() = default;
  not_trivially_move_assignable( const not_trivially_move_assignable& ) = default;
  not_trivially_move_assignable( not_trivially_move_assignable&& ) = default;
  not_trivially_move_assignable& operator=( const not_trivially_move_assignable& ) & = default;
  not_trivially_move_assignable& operator=( not_trivially_move_assignable&& ) & { return *this; }
};

static_assert( std::is_trivially_copy_assignable_v< not_trivially_move_assignable > );
static_assert( std::is_trivially_copy_constructible_v< not_trivially_move_assignable > );
static_assert( std::is_trivially_destructible_v< not_trivially_move_assignable > );
static_assert( !std::is_trivially_move_assignable_v< not_trivially_move_assignable > );
static_assert( std::is_trivially_move_constructible_v< not_trivially_move_assignable > );

static_assert( std::is_trivially_copy_assignable_v< tcc::functional::optional< not_trivially_move_assignable > > );
static_assert( std::is_trivially_copy_constructible_v< tcc::functional::optional< not_trivially_move_assignable > > );
static_assert( std::is_trivially_destructible_v< tcc::functional::optional< not_trivially_move_assignable > > );
static_assert( !std::is_trivially_move_assignable_v< tcc::functional::optional< not_trivially_move_assignable > > );
static_assert( !std::is_trivially_move_constructible_v< tcc::functional::optional< not_trivially_move_assignable > > );

struct not_trivially_destructible {
  not_trivially_destructible() = default;
  ~not_trivially_destructible() {}
};

static_assert( !std::is_trivially_copyable_v< not_trivially_destructible > );
static_assert( !std::is_trivially_destructible_v< not_trivially_destructible > );

static_assert( !std::is_trivially_copyable_v< tcc::functional::optional< not_trivially_destructible > > );
static_assert( !std::is_trivially_destructible_v< tcc::functional::optional< not_trivially_destructible > > );

TEST( TestOptional, TestNoValueDefaultConstructor ) {
  tcc::functional::optional<int> empty_optional{};
  bool is_empty = !empty_optional;
  EXPECT_TRUE( is_empty );
}

TEST( TestOptional, TestValueConstructor ) {
  tcc::functional::optional<int> should_not_be_empty_optional{4};
  bool should_not_be_empty = should_not_be_empty_optional;
  EXPECT_TRUE( should_not_be_empty );
}

TEST( TestOptional, TestNoOptConstructor ) {
  tcc::functional::optional<int> empty_optional{tcc::functional::nullopt};
  bool is_empty = !empty_optional;
  EXPECT_TRUE( is_empty );
}

TEST( TestOptional, TestCorrectValueStored ) {
  tcc::functional::optional<int> should_not_be_empty_optional{4};
  ASSERT_TRUE( ( bool ) should_not_be_empty_optional );
  EXPECT_EQ(*should_not_be_empty_optional, 4);
  tcc::functional::optional<int> not_biased_please{42};
  EXPECT_EQ(*not_biased_please, 42);
  tcc::functional::optional<int> WE_ARE_PARANOID{666};
  EXPECT_EQ(*WE_ARE_PARANOID, 666);
}

TEST( TestOptional, TestAssignmentEmptyToValue ) {
  tcc::functional::optional<int> empty{};
  tcc::functional::optional<int> value{4};
  empty = value;
  EXPECT_EQ(*empty, 4);
}

TEST( TestOptional, TestAssignmentValueToEmpty ) {
  tcc::functional::optional<int> empty{};
  tcc::functional::optional<int> value{4};
  value = empty;
  EXPECT_FALSE( ( bool ) value );
  EXPECT_FALSE( ( bool ) empty );
}

TEST( TestOptional, TestMoveAssignment ) {
  tcc::functional::optional<int> empty{};
  tcc::functional::optional<int> value{ 4 };
  empty = std::move( value );
  EXPECT_TRUE( ( bool ) value );
  ASSERT_TRUE( ( bool ) empty );
  EXPECT_EQ( *empty, 4 );
  empty = tcc::functional::nullopt;
  EXPECT_FALSE( ( bool ) empty );
  value = empty;
  EXPECT_FALSE( ( bool ) value );
}

TEST( TestOptional, TestAssignOptionalToValue ) {
  tcc::functional::optional<int> opt{ 4 };
  opt = 5;
  ASSERT_TRUE( ( bool ) opt );
  EXPECT_EQ( *opt, 5 );
}

TEST( TestOptional, TestConstOptional ) {
  const tcc::functional::optional<int> opt{ 47 };
  ASSERT_TRUE( ( bool ) opt );
  EXPECT_EQ( *opt, 47 );
}

TEST( TestOptional, TestArrowAccess ) {
  struct life {
    int meaning() const { return 42; }
  };
  tcc::functional::optional<life> opt{life{}};
  EXPECT_EQ( opt->meaning(), 42 );
}

TEST( TestOptional, TestConstArrowAccess ) {
  struct life {
    int meaning() const { return 42; }
  };
  const tcc::functional::optional<life> opt{life{}};
  EXPECT_EQ( opt->meaning(), 42 );
}

TEST( TestOptional, TestNonTrivialCopy ) {
  struct not_trivial_to_copy_me {
    int m_value;
    not_trivial_to_copy_me(int value): m_value(value) {}
    not_trivial_to_copy_me(const not_trivial_to_copy_me& other): m_value(other.m_value) {}
  };
  tcc::functional::optional<not_trivial_to_copy_me> opt{ not_trivial_to_copy_me{ 777 } };
  tcc::functional::optional<not_trivial_to_copy_me> copy = opt;
  EXPECT_EQ( copy->m_value, 777 );
}

TEST( TestOptional, TestNonTrivialMove ) {
  struct not_trivial_to_move_me {
    int m_value;
    not_trivial_to_move_me(int value): m_value(value) {}
    not_trivial_to_move_me(not_trivial_to_move_me&& other): m_value(other.m_value) {
      other.m_value = 0;
    }
    ~not_trivial_to_move_me() = default;
  };
  tcc::functional::optional<not_trivial_to_move_me> opt{ not_trivial_to_move_me{ 42 } };
  EXPECT_TRUE( opt );
  EXPECT_EQ( opt->m_value, 42 );
  tcc::functional::optional<not_trivial_to_move_me> moved{ std::move( opt ) };
  EXPECT_EQ( moved->m_value, 42 );
  EXPECT_EQ( opt->m_value, 0 );
}
