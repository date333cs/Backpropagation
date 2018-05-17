unset key
set xrange [-6.5:6.5]
set yrange [-1.8:1.8]
set cbrange [0.2:0.8]
set size ratio -1 # set the scales so that the unit has the same length on both the x and y axes
set palette defined ( 0 'coral', 0.5 'white', 1 'web-blue')
plot "output_t0.dat"  using 2:1:3 with image, "ex_positive_001.dat" with points pt 3 ps 1 lc rgb "orange", "ex_negative_001.dat" w points pt 5 ps 1 lc rgb "skyblue", sin(3.14*x/2) lc rgb "black"
