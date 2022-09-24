# set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400 
# set output 'pm3dcolors.2.png'
set format cb "%3.1f" 
unset key
set view map scale 1
set samples 101, 101
set isosamples 2, 2
set style function pm3d
set xtics  norangelimit 2
unset ytics
unset ztics
set cbtics  norangelimit 0.1
set title "set palette defined" 
set xrange [ -10.0000 : 10.0000 ] noreverse nowriteback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ 0.00000 : 1.00000 ] noreverse nowriteback
set rrange [ * : * ] noreverse writeback
set palette defined ( 0 0.05 0.05 0.2, 0.1 0 0 1, 0.25 0.7 0.85 0.9,\
     0.4 0 0.75 0, 0.5 1 1 0, 0.7 1 0 0, 0.9 0.6 0.6 0.6,\
     1 0.95 0.95 0.95 )
g(x)=x
f(x)=(x+10)/20
NO_ANIMATION = 1
splot f(x)
