/**
 * MIT License
 * 
 * Copyright (c) 2019 Ilwoong Jeong, https://github.com/ilwoong
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "string_algorithms.h"

#include <algorithm>

suffix_array_t get_suffix_array(const uint8_t* data, size_t count)
{
    suffix_array_t sa;

    for (int i = count - 1; i >= 0; --i) {
        suffix_t suffix = {static_cast<size_t>(i), count, data};
        sa[suffix] = i;
    }

    return sa;
}

longest_common_prefix_t get_lcp(suffix_array_t suffix_array)
{
    auto max = 0;
    longest_common_prefix_t lcp;

    auto prev = suffix_array.begin();
    lcp.cp_array[prev->first] = 0;
    
    for (auto curr = std::next(prev); curr != suffix_array.end(); std::advance(curr, 1))
    {
        auto prev_suffix = prev->first.data_ptr();
        auto curr_suffix = curr->first.data_ptr();
        auto min = std::min(prev->first.length(), curr->first.length());

        auto common = 0;
        while ((min--) > 0 && *(prev_suffix++) == *(curr_suffix++)) {
            common += 1;
        }

        if (max < common) {
            max = common;
        }

        lcp.cp_array[curr->first] = common;
        prev = curr;
    }

    lcp.lcp = max;

    return lcp;
}