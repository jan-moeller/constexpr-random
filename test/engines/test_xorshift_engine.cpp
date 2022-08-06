//
// MIT License
//
// Copyright (c) 2022 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "helper_check_uniformness.hpp"

#include <bugspray/bugspray.hpp>
#include <crand/engines/xorshift_engine.hpp>

TEST_CASE("xorshift32", "[engines]")
{
    using namespace crand;
    xorshift32 e;
    SECTION("discard(n) must be same as n * operator()")
    {
        auto       copy = e;
        auto const n    = 32;
        for (int i = 0; i < n; ++i)
        {
            copy();
        }
        e.discard(n);
        auto const first  = copy();
        auto const second = e();
        REQUIRE(first == second);
    }
    SECTION("should generate approximately uniform numbers")
    {
        CALL(helper_check_uniformness, e, 0.15); // This is a pretty bad generator
    }
}
EVAL_TEST_CASE("xorshift32");

TEST_CASE("xorshift64", "[engines]")
{
    using namespace crand;
    xorshift64 e;
    SECTION("discard(n) must be same as n * operator()")
    {
        auto       copy = e;
        auto const n    = 32;
        for (int i = 0; i < n; ++i)
        {
            copy();
        }
        e.discard(n);
        auto const first  = copy();
        auto const second = e();
        REQUIRE(first == second);
    }
    SECTION("should generate approximately uniform numbers")
    {
        CALL(helper_check_uniformness, e, 0.10);
    }
}
EVAL_TEST_CASE("xorshift64");