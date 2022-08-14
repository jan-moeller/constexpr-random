#
# MIT License
#
# Copyright (c) 2022 Jan Möller
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
cmake_minimum_required(VERSION 3.20)
project(constexpr_random)

add_library(constexpr_random
        include/crand/distributions/detail/uniform_int_distribution_details.hpp
        include/crand/distributions/normal_distribution.hpp
        include/crand/distributions/uniform_int_distribution.hpp
        include/crand/distributions/uniform_real_distribution.hpp
        include/crand/engines/detail/tiny_splitmix64.hpp
        include/crand/engines/detail/xorshift_engine_details.hpp
        include/crand/engines/detail/xoshiro256_starstar_details.hpp
        include/crand/engines/splitmix64_engine.hpp
        include/crand/engines/xorshift_engine.hpp
        include/crand/engines/xoshiro256_starstar_engine.hpp
        )
target_include_directories(constexpr_random PUBLIC include/)
set_target_properties(constexpr_random PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(constexpr_random PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

include(FetchContent)
fetchcontent_declare(
        Bugspray
        GIT_REPOSITORY https://github.com/jan-moeller/bugspray.git
        GIT_TAG v0.1.0
)
fetchcontent_makeavailable(Bugspray)

add_executable(constexpr_random-tests
        test/distributions/test_normal_distribution.cpp
        test/distributions/test_uniform_int_distribution.cpp
        test/distributions/test_uniform_real_distribution.cpp
        test/engines/helper_check_uniformness.hpp
        test/engines/test_splitmix64_engine.cpp
        test/engines/test_xorshift_engine.cpp
        test/engines/test_xoshiro256_starstar_engine.cpp
        )
target_link_libraries(constexpr_random-tests PUBLIC bugspray-with-main constexpr_random)
set_target_properties(constexpr_random-tests PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
set_target_properties(constexpr_random-tests PROPERTIES COMPILE_FLAGS -fconstexpr-ops-limit=4294967296)
