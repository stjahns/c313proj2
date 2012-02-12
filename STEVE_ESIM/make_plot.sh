#!/bin/bash

# Run simulation over a range of K values and E values, single plot with error
# bars

mkdir output

plots=""

for K in 0 1 2 5 10 25 50 100 500 1000 4000
do

    rm output/results_K${K} # clear file

    for E in $(seq -0.5 -0.2 -8 | awk '{print 10^$1}')

    do
        echo "Running experiment for K=${K}, E=$E"

        ./esim 10000 $K 8000 \
            $E \
            10000000 5 1 2 3 4 5 > output/results_K${K}_e${E}

        # grab third line (throughput)
        # or 2nd line (total tx'd / recieved)
        cat output/results_K${K}_e${E} | sed -n 3p | \
            awk "{print ${E},\$1,\$2,\$3;}" \
            >> output/results_K${K}

    done
    
    plots="${plots} \"output/results_K${K}\" lc ${K} title \"K = ${K}\" with yerrorbars,
                    \"output/results_K${K}\" lc ${K} notitle with lines," 
             
done


plots="${plots%,}" # remove trailing comma

gnuplot <<EOF
    set term postscript eps color blacktext "Helvetica" 12
    set output 'plot1.eps'
    set datafile separator " "
    set xlabel "Bit Error Rate"
    set ylabel "Throughput"
    set log x
    plot $(echo -e $plots)
    set output
    pause -1
EOF
