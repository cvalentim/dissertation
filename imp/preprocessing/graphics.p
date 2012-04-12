# Gnuplot script for plotting experimental data
set autoscale
unset log
unset label
set xtic auto
set ytic auto
set title "Tempo x Tamanho entrada"
set xlabel "Numero de chaves"
set terminal latex 
set out 'graphics.tex'
plot "our_serieA.dat" using 1:2 with linespoint title 'Serie A', \
     "our_serieB.dat" using 1:2 with linespoint title 'Serie B', \
     "our_serieC.dat" using 1:2 with linespoint title 'Serie C',\
     "our_serieD.dat" using 1:2 with linespoint title 'Serie D'
set term pop
