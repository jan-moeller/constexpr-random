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

    int runs;
    if (std::is_constant_evaluated())
        runs = 1000;
    else
        runs = 100000;

    SECTION("inclusive - inclusive")
    {
        SECTION("distribution with same range as engine should produce same results")
        {
            auto engine_copy = e;

            using T                       = xoshiro256_starstar::result_type;
            T const                     a = 0;
            T const                     b = std::numeric_limits<T>::max();
            uniform_int_distribution<T> d(inclusive{a}, inclusive{b});
            for (int i = 0; i < runs; ++i)
            {
                auto const first  = e();
                auto const second = d(engine_copy);
                CAPTURE(i);
                REQUIRE(first == second);
            }
        }
        SECTION("distribution with 1 element should always produce that element")
        {
            int const                a = 0;
            int const                b = 0;
            uniform_int_distribution d(inclusive{a}, inclusive{b});

            for (int i = 0; i < runs; ++i)
            {
                auto const c = d(e);
                REQUIRE(c == a);
            }
        }
        SECTION("distributions with negative numbers")
        {
            int const                a = -8;
            int const                b = 2;
            uniform_int_distribution d(inclusive{a}, inclusive{b});
            REQUIRE(d.a() == d.min());
            REQUIRE(d.b() == d.max());
            for (int i = 0; i < runs; ++i)
            {
                auto const c = d(e);
                REQUIRE(c >= a);
                REQUIRE(c <= b);
            }
        }
        SECTION("Works with engine that generates fewer bits than required")
        {
            xorshift32                              xe;
            std::uint64_t                           a = 0;
            std::uint64_t                           b = std::numeric_limits<std::uint32_t>::max() + std::uint64_t(1u);
            uniform_int_distribution<std::uint64_t> d(inclusive{a}, inclusive{b});
            for (int i = 0; i < runs; ++i)
            {
                auto c = d(xe);
                REQUIRE(c >= a);
                REQUIRE(c <= b);
            }
        }
    }
    SECTION("exclusive - inclusive")
    {
        int const                a = -8;
        int const                b = 2;
        uniform_int_distribution d(exclusive{a}, inclusive{b});
        REQUIRE((d.a() + 1) == d.min());
        REQUIRE(d.b() == d.max());
        for (int i = 0; i < runs; ++i)
        {
            auto const c = d(e);
            REQUIRE(c > a);
            REQUIRE(c <= b);
        }
    }
    SECTION("inclusive - exclusive")
    {
        int const                a = -8;
        int const                b = 2;
        uniform_int_distribution d(inclusive{a}, exclusive{b});
        REQUIRE(d.a() == d.min());
        REQUIRE((d.b() - 1) == d.max());
        for (int i = 0; i < runs; ++i)
        {
            auto const c = d(e);
            REQUIRE(c >= a);
            REQUIRE(c < b);
        }
    }
    SECTION("exclusive - exclusive")
    {
        int const                a = -8;
        int const                b = 2;
        uniform_int_distribution d(exclusive{a}, exclusive{b});
        REQUIRE((d.a() + 1) == d.min());
        REQUIRE((d.b() - 1) == d.max());
        for (int i = 0; i < runs; ++i)
        {
            auto const c = d(e);
            REQUIRE(c > a);
            REQUIRE(c < b);
        }
    }
}
EVAL_TEST_CASE("uniform_int_distribution");