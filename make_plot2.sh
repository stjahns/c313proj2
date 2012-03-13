#!/bin/bash

# GOAL - compare MAC protocol performance in terms of 'throughput' and
# 'average delay per frame transmission' under a variety of different loading
# conditions, number of stations on bus, etc.

mkdir output
mkdir output/exp_1
rm -rf output/exp_1/* # clear old results

plot_t="" # plot command strings for throughput
plot_d="" # plot command strings for delay

# sim params
stations=20
gen_prob=1
trial_time=50000
trials=5
seeds='1 2 3 4 5'

color=1

# For each protocol...
for protocol in T P I B
do


    echo "Running for protocol ${protocol}..."

    #For a range of loads (gen_prob) between 0 and 1
    for gen_prob in $(seq 0 0.005 0.04)
    do

        echo "  for gen_prob = ${gen_prob}..."

        # run sim, take throughput CI (line 2)
        ./psim $protocol $stations $gen_prob $trial_time $trials $seeds > tmp
        
        throughput=$(cat tmp | sed -n 2p)
        delay=$(cat tmp | sed -n 3p)

        echo $gen_prob $throughput >> output/exp_1/results_t_${protocol}
        echo $gen_prob $delay >> output/exp_1/results_d_${protocol}

    done

    plot_t="${plot_t} \"output/exp_1/results_t_${protocol}\" \
                        lc ${color} title \"${protocol}\" with yerrorbars,
                    \"output/exp_1/results_t_${protocol}\" lc ${color} notitle with lines," 

    plot_d="${plot_d} \"output/exp_1/results_d_${protocol}\" \
                        lc ${color} title \"${protocol}\" with yerrorbars,
                    \"output/exp_1/results_d_${protocol}\" lc ${color} notitle with lines," 

    color=$((color + 1))

done

plot_t="${plot_t%,}" # remove trailing comma
plot_d="${plot_d%,}" # remove trailing comma


# plot throughput stats
gnuplot <<EOF
    set key
    set term postscript eps color blacktext "Helvetica" 12
    set output 'plot1_throughput.eps'
    set datafile separator " "
    set xlabel "Frame Generation Probability at each Time Slot"
    set ylabel "Throughput"
    plot $(echo -e $plot_t)
    set output
    pause -1
EOF

# plot delay stats
gnuplot <<EOF
    set key
    set term postscript eps color blacktext "Helvetica" 12
    set output 'plot1_delay.eps'
    set datafile separator " "
    set xlabel "Frame Generation Probability at each Time Slot"
    set ylabel "Average Delay per Frame Delivered"
    plot $(echo -e $plot_d)
    set output
    pause -1
EOF
