reset
unset label
unset key
set title "Spectrogram from audio data"
set xlabel "Time (s)"
set ylabel "Frequency (Hz)"
set palette rgbformulae 22,13,-31
set autoscale xy
set datafile separator whitespace
set pm3d map
set pm3d interpolate 0,0
splot '-' '/tmp/test.dat' using 1:2:3 with image
