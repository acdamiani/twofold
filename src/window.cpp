#include "window.h"
#include <cmath>

double win(WindowFunc func, double v, size_t I, size_t N) {
  switch (func) {
    case WindowFunc::RECTANGULAR:
      return win_rectangular(v, I, N);
    case WindowFunc::HANN:
      return win_hann(v, I, N);
  }

  return v;
}

double win_rectangular(double v, size_t I, size_t N) {
  _WINDOW_CHECK_IN(I, N);
  return v;
}

double win_hann(double v, size_t I, size_t N) {
  _WINDOW_CHECK_IN(I, N);
  return v * 0.5 * (1 - std::cos((2 * M_PI * I) / N));
}
