set terminal png
set output "graph.png"

set size square
set polar
set rrange[45:85]
unset key
unset border
unset xtics
unset ytics
set label 1 '[dB]' at graph 1.01,0.45
set label 2 '90^o' at graph 1.01,0.5 left
set label 3 '135^o' at graph 0.8,0.85
set label 4 '180^o' at graph 0.5,1.0 center
set label 5 '225^o' at graph 0.1,0.85
set label 6 '270^o' at graph -0.01,0.5
set label 7 '315^o' at graph 0.1,0.15
set label 8 '0^o' at graph 0.5,0.0 center
set label 9 '45^o' at graph 0.8,0.15

set angles degrees
set grid polar 45

plot "resultSPL.dat" with linespoints