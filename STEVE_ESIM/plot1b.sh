#!/bin/bash

$RUN_SIM=true

# Run simulation over a range of K values and E values, single plot with error
# bars

plots=""

for K in 0 1 2 5 10 20 30 50
do

    rm output/results_K${K} # clear file

    for E in $(seq 0.0000 0.00001 0.0001)
    do
        echo "Running experiment for K=${K}, E=$E"

        if $RUN_SIM; then
        ./ass1 100 $K 8000 \
            $E \
            1000000 5 1 2 3 4 5 > output/results_K${K}_e${E}
        fi

        # grab third line (throughput)
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
    set output 'plot1b.eps'
    set datafile separator " "
    set xlabel "Bit Error Rate"
    set ylabel "Throughput"
    plot $(echo -e $plots)
    set output
    pause -1
EOF
