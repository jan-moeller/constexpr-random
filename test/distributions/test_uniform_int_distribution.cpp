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

#include <bugspray/bugspray.hpp>
#include <crand/distributions/uniform_int_distribution.hpp>
#include <crand/engines/xorshift_engine.hpp>
#include <crand/engines/xoshiro256_starstar_engine.hpp>

TEST_CASE("uniform_int_distribution", "[distributions]")
{
    using namespace crand;
    xoshiro256_starstar e;
    SECTION("distribution with same range as engine should produce same results")
    {
        auto          engine_copy = e;
        constexpr int n           = 100;

        uniform_int_distribution<xoshiro256_starstar::result_type> d;
        for (int i = 0; i < n; ++i)
        {
            auto const a = e();
            auto const b = d(engine_copy);
            CAPTURE(i);
            REQUIRE(a == b);
        }
    }
    SECTION("distribution with 1 element should always produce that element")
    {
        auto const elem = e();

        uniform_int_distribution d(elem, elem);

        constexpr auto n = 100;
        for (int i = 0; i < n; ++i)
        {
            auto const c = d(e);
            REQUIRE(c == elem);
        }
    }
    SECTION("distributions with negative numbers")
    {
        uniform_int_distribution d(-8, -3);
        constexpr auto           n = 100;
        for (int i = 0; i < n; ++i)
        {
            auto const c = d(e);
            REQUIRE(c >= d.a());
            REQUIRE(c <= d.b());
        }
    }
    SECTION("Works with engine that generates fewer bits than required")
    {
        xorshift32                              xe;
        uniform_int_distribution<std::uint64_t> d(0, std::numeric_limits<std::uint32_t>::max() + std::uint64_t(1u));
        constexpr auto                          n = 100;
        for (int i = 0; i < n; ++i)
        {
            auto c = d(xe);
            REQUIRE(c >= d.min());
            REQUIRE(c <= d.max());
        }
    }
}
EVAL_TEST_CASE("uniform_int_distribution");