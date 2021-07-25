set term wxt

data = "data.txt"

set size ratio -1
set style data lines
set key autotitle columnhead #

plot data u 2:3 skip 1 title "Hvězda 1" ,\
	 data u 5:6 skip 1 title "Hvězda 2" ,\
	 data u 8:9 skip 1 title "Hvězda 3" ,\
	 data u 11:12 skip 1 title "Hvězda 4"


set term pdf
set output "obrazek-pohyb.pdf"
replot 
set term wxt

set xrange [0:10]
set yrange [-1500:-800]

plot data u 1:14 title "Energie"
