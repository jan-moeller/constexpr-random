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
#include "crand/concepts/random_number_distribution.hpp"
#include "crand/distributions/uniform_real_distribution.hpp"
#include "crand/engines/xoshiro256_starstar_engine.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("uniform_real_distribution", "[distributions]")
{
    using namespace crand;
    xoshiro256_starstar e;

    int runs;
    if (std::is_constant_evaluated())
        runs = 1000;
    else
        runs = 100000;

    SECTION("satisfies random_number_distribution")
    {
        REQUIRE(random_number_distribution<uniform_real_distribution<double>>);
    }

    SECTION("inclusive - inclusive")
    {
        SECTION("range [0,1]")
        {
            double const              a = 0.;
            double const              b = 1.;
            uniform_real_distribution d(inclusive{a}, inclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a);
            REQUIRE(d.max() == b);

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k >= a);
                REQUIRE(k <= b);
            }
        }

        SECTION("range [42, 42]")
        {
            double const              a = 42.;
            double const              b = 42.;
            uniform_real_distribution d(inclusive{a}, inclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a);
            REQUIRE(d.max() == b);

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k == a);
            }
        }

        SECTION("range [0,max]")
        {
            double const              a = 0.;
            double const              b = std::numeric_limits<double>::max();
            uniform_real_distribution d(inclusive{a}, inclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a);
            REQUIRE(d.max() == b);

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k >= a);
                REQUIRE(k <= b);
            }
        }
    }
    SECTION("exclusive - inclusive")
    {
        SECTION("range (0,1]")
        {
            double const              a = 0.;
            double const              b = 1.;
            uniform_real_distribution d(exclusive{a}, inclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a + d.gamma());
            REQUIRE(d.max() == b);

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k > a);
                REQUIRE(k <= b);
            }
        }
        SECTION("range (0,max]")
        {
            double const              a = 0.;
            double const              b = std::numeric_limits<double>::max();
            uniform_real_distribution d(exclusive{a}, inclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a + d.gamma());
            REQUIRE(d.max() == b);

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k > a);
                REQUIRE(k <= b);
            }
        }
    }
    SECTION("inclusive - exclusive")
    {
        SECTION("range [0, 1)")
        {
            double const              a = 0.;
            double const              b = 1.;
            uniform_real_distribution d(inclusive{a}, exclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a);
            REQUIRE(d.max() == b - d.gamma());

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k >= a);
                REQUIRE(k < b);
            }
        }
        SECTION("range [0,max)")
        {
            double const              a = 0.;
            double const              b = std::numeric_limits<double>::max();
            uniform_real_distribution d(inclusive{a}, exclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a);
            REQUIRE(d.max() == b - d.gamma());

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k >= a);
                REQUIRE(k < b);
            }
        }
    }
    SECTION("exclusive - exclusive")
    {
        SECTION("range (0,1)")
        {
            double const              a = 0.;
            double const              b = 1.;
            uniform_real_distribution d(exclusive{a}, exclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a + d.gamma());
            REQUIRE(d.max() == b - d.gamma());

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k > a);
                REQUIRE(k < b);
            }
        }
        SECTION("range (0,max)")
        {
            double const              a = 0.;
            double const              b = std::numeric_limits<double>::max();
            uniform_real_distribution d(exclusive{a}, exclusive{b});
            REQUIRE(d.a() == a);
            REQUIRE(d.b() == b);
            REQUIRE(d.min() == a + d.gamma());
            REQUIRE(d.max() == b - d.gamma());

            for (int i = 0; i < runs; ++i)
            {
                auto const k = d(e);
                REQUIRE(k > a);
                REQUIRE(k < b);
            }
        }
    }
}
EVAL_TEST_CASE("uniform_real_distribution");