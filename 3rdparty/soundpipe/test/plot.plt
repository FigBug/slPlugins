set terminal png size 900,400 font ',10'
set bmargin 10 
set output "demo.png"
set nokey
set style data histogram
unset border
set xtics rotate 
set xtics axis

plot 'time.log' using 2:xtic(1) with boxes

