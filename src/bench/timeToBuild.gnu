set encoding utf8
set xlabel 'Tamanho do Arquivo (MB)'
set ylabel 'Tempo Médio de Execução (segundos)'

set key outside nobox
set style data points

set output "mean.pdf"

set terminal pdf font 'Helvetica,10'
set grid

plot 'time.results' using 1:2 title "Árvore de Sufixos" with lp pt 7 ps 0.2 , \
	 'time.results' using 1:3 title "Array de Sufixos" with lp pt 2 ps 0.2, \
	 'time.results' using 1:4 title "Árvore de Sufixos 2" with lp pt 2 ps 0.2, \
	

#"pmt" using 1:2 title "pmt"          w lp pt 6 lt rgb "green", \	"G"  using 1:2 title "Grep"         w lp pt 0 lt rgb "blue"
