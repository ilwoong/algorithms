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

#include <cmath>
#include "fourier_transform.h"

static constexpr double PI = acos(-1);
static constexpr spectral::Complex ONE(1, 0);
static constexpr uint8_t BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

inline static uint64_t bit_reverse(uint64_t value) {
    uint64_t reversed = 0;
    const uint8_t* in = (uint8_t*) &value;
    uint8_t* out = (uint8_t*) &reversed;

    out[0] = BitReverseTable256[in[7]];
    out[1] = BitReverseTable256[in[6]];
    out[2] = BitReverseTable256[in[5]];
    out[3] = BitReverseTable256[in[4]];
    out[4] = BitReverseTable256[in[3]];
    out[5] = BitReverseTable256[in[2]];
    out[6] = BitReverseTable256[in[1]];
    out[7] = BitReverseTable256[in[0]];

    return reversed;
}

void spectral::fft(ComplexArray& x) 
{
	const size_t N = x.size();
    size_t k = N, n;
	double theta = PI / N;
	Complex phi = Complex(cos(theta), -sin(theta)), tmp;
	
    while (k > 1)
	{
		n = k;
		k >>= 1;
		phi = phi * phi;
		tmp = 1.0L;
		for (auto l = 0; l < k; l++)
		{
			for (auto a = l; a < N; a += n)
			{
				auto b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * tmp;
			}
			tmp *= phi;
		}
	}
	
	auto m = static_cast<uint64_t>(log2(N));
	for (auto a = 0; a < N; a++)
	{
		auto b = bit_reverse(a) >> (64 - m);
		if (b > a)
		{
            std::swap(x[a], x[b]);
		}
	}
}

void spectral::ifft(ComplexArray& x) 
{
    x = x.apply(std::conj);

    fft(x);

    x = x.apply(std::conj);

    x /= x.size();
}

void spectral::fft_recursive(ComplexArray& x) 
{
    const size_t N = x.size();
    if (N <= 1) return;
 
    ComplexArray even = x[std::slice(0, N/2, 2)];
    ComplexArray  odd = x[std::slice(1, N/2, 2)];
 
    fft(even);
    fft(odd);
 
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

void spectral::ifft_recursive(ComplexArray& x) 
{
    x = x.apply(std::conj);

    fft_recursive(x);

    x = x.apply(std::conj);

    x /= x.size();
}

void spectral::czt(ComplexArray& dst)
{
    size_t length = dst.size();
    double unit_rad = PI / length;
    size_t chirpLen = length + length - 1;

    // generate chirp
    ComplexArray chirp(chirpLen);
    for (size_t i = 0; i < chirpLen; ++i) {
        double arg = (1.0 - static_cast<double>(length) + i);
        arg *= unit_rad * arg;
        chirp[i] = Complex(cos(arg), -sin(arg));
    }
    
    // Choose L = 2^m & L >= N + M - 1
    size_t length_pow2 = 1;
    while(length_pow2 < chirpLen) {
        length_pow2 <<= 1;
    }

    // form y_n & Compute Y : L point DFT of y_n
    ComplexArray Y(length_pow2);
    for (size_t i = 0; i < length; ++i) {
        Y[i] = dst[i] * chirp[length - 1 + i];
    }
    fft(Y);

    // Form v_n & Compute V : L point DFT of v_n
    ComplexArray V(length_pow2);
    for (size_t i = 0; i < chirpLen; ++i) {
        V[i] = ONE / chirp[i];
    }
    fft(V);

    // from g_n : multiply V and Y & compute G : L point IDFT of g_n
    ComplexArray G = Y * V;
    ifft(G);

    // multiply g_k by W^k^2/2.0 && discard g_k for k >= M
    for (size_t i = 0; i < length; ++i) {
        dst[i] = G[length - 1 + i] * chirp[length - 1 + i];
    }
}
