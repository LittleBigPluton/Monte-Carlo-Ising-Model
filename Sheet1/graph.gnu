# Assume you have a data file "mean.dat"
set xlabel "N"
set ylabel "mu"
set title "mu versus N"
set logscale x 100
# plot on screen
#plot for [i=2:101]'mean.dat'using 1:i w p pt 6 ps 0.5 notitle
plot 'mean.dat' matrix  w p pt 6 ps 0.5
#With Points
#pt is Point Type which is 6 here
#ps is Point Size which is 0.5 here
pause -1

# replot on pdf-file
#set term push
#set term pdf
#set out "mean.pdf"
#rep
#set term pop
