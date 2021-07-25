set size ratio -1
set palette model RGB defined ( 0 'red', 4 'blue' )
set cbrange [0:4]
unset colorbox

set term gif size 800,800 animate;
set output "animace.gif";

stats "puntiky.txt" nooutput
do for [k=0:STATS_blocks-2] {
    plot [-80:80][-80:80] "puntiky.txt" \
        index k \
        using 1:2:3:3 \
        with circles palette fs solid;
}
