set term wxt
set key top right box

dataEuler = "dataEuler.txt"
dataRK = "dataRK.txt"

set size ratio -1

set xrange [-1.5:2]
set yrange [-1.5:2]

set style data lines
set key autotitle columnhead #

plot dataEuler u 2:3 skip 1 title "Hvězda podle Eulera" ,\
	 dataRK u 2:3 skip 1 title "Hvězda podle RK4" ,\


set term pdf
set output "obrazek-srovnani.pdf"
replot 
set term wxt

