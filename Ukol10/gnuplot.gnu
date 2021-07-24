set term wxt

data = "data.txt"

set size ratio -1
set style data lines

plot data u 2:3 title "Hvězda 1",\
	 data u 5:6 title "Hvězda 2",\
	 data u 8:9 title "Hvězda 3",\
	 data u 11:12 title "Hvězda 4"


set term pdf
set output "obrazek-pohyb.pdf"
replot 
set term wxt
