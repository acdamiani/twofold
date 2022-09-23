#include <pulse/sample.h>
#include <pulse/simple.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
#include "audio.h"
#include "transform.h"
#include "window.h"

int main(int argc, char* argv[]) {
  Audio<float> a;

  a.load("/home/acdamiani/Downloads/gettysburg.wav");

  std::vector<float> samp = a.samples();

  Transformer t(0.3, a.sample_rate(), 0.5, WindowFunc::HANN, false);

  std::vector<double> in(samp.begin(), samp.end());
  std::vector<std::pair<double, double>> out;

  t.transform(in, out);

  printf("%d\n", a.sample_rate());
  printf("%d\n", t.N());

  printf("F: %lf, V: %lf\n", out[23].first, out[23].second);

  /* pa_simple* s; */
  /* pa_sample_spec ss; */

  /* ss.format = PA_SAMPLE_FLOAT32NE; */
  /* ss.channels = a.channels(); */
  /* ss.rate = a.sample_rate(); */

  /* s = pa_simple_new(NULL, "twofold", PA_STREAM_PLAYBACK, NULL, "test", &ss,
   */
  /*                   NULL, NULL, NULL); */

  /* std::vector<float> sa = a.samples(); */

  /* printf("3: %zu\n", sa.size()); */

  /* int err; */
  /* pa_simple_write(s, sa.data(), sizeof(float) * sa.size(), &err); */

  return 0;
}
