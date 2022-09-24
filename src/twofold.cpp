#include <matplot/matplot.h>
#include <pulse/sample.h>
#include <pulse/simple.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
#include "audio.h"
#include "gnuplot-iostream/gnuplot-iostream.h"
#include "transform.h"
#include "window.h"

int main(int argc, char* argv[]) {
  Audio<float> a;

  a.load("/home/acdamiani/Downloads/gettysburg.wav");

  std::vector<float> samp = a.samples();

  Transformer t(0.3, a.sample_rate(), 0.0, WindowFunc::HANN, false);

  std::vector<double> in(samp.begin(), samp.end());
  std::vector<std::pair<double, double>> out;

  t.transform(in, out);

  gnuplotio::Gnuplot gp;

  gp << "set title \"Spectrogram\"\n";
  gp << "set autoscale xy\n";
  gp << "plot '-' with vectors title 'pts_A', '-' with vectors title 'pts_B'\n";

  gp.send1d(out);
}
