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

#ifndef CONSTEXPR_RANDOM_UNIFORM_REAL_DISTRIBUTION_HPP
#define CONSTEXPR_RANDOM_UNIFORM_REAL_DISTRIBUTION_HPP

#include "detail/uniform_real_distribution_details.hpp"
#include "distribution_limits.hpp"
#include "uniform_int_distribution.hpp"

#include <random>

#include <cassert>
#include <cmath>

namespace crand
{
template<class RealType = double>
class uniform_real_distribution
{
  public:
    using result_type = RealType;

    constexpr uniform_real_distribution(inclusive<RealType> a, inclusive<RealType> b) noexcept
        : m_a(a.value)
        , m_b(b.value)
        , m_g(detail::uniform_real_distribution::compute_gamma(m_a, m_b))
        , m_hi(m_g > 0 ? detail::uniform_real_distribution::ceilint(m_a, m_b, m_g) : 0)
        , m_min(m_a)
        , m_max(m_b)
        , m_int_dist(inclusive{std::size_t{0}}, inclusive{m_hi})
        , m_gn(std::abs(m_a) <= std::abs(m_b)
                   ? detail::uniform_real_distribution::gen_inclusive_inclusive_loe<RealType>
                   : detail::uniform_real_distribution::gen_inclusive_inclusive_nloe<RealType>)
    {
        assert(m_b - m_a <= std::numeric_limits<RealType>::max());
        assert(m_a <= m_b);
    }
    constexpr uniform_real_distribution(exclusive<RealType> a, inclusive<RealType> b) noexcept
        : m_a(a.value)
        , m_b(b.value)
        , m_g(detail::uniform_real_distribution::compute_gamma(m_a, m_b))
        , m_hi(detail::uniform_real_distribution::ceilint(m_a, m_b, m_g))
        , m_min(std::abs(m_a) <= std::abs(m_b) ? m_b - (m_hi - 1) * m_g : m_a + m_g)
        , m_max(m_b)
        , m_int_dist(inclusive{std::size_t{0}}, inclusive{m_hi - 1})
        , m_gn(std::abs(m_a) <= std::abs(m_b)
                   ? detail::uniform_real_distribution::gen_exclusive_inclusive_loe<RealType>
                   : detail::uniform_real_distribution::gen_exclusive_inclusive_nloe<RealType>)
    {
        assert(m_b - m_a <= std::numeric_limits<RealType>::max());
        assert(m_a < m_b);
    }
    constexpr uniform_real_distribution(inclusive<RealType> a, exclusive<RealType> b) noexcept
        : m_a(a.value)
        , m_b(b.value)
        , m_g(detail::uniform_real_distribution::compute_gamma(m_a, m_b))
        , m_hi(detail::uniform_real_distribution::ceilint(m_a, m_b, m_g))
        , m_min(m_a)
        , m_max(std::abs(m_a) <= std::abs(m_b) ? m_b - m_g : m_a + (m_hi - 1) * m_g)
        , m_int_dist(inclusive{std::size_t{1}}, inclusive{m_hi})
        , m_gn(std::abs(m_a) <= std::abs(m_b)
                   ? detail::uniform_real_distribution::gen_inclusive_exclusive_loe<RealType>
                   : detail::uniform_real_distribution::gen_inclusive_exclusive_nloe<RealType>)
    {
        assert(m_b - m_a <= std::numeric_limits<RealType>::max());
        assert(m_a < m_b);
    }
    constexpr uniform_real_distribution(exclusive<RealType> a, exclusive<RealType> b) noexcept
        : m_a(a.value)
        , m_b(b.value)
        , m_g(detail::uniform_real_distribution::compute_gamma(m_a, m_b))
        , m_hi(detail::uniform_real_distribution::ceilint(m_a, m_b, m_g))
        , m_min(std::abs(m_a) <= std::abs(m_b) ? m_b - (m_hi - 1) * m_g : m_a + m_g)
        , m_max(std::abs(m_a) <= std::abs(m_b) ? m_b - m_g : m_a + (m_hi - 1) * m_g)
        , m_int_dist(inclusive{std::size_t{1}}, inclusive{m_hi - 1})
        , m_gn(std::abs(m_a) <= std::abs(m_b)
                   ? detail::uniform_real_distribution::gen_exclusive_exclusive_loe<RealType>
                   : detail::uniform_real_distribution::gen_exclusive_exclusive_nloe<RealType>)
    {
        assert(m_b - m_a <= std::numeric_limits<RealType>::max());
        assert(m_a < m_b);
//        assert(std::nexttoward(m_a, m_b) != m_b); // TODO: check why it doesn't compile
    }

    template<std::uniform_random_bit_generator G>
    constexpr auto operator()(G& g) const -> result_type
    {
        auto const k = m_int_dist(g);
        return m_gn(m_a, m_b, m_g, m_hi, k);
    }

    [[nodiscard]] constexpr auto a() const noexcept -> result_type { return m_a; }
    [[nodiscard]] constexpr auto b() const noexcept -> result_type { return m_b; }
    [[nodiscard]] constexpr auto min() const noexcept -> result_type { return m_min; }
    [[nodiscard]] constexpr auto max() const noexcept -> result_type { return m_max; }
    [[nodiscard]] constexpr auto gamma() const noexcept -> result_type { return m_g; }
    [[nodiscard]] constexpr auto num_unique_values() const noexcept -> std::size_t
    {
        return m_int_dist.max() - m_int_dist.min();
    }

  private:
    RealType                              m_a;
    RealType                              m_b;
    RealType                              m_g;
    std::size_t                           m_hi;
    RealType                              m_min;
    RealType                              m_max;
    uniform_int_distribution<std::size_t> m_int_dist;
    RealType (*m_gn)(RealType a, RealType b, RealType g, RealType hi, std::size_t k);
};
} // namespace crand

#endif // CONSTEXPR_RANDOM_UNIFORM_REAL_DISTRIBUTION_HPP
