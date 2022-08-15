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

#ifndef CONSTEXPR_RANDOM_BERNOULLI_DISTRIBUTION_HPP
#define CONSTEXPR_RANDOM_BERNOULLI_DISTRIBUTION_HPP

#include "crand/concepts/uniform_random_bit_generator.hpp"
#include "crand/distributions/uniform_real_distribution.hpp"

namespace crand
{
class bernoulli_distribution
{
  public:
    using result_type = bool;

    constexpr bernoulli_distribution() noexcept
        : bernoulli_distribution(0.5)
    {
    }
    constexpr explicit bernoulli_distribution(double p) noexcept
        : m_p(p)
    {
    }

    template<uniform_random_bit_generator G>
    constexpr auto operator()(G& g) const -> result_type
    {
        return s_dist(g) < m_p;
    }

    constexpr auto p() const noexcept -> double { return m_p; }

    constexpr auto min() const noexcept -> bool { return false; }
    constexpr auto max() const noexcept -> bool { return true; }

    friend constexpr auto operator==(bernoulli_distribution const& lhs, bernoulli_distribution const& rhs)
        -> bool = default;

  private:
    constexpr static uniform_real_distribution<double> s_dist{inclusive{0.}, exclusive{1.}};

    double m_p;
};
} // namespace crand

#endif // CONSTEXPR_RANDOM_BERNOULLI_DISTRIBUTION_HPP
