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

#ifndef CONSTEXPR_RANDOM_XOSHIRO256_STARSTAR_ENGINE_HPP
#define CONSTEXPR_RANDOM_XOSHIRO256_STARSTAR_ENGINE_HPP

#include "detail/xoshiro256_starstar_details.hpp"

#include <cstdint>

namespace crand
{
class xoshiro256_starstar
{
  public:
    using result_type = std::uint64_t;

    static constexpr result_type default_seed = 1;

    constexpr xoshiro256_starstar() noexcept
        : xoshiro256_starstar(default_seed)
    {
    }
    constexpr explicit xoshiro256_starstar(result_type value) noexcept
        : m_state(detail::xoshiro256_starstar::seed(value))
    {
    }

    constexpr void seed(result_type value = default_seed) noexcept
    {
        m_state = detail::xoshiro256_starstar::seed(value);
    }

    constexpr auto operator()() noexcept -> result_type { return detail::xoshiro256_starstar::advance_state(m_state); }
    constexpr void discard(unsigned long long z) noexcept
    {
        for (unsigned long long i = 0; i < z; ++i)
            operator()();
    }
    constexpr void discard_2_to_the_128() noexcept
    {
        m_state = detail::xoshiro256_starstar::generate_forwarded_state<
            {0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c}>(m_state);
    }
    constexpr void discard_2_to_the_192() noexcept
    {
        m_state = detail::xoshiro256_starstar::generate_forwarded_state<
            {0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635}>(m_state);
    }

    static constexpr auto min() noexcept -> result_type { return 0; }
    static constexpr auto max() noexcept -> result_type { return -1; }

    friend constexpr auto operator==(xoshiro256_starstar const& lhs, xoshiro256_starstar const& rhs) -> bool = default;

  private:
    std::array<std::uint64_t, 4> m_state;
};
} // namespace crand

#endif // CONSTEXPR_RANDOM_XOSHIRO256_STARSTAR_ENGINE_HPP
