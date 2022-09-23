#pragma once

#include <cstddef>
#include <cstdio>

#define _WINDOW_CHECK_IN(I, N)                     \
  if (I < 0 || I > N) {                            \
    printf("ERROR: Invalid window function args"); \
    return 0.0;                                    \
  }

enum class WindowFunc { RECTANGULAR, HANN };

double win(WindowFunc func, double v, size_t I, size_t N);
double win_rectangular(double v, size_t I, size_t N);
double win_hann(double v, size_t I, size_t N);
