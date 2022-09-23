#include "transform.h"
#include <fftw3.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <vector>
#include "window.h"

uint32_t closest_pow2(uint32_t x) {
  uint32_t p2a = x == 1 ? 1 : 1 << (32 - __builtin_clz(x - 1));
  uint32_t p2b = p2a >> 1;

  return (p2a - x) > (x - p2b) ? p2b : p2a;
}

uint32_t closest_powk(uint32_t x, uint8_t k) {
  uint32_t log = std::log(x) / std::log(k);

  uint32_t a = std::pow(k, log);
  uint32_t b = std::pow(k, log + 1);

  return ((a - x) > (x - b) ? b : a);
}

uint32_t closest_i(std::vector<uint32_t>& v, uint32_t d) {
  auto it = std::lower_bound(v.begin(), v.end(), d);
  if (it == v.begin())
    it = v.end();
  else
    --it;
  auto jt = std::upper_bound(v.begin(), v.end(), d), out = it;
  if (it == v.end() || jt != v.end() && d - *it > *jt - d) out = jt;
  return *out;
}

uint32_t get_best_n(double target_interval, uint32_t sampling_rate) {
  if (target_interval <= 0.0) {
    printf("ERROR: target_interval should be greater than 0\n");
    return 0;
  }

  // FFTW is fastest when working with numbers in the form 2^a, 3^b, 5^c, or 7^d

  uint32_t target =
      static_cast<uint32_t>(std::round(sampling_rate * target_interval));

  std::vector<uint32_t> vn = {closest_pow2(target), closest_powk(target, 3),
                              closest_powk(target, 5), closest_powk(target, 7)};

  std::sort(vn.begin(), vn.end());

  return closest_i(vn, target);
}

Transformer::Transformer(double target_interval, uint32_t sampling_rate,
                         double overlap, WindowFunc func, bool get_db) {
  target_interval_ = target_interval;
  sampling_rate_ = sampling_rate;
  overlap_ = overlap;
  get_db_ = get_db;
  func_ = func;

  N_ = get_best_n(target_interval_, sampling_rate_);

  fftw_in_ = new double[N_];
  fftw_out_ = new std::complex<double>[N_];

  std::filesystem::path path = std::filesystem::temp_directory_path();
  std::filesystem::path file("twofold_fftw_dft_r2c_1d.wis");

  std::filesystem::path full_path = path / file;

  if (std::filesystem::exists(full_path))
    fftw_import_wisdom_from_filename(full_path.c_str());

  fftw_plan_ = fftw_plan_dft_r2c_1d(
      N_, fftw_in_, reinterpret_cast<fftw_complex*>(fftw_out_), FFTW_PATIENT);

  fftw_export_wisdom_to_filename(full_path.c_str());
}

Transformer::~Transformer() {
  delete[] fftw_in_;
  delete[] fftw_out_;
  fftw_destroy_plan(fftw_plan_);
}

void Transformer::transform(std::vector<double>& in,
                            std::vector<std::pair<double, double>>& out) {
  out.clear();

  double overlap = std::max(0.0, std::min(overlap_, 0.5));
  uint32_t o = overlap * N_;

  std::vector<double> buf_c(in.begin(), in.end());

  while (buf_c.size() % N_) buf_c.push_back(0);

  std::vector<double> sampled;
  sampled.reserve(N_ * (buf_c.size() / (N_ - o)));

  for (size_t i = 0; i < in.size(); i += N_ - o)
    sampled.insert(sampled.end(), buf_c.begin() + i, buf_c.begin() + i + N_);

  for (size_t i = 0; i < sampled.size(); i += N_) {
    for (size_t j = 0; j < N_; j++) {
      double windowed = win(func_, sampled[i + j], j, N_);

      fftw_in_[j] = sampled[i + j] = windowed;
    }

    fftw_execute(fftw_plan_);

    for (size_t j = 0; j < N_; j++) {
      std::complex<double> c = fftw_out_[j];
      double v = c.real() * c.real() + c.imag() * c.imag();
      if (get_db_) v = 10 * std::log10(v);

      out.push_back(std::pair<double, double>(
          (static_cast<double>(sampling_rate_) / static_cast<double>(N_)) * j,
          v));
    }
  }
}

uint32_t Transformer::N() { return N_; }
