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

#ifndef CONSTEXPR_RANDOM_XORSHIFT_ENGINE_HPP
#define CONSTEXPR_RANDOM_XORSHIFT_ENGINE_HPP

#include "detail/xorshift_engine_details.hpp"

#include <concepts>

#include <cstdint>

namespace crand
{
template<std::unsigned_integral T, std::uint8_t a, std::uint8_t b, std::uint8_t c>
class xorshift_engine
{
  public:
    using result_type = T;

    static constexpr result_type default_seed = 1;

    constexpr xorshift_engine() noexcept
        : xorshift_engine(default_seed)
    {
    }
    constexpr explicit xorshift_engine(result_type value) noexcept
        : m_state(detail::xorshift_engine::seed(value))
    {
    }

    constexpr void seed(result_type value = default_seed) noexcept { m_state = detail::xorshift_engine::seed(value); }

    constexpr auto operator()() noexcept -> result_type
    {
        m_state ^= m_state << a;
        m_state ^= m_state >> b;
        m_state ^= m_state << c;
        return m_state;
    }
    constexpr void discard(unsigned long long z) noexcept
    {
        for (unsigned long long i = 0; i < z; ++i)
            operator()();
    }

    static constexpr auto min() noexcept -> result_type { return 0; }
    static constexpr auto max() noexcept -> result_type { return -1; }

    friend constexpr auto operator==(xorshift_engine const& lhs, xorshift_engine const& rhs) -> bool = default;

  private:
    T m_state;
};

using xorshift32 = xorshift_engine<std::uint32_t, 13, 17, 5>;
using xorshift64 = xorshift_engine<std::uint64_t, 13, 7, 17>;
} // namespace crand

#endif // CONSTEXPR_RANDOM_XORSHIFT_ENGINE_HPP
