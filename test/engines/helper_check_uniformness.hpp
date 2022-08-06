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

#ifndef CONSTEXPR_RANDOM_HELPER_CHECK_UNIFORMNESS_HPP
#define CONSTEXPR_RANDOM_HELPER_CHECK_UNIFORMNESS_HPP

#include <bugspray/bugspray.hpp>

#include <array>
#include <random>

ASSERTING_FUNCTION(helper_check_uniformness, (std::uniform_random_bit_generator auto& gen, double delta))
{
    std::array<std::size_t, 256> occurences{};
    constexpr auto               runs = occurences.size() * 200;
    for (std::size_t i = 0; i < runs; ++i)
    {
        auto const r = gen();
        for (int i = 0; i < sizeof(r); ++i)
        {
            std::uint8_t byte = (r >> (i * 8));
            ++occurences[byte];
        }
    }
    std::size_t const expected = (runs * sizeof(gen())) / occurences.size();
    std::size_t const ok_max   = expected * (1 + delta);
    std::size_t const ok_min   = expected * (1 - delta);
    CAPTURE(expected, ok_max, ok_min, occurences);
    for (std::size_t i = 0; i < occurences.size(); ++i)
    {
        CAPTURE(i);
        REQUIRE(occurences[i] > ok_min);
        REQUIRE(occurences[i] < ok_max);
    }
}

#endif // CONSTEXPR_RANDOM_HELPER_CHECK_UNIFORMNESS_HPP
