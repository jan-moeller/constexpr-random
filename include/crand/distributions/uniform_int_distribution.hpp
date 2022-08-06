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

#ifndef CONSTEXPR_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP
#define CONSTEXPR_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP

#include "detail/uniform_int_distribution_details.hpp"

#include <bit>
#include <limits>
#include <random>

#include <cassert>

namespace crand
{
template<typename IntType = int>
class uniform_int_distribution
{
  public:
    using result_type = IntType;

    constexpr uniform_int_distribution() noexcept
        : uniform_int_distribution(0)
    {
    }
    constexpr explicit uniform_int_distribution(IntType a, IntType b = std::numeric_limits<IntType>::max()) noexcept
        : m_a(a)
        , m_b(b)
        , m_range_bits(detail::uniform_int_distribution::range_bit_width(a, b))
    {
        assert(a <= b);
    }

    template<std::uniform_random_bit_generator G>
    constexpr auto operator()(G& g) -> result_type
    {
        using engine_int = std::invoke_result_t<G&>;
        using uint_t     = std::make_unsigned_t<result_type>;
        using result_t   = std::conditional_t<(sizeof(uint_t) > sizeof(engine_int)), uint_t, engine_int>;
        result_t result;
        do
        {
            result        = 0;
            std::size_t i = 0;
            for (; i < m_range_bits; i += sizeof(engine_int) * CHAR_BIT)
            {
                result <<= i;
                result |= g();
            }
            auto const shift = i - m_range_bits;
            assert(shift < sizeof(result) * CHAR_BIT);
            result >>= shift;
        } while (result > (b() - a()));
        return result + a();
    }

    constexpr auto a() const noexcept -> result_type { return m_a; }
    constexpr auto b() const noexcept -> result_type { return m_b; }

    constexpr auto min() const noexcept -> result_type { return a(); };
    constexpr auto max() const noexcept -> result_type { return b(); };

    friend constexpr auto operator==(uniform_int_distribution const& lhs, uniform_int_distribution const& rhs)
        -> bool = default;

  private:
    IntType      m_a;
    IntType      m_b;
    std::uint8_t m_range_bits;
};
} // namespace crand

#endif // CONSTEXPR_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP
