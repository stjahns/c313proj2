#!/bin/bash

# Run simulation over a range of K values and E values, plots the optimal K value
# for each E value that provides the highest throughput.

plots=""

rm output/* # clear output folder

for K in {0..3}
do


    for E in $(seq -0.5 -0.2 -8 | awk '{print 10^$1}')

    do
        echo "Running experiment for K=${K}, E=$E"

        ./esim 100 $K 8000 \
            $E \
            1000000 5 1 2 3 4 5 \
        | sed -n 3p | \ awk "{print ${E},\$1}" \ >> output/${K}
        
    done

    for k in $(ls output)
    do 
        cat output



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
    set ylabel "Average Total Frames Transmitted / Frame Successfully Received"
    set log x
    plot $(echo -e $plots)
    set output
    pause -1
EOF
