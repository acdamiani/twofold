#include <pulse/sample.h>
#include <pulse/simple.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "audio.h"
#include "gnuplot-iostream.h"
#include "transform.h"
#include "window.h"

int main(int argc, char* argv[]) {
  Audio<float> a;

  a.load("/home/acdamiani/Downloads/Alesis-Fusion-Voice-Oohs-C4.wav");

  std::vector<float> samp = a.samples(0);

  Transformer t(0.001, a.sample_rate(), 0.0, WindowFunc::HANN, true);

  std::vector<double> in(samp.begin(), samp.end());
  std::vector<std::tuple<double, double, double>> out;

  t.transform(in, out);

  gnuplotio::Gnuplot gp;
  std::string file = gp.file1d(out, "/tmp/test2.dat");
  gp.clearTmpfiles();

  std::ifstream dat("/tmp/test2.dat");
  std::ofstream dat2("/tmp/test.dat");

  std::string f;
  std::string line;

  size_t i = 0;
  while (std::getline(dat, line)) {
    i++;

    dat2 << line << std::endl;

    if (i % t.N() == 0 && i != 0) {
      dat2 << std::endl;
    }
  }

  dat.close();

  dat2.close();

  gp << "reset" << std::endl;
  gp << "unset label" << std::endl;
  gp << "unset key" << std::endl;
  gp << "set title \"Spectrogram from audio data\" tc rgb 'white'" << std::endl;
  gp << "set xlabel \"Time (s)\" tc rgb 'white'" << std::endl;
  gp << "set ylabel \"Frequency (Hz)\" tc rgb 'white'" << std::endl;
  gp << "set palette rgbformulae 7,5,15" << std::endl;
  gp << "set object 1 rectangle from screen 0,0 to screen 1,1 fillcolor rgb "
        "'black' behind"
     << std::endl;
  gp << "set autoscale xy" << std::endl;
  gp << "set tics tc rgb 'white'" << std::endl;
  gp << "set pm3d map" << std::endl;
  gp << "set cbrange [-60:0]" << std::endl;
  gp << "splot \"/tmp/test.dat\"" << std::endl;
}
