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
#include "crand/distributions/bernoulli_distribution.hpp"
#include "crand/engines/xoshiro256_starstar_engine.hpp"

#include <bugspray/bugspray.hpp>

#include <algorithm>

TEST_CASE("bernoulli_distribution", "[distributions]")
{
    using namespace crand;
    xoshiro256_starstar e;

    int runs;
    if (std::is_constant_evaluated())
        runs = 1000;
    else
        runs = 100000;

    SECTION("p = 0.5")
    {
        bernoulli_distribution d{};
        std::vector<double>    samples;
        for (int i = 0; i < runs; ++i)
        {
            auto const n = d(e);
            samples.push_back(n);
            REQUIRE(n >= d.min());
            REQUIRE(n <= d.max());
        }
        REQUIRE((std::ranges::count(samples, true) - (samples.size() / 2.)) < 10.);
    }
    SECTION("p = 0")
    {
        bernoulli_distribution d{0.};
        for (int i = 0; i < runs; ++i)
        {
            auto const n = d(e);
            REQUIRE(n == false);
        }
    }
    SECTION("p = 1")
    {
        bernoulli_distribution d{1.};
        for (int i = 0; i < runs; ++i)
        {
            auto const n = d(e);
            REQUIRE(n == true);
        }
    }
}
EVAL_TEST_CASE("bernoulli_distribution");