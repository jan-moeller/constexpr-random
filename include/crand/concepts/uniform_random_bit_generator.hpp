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

#ifndef CONSTEXPR_RANDOM_UNIFORM_RANDOM_BIT_GENERATOR_HPP
#define CONSTEXPR_RANDOM_UNIFORM_RANDOM_BIT_GENERATOR_HPP

#include <concepts>

namespace crand
{
template<class G>
concept uniform_random_bit_generator // clang-format off
    =  std::invocable<G&>
    && std::unsigned_integral<std::invoke_result_t<G&>>
    && requires {
           { G::min() } -> std::same_as<std::invoke_result_t<G&>>;
           { G::max() } -> std::same_as<std::invoke_result_t<G&>>;
           requires std::bool_constant<(G::min() < G::max())>::value;
    }; // clang-format on

namespace detail::uniform_random_bit_generator
{
struct uniform_random_bit_generator_archetype
{
    uniform_random_bit_generator_archetype()                                              = delete;
    uniform_random_bit_generator_archetype(uniform_random_bit_generator_archetype const&) = delete;
    uniform_random_bit_generator_archetype(uniform_random_bit_generator_archetype&&)      = delete;
    auto operator=(uniform_random_bit_generator_archetype const&)                         = delete;
    auto operator=(uniform_random_bit_generator_archetype&&)                              = delete;

    auto                  operator()() -> unsigned { return 0; };
    constexpr static auto min() -> unsigned { return 0; }
    constexpr static auto max() -> unsigned { return 1; }
};
static_assert(crand::uniform_random_bit_generator<uniform_random_bit_generator_archetype>);
} // namespace detail::uniform_random_bit_generator
} // namespace crand

#endif // CONSTEXPR_RANDOM_UNIFORM_RANDOM_BIT_GENERATOR_HPP
