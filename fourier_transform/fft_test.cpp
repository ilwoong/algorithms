/**
 * MIT License
 * 
 * Copyright (c) 2018 Ilwoong Jeong, https://github.com/ilwoong
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

#include <iomanip>
#include <iostream>
#include "fourier_transform.h"

using namespace spectral;

static void print_complex_array(const char* title, const ComplexArray& array) 
{
    std::cout << title << std::endl;
    std::cout << std::fixed << std::setprecision(5);
    for (auto item : array) {
        std::cout << item << std::endl;
    }
    std::cout << std::endl;
}

static void test_fft() 
{
    const Complex test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    ComplexArray data(test, 8);

    // forward fft
    fft(data);
    print_complex_array("fft", data);
 
    // inverse fft
    ifft(data); 
    print_complex_array("ifft", data);
}

static void test_czt() {
    const Complex test2[] = { 1, -1, -1, 1, -1, 1, -1, -1, 1, 1 };
    ComplexArray data2(test2, 10);
    
    czt(data2);
    print_complex_array("czt", data2);
}

int main()
{
    test_fft();
    test_czt();

    return 0;
}