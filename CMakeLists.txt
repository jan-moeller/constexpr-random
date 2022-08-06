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

include(FetchContent)
fetchcontent_declare(
        Bugspray
        GIT_REPOSITORY https://github.com/jan-moeller/bugspray.git
        GIT_TAG v0.1.0
)
fetchcontent_makeavailable(Bugspray)

add_executable(constexpr_random-tests
        test/engines/helper_check_uniformness.hpp
        )
target_link_libraries(constexpr_random-tests PUBLIC bugspray-with-main constexpr_random)
set_target_properties(constexpr_random-tests PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
set_target_properties(constexpr_random-tests PROPERTIES COMPILE_FLAGS -fconstexpr-ops-limit=4294967296)