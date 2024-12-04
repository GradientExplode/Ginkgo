echo "plot '$1_$2.txt' using 1:2 title \"Ahn constant phase slice=$2\" , '$1_$2.txt' using 1:4" title \"EPI constant phase slice=$2\" | gnuplot -persist
echo "plot '$1_$2.txt' using 1:3 title \"Ahn linear phase slice=$2\" , '$1_$2.txt' using 1:5" title \"EPI linear phase slice=$2\" | gnuplot -persist
