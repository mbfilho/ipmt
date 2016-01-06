set encoding utf8
#set title "Tempo de Execucao" font ",20"
set xlabel 'Tamanho do Arquivo (Kb)'
set ylabel 'Tempo de Execução (segundos)'

set key outside nobox
set style data points

set output "mean.pdf"

set terminal pdf font 'Helvetica,10'
set grid

plot 'time.results' using 1:2 title column(2) with lp pt 7 ps 0.2 , \
	 'time.results' using 1:3 title column(3) with lp pt 2 ps 0.2, \
	 'time.results' using 1:4 title column(4) with lp pt 3 ps 0.2, \
	 'time.results' using 1:5 title column(5) with lp pt 4 ps 0.2, \
	 'time.results' using 1:6 title column(6) with lp pt 5 ps 0.2, \
	 'time.results' using 1:7 title column(7) with lp pt 6 ps 0.2, \
	 'time.results' using 1:8 title column(8) with lp pt 7 ps 0.2, \
	 'time.results' using 1:9 title column(9) with lp pt 8 ps 0.2, \
	 'time.results' using 1:10 title column(10) with lp pt 9 ps 0.2, \
	 'time.results' using 1:11 title column(11) with lp pt 10 ps 0.2, \
	

#"pmt" using 1:2 title "pmt"          w lp pt 6 lt rgb "green", \	"G"  using 1:2 title "Grep"         w lp pt 0 lt rgb "blue"
