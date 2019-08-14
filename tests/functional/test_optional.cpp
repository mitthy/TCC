/*
Copyright (c) 2019 Lucas Finger Roman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "gtest/gtest.h"
#include "functional/optional.hpp"
#include <utility>

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
  EXPECT_EQ(*should_not_be_empty_optional, 4);
  tcc::functional::optional<int> not_biased_please{42};
  EXPECT_EQ(*not_biased_please, 42);
  tcc::functional::optional<int> WE_ARE_PARANOID{666};
  EXPECT_EQ(*WE_ARE_PARANOID, 42);
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
  EXPECT_FALSE( ( bool ) value );
  EXPECT_TRUE( ( bool ) empty );
  EXPECT_EQ( *empty, 4 );
  empty = std::move( value );
  EXPECT_FALSE( ( bool ) value );
  EXPECT_FALSE( ( bool ) empty );
}
