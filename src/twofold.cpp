#include <pulse/sample.h>
#include <pulse/simple.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <vector>
#include "audio.h"

int main(int argc, char* argv[]) {
  Audio<float> a;

  a.load("/home/acdamiani/Downloads/gettysburg.wav");

  pa_simple* s;
  pa_sample_spec ss;

  ss.format = PA_SAMPLE_FLOAT32NE;
  ss.channels = a.channels();
  ss.rate = a.sample_rate();

  s = pa_simple_new(NULL, "twofold", PA_STREAM_PLAYBACK, NULL, "test", &ss,
                    NULL, NULL, NULL);

  std::vector<float> sa = a.samples();

  printf("3: %zu\n", sa.size());

  int err;
  pa_simple_write(s, sa.data(), sizeof(float) * sa.size(), &err);

  return 0;
}
