#pragma once

#include <fftw3.h>
#include <complex>
#include <cstdint>
#include <tuple>
#include <vector>
#include "window.h"

class Transformer {
 public:
  Transformer(double target_interval, uint32_t sampling_rate, double overlap,
              WindowFunc func, bool get_db);
  ~Transformer();

  void transform(std::vector<double>& in,
                 std::vector<std::pair<double, double>>& out);
  uint32_t N();

 private:
  uint32_t N_;
  double* fftw_in_;
  std::complex<double>* fftw_out_;
  double target_interval_;
  uint32_t sampling_rate_;
  double overlap_;
  bool get_db_;
  fftw_plan fftw_plan_;

  WindowFunc func_;
};
