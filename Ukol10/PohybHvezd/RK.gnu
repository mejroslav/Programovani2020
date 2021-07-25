set term wxt
set key top right box

dataEuler = "dataEuler.txt"
dataRK = "dataRK.txt"

set size ratio -1

set xrange [-1.5:2]
set yrange [-1.5:2]

set style data lines
set key autotitle columnhead #

plot dataRK u 2:3 skip 1 title "Hvězda 1" ,\
	 dataRK u 5:6 skip 1 title "Hvězda 2" ,\
	 dataRK u 8:9 skip 1 title "Hvězda 3" ,\
	 dataRK u 11:12 skip 1 title "Hvězda 4"


set term pdf
set output "obrazek-RK.pdf"
replot 
set term wxt

