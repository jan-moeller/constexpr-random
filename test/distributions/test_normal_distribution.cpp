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
#include "crand/distributions/normal_distribution.hpp"
#include "crand/engines/xoshiro256_starstar_engine.hpp"

#include <bugspray/bugspray.hpp>

#include <algorithm>
#include <numbers>
#include <numeric>
#include <vector>

template<std::floating_point T>
constexpr auto erf(T x) noexcept -> T
{
    auto const t   = 1. / (1 + 0.5 * std::abs(x));
    auto const tau = t
                     * std::exp(
                         -(x * x) - 1.26551223 + 1.00002368 * t + 0.37409196 * t * t + 0.09678418 * std::pow(t, 3)
                         - 0.18628806 * std::pow(t, 4) + 0.27886807 * std::pow(t, 5) - 1.13520398 * std::pow(t, 6)
                         + 1.48851587 * std::pow(t, 7) - 0.82215223 * std::pow(t, 8) + 0.17087277 * std::pow(t, 9));
    if (x >= 0)
        return 1 - tau;
    return tau - 1;
}
static_assert(erf(0.) == 0.);
static_assert(std::abs(erf(1.) - 0.8427007929497148693412206350826092592960669979663029084599378978) < 1e-10);
static_assert(std::abs(erf(0.5) - 0.5204998778130465376827466538919645287364515757579637000588) < 1e-10);

template<std::floating_point T>
constexpr auto normal_cdf(T x, T mean = 0., T stddev = 1.) noexcept -> T
{
    return 0.5 * (1 + erf((x - mean) / (stddev * std::numbers::sqrt2_v<T>)));
}
static_assert(normal_cdf(0.) == 0.5);
static_assert(std::abs(normal_cdf(1.) - 0.8413447460685429485852325456320379224779129667266043909873944502) < 1e-10);
static_assert(std::abs(normal_cdf(0.5) - 0.6914624612740131036377046106083377398836021755545779368207) < 1e-10);

template<std::floating_point T>
constexpr auto calc_mean(std::vector<T> const& samples) -> T
{
    T const n = samples.size();
    return 1. / n * std::accumulate(samples.begin(), samples.end(), T{0});
}

template<std::floating_point T>
constexpr auto calc_stddev(std::vector<T> const& samples, T mean) -> T
{
    T const n = samples.size();
    return std::sqrt(1 / (n - 1)
                     * std::accumulate(samples.begin(),
                                       samples.end(),
                                       T{0},
                                       [&](T const& a, T const& b) { return a + std::pow(b - mean, 2); }));
}

template<std::floating_point T>
constexpr auto check_normality(std::vector<T> samples) -> bool
{
    return check_normality(samples, calc_mean(samples));
}

template<std::floating_point T>
constexpr auto check_normality(std::vector<T> samples, T mean) -> bool
{
    return check_normality(samples, mean, calc_stddev(samples, mean));
}

template<std::floating_point T>
constexpr auto check_normality(std::vector<T> samples, T mean, T stddev) -> bool
{
    // Anderson–Darling test
    T const n = samples.size();
    std::ranges::sort(samples);

    std::ranges::transform(samples, samples.begin(), [&](T v) { return (v - mean) / stddev; });
    T asquared = 0;
    for (std::size_t i = 0; i < samples.size(); ++i)
    {
        auto const p = normal_cdf(samples[i]);
        assert(p != 0 && p != 1);
        asquared += (T{2} * i - 1) * std::log(p) + (2 * (n - i) + 1) * std::log(1 - p);
    }
    asquared = -n - (1. / n) * asquared;

    return asquared < 1.621;
}

TEST_CASE("normal_distribution", "[distributions]")
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
        REQUIRE(random_number_distribution<normal_distribution<double>>);
    }

    normal_distribution d{5., 2.};
    std::vector<double> samples;
    for (int i = 0; i < runs; ++i)
    {
        auto const n = d(e);
        samples.push_back(n);
        REQUIRE(n >= d.min());
        REQUIRE(n <= d.max());
    }
    REQUIRE(check_normality(samples, 5., 2.));
}
EVAL_TEST_CASE("normal_distribution");