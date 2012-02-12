#!/bin/bash


# See what happens to k=8000 for sufficiently high error rates

plots=""

K=8000

rm output/results_K8000${K} # clear file

for E in $(seq 0.0000 0.002 0.05)
do
    echo "Running experiment for K=${K}, E=$E"

    ./ass1 100 $K 8000 \
        $E \
        1000000 5 1 2 3 4 5 > output/results_K${K}_e${E}

    # grab third line (throughput)
    cat output/results_K${K}_e${E} | sed -n 3p | \
        awk "{print ${E},\$1,\$2,\$3;}" \
        >> output/results_K${K}

done

plots="\"output/results_K${K}\" lc 1 title \"K = ${K}\" with yerrorbars,
       \"output/results_K${K}\" lc 1 notitle with lines" 
         


gnuplot <<EOF
    set term postscript eps color blacktext "Helvetica" 12
    set output 'plotk8000.eps'
    set datafile separator " "
    set xlabel "Bit Error Rate"
    set ylabel "Throughput"
    plot $(echo -e $plots)
    set output
    pause -1
EOF
