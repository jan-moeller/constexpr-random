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

#ifndef CONSTEXPR_RANDOM_SPLITMIX64_ENGINE_HPP
#define CONSTEXPR_RANDOM_SPLITMIX64_ENGINE_HPP

#include <bit>

#include <cstdint>

namespace crand
{
template<std::uint64_t m1,
         std::uint64_t m2,
         unsigned      p,
         unsigned      q,
         unsigned      r,
         std::uint64_t m3,
         std::uint64_t m4,
         unsigned      s,
         unsigned      t,
         unsigned      u>
class splitmix64_engine
{
  public:
    using result_type                          = std::uint64_t;
    static constexpr result_type default_seed  = 0xbad0ff1ced15ea5e;
    static constexpr result_type default_gamma = 0x9e3779b97f4a7c15;

    constexpr splitmix64_engine() noexcept
        : splitmix64_engine(default_seed)
    {
    }
    constexpr explicit splitmix64_engine(result_type seed, result_type gamma = default_gamma) noexcept
        : m_state(seed)
        , m_gamma(gamma)
    {
    }

    constexpr void seed(result_type seed = default_seed) noexcept { m_state = seed; }

    constexpr auto operator()() noexcept -> result_type
    {
        auto x = m_state;
        m_state += m_gamma;
        x ^= x >> s;
        x *= m3;
        x ^= x >> t;
        x *= m4;
        x ^= x >> u;
        return x;
    }

    constexpr void discard(unsigned long long z) noexcept { m_state += z * m_gamma; }

    constexpr auto split() noexcept -> splitmix64_engine
    {
        auto const seed = (*this)();
        auto       g    = m_state;
        g ^= g >> p;
        g *= m1;
        g ^= g >> q;
        g *= m2;
        g ^= g >> r;
        g |= 1u;
        int const n = std::popcount(g ^ (g >> 1u));
        if (n < 24)
            g ^= 0xaaaaaaaaaaaaaaaa;
        m_state += m_gamma;
        return splitmix64_engine{seed, g};
    }

    static constexpr auto min() noexcept -> result_type { return 0; }
    static constexpr auto max() noexcept -> result_type { return -1; }

    friend constexpr auto operator==(splitmix64_engine const& lhs, splitmix64_engine const& rhs) -> bool = default;

  private:
    result_type m_state;
    result_type m_gamma;
};

using splitmix64 = splitmix64_engine<0xff51afd7ed558ccd,
                                     0xc4ceb9fe1a85ec53,
                                     33,
                                     33,
                                     33,
                                     0xbf58476d1ce4e5b9,
                                     0x94d049bb133111eb,
                                     30,
                                     27,
                                     31>;
} // namespace crand

#endif // CONSTEXPR_RANDOM_SPLITMIX64_ENGINE_HPP
