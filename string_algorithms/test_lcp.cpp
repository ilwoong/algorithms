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

#include <iostream>
#include <string>

#include "string_algorithms.h"

static void print_suffix(suffix_t suffix)
{
    for (auto i = suffix.start; i < suffix.end; ++i) {
        std::cout << suffix.data[i];
    }
}

int main(int argc, const char** argv) {

    const uint8_t data[] = {'b', 'a', 'n', 'a', 'n', 'a', 'b', 'a', 'n', 'a', 'n', 'a'};
    const size_t length = 12;

    auto sa = get_suffix_array(data, length);
    auto lcp = get_lcp(sa);
    auto map = lcp.cp_array;
    
    std::cout << "Longest Common Prefix: " << lcp.lcp << std::endl;
    for (auto iter = map.begin(); iter != map.end(); std::advance(iter, 1)) {
        std::cout << std::string(length - iter->first.length(), ' ');
        print_suffix(iter->first);

        std::cout << " " << iter->second << std::endl;
    }

    return 0;
}