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

#pragma once

#ifndef __ALGORITHMS_STRING_ALGORITHMS_H__
#define __ALGORITHMS_STRING_ALGORITHMS_H__

#include <cstdint>
#include <cstddef>
#include <map>

typedef struct {
    size_t start;
    size_t end;
    const uint8_t* data;

    const uint8_t* data_ptr() const {
        return data + start;
    }

    size_t length() const {
        return end - start;
    }
} suffix_t;

namespace std {
    template<> struct less<suffix_t>
    {
        bool operator() (const suffix_t& lhs, const suffix_t& rhs) {
            auto lhs_len = lhs.length();
            auto rhs_len = rhs.length();
            auto lptr = lhs.data_ptr();
            auto rptr = rhs.data_ptr();

            while (lhs_len > 0 && rhs_len > 0 && *lptr == *rptr) {
                lhs_len -= 1;
                rhs_len -= 1;

                ++lptr;
                ++rptr;
            }

            if (lhs_len == 0 || rhs_len == 0) {
                return lhs_len < rhs_len;
            }

            return *lptr < *rptr;
        }
    };
}

using suffix_array_t = std::map<suffix_t, size_t>;
using common_prefix_array_t = std::map<suffix_t, size_t>;

typedef struct {
    size_t lcp;
    common_prefix_array_t cp_array;
} longest_common_prefix_t;

suffix_array_t get_suffix_array(const uint8_t* data, size_t count);
longest_common_prefix_t get_lcp(suffix_array_t suffix_array);

#endif