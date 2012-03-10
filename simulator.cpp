#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#include "simulator.h"

Simulator::Simulator( params &p )
{

    this->num_stations = p.num_stations;
    this->gen_prob = p.gen_prob;
    this->num_slots = p.num_slots;
    this->num_trials = p.num_trials;
    this->seeds = p.seeds;

}

/***********************************************************************
 * Run num_trials trials of the simulation, then calculate and print
 * result statstics.
 ***********************************************************************/
void Simulator::run()
{

    /* Run T trials of length R */
    for (unsigned int i = 0; i < seeds.size(); i++) {
        run_trial(seeds[i]);
    }

    // print statistics ?

}

/***********************************************************************
 * Run a single trial of simulation
 ***********************************************************************/
void Simulator::run_trial( int seed )
{

    // loop for each timeslot 1,2,3 ... R
    int slot = 0;
    while (slot < this->num_slots) {

        // TODO some kind of class or struct to contain data/stats for
        // an individual station??
        
        // for each station ...
            // generate a frame to tx with probablity this->gen_prob
            //      add frame to stations tx queue
            // call should_transmit(station) to determine if station
            // should transmit or not - POLYMORPHISM YO
            // attempt transmit if returns true

        // check if multiple stations attempted transmission
        // if only 1, remove the frame from the station that tx'd 
        // successfully from its tx queue
        
        slot++;

    }

}

// virtual bool should_transmit(station) 
// return true if station allowed to (TDM) or should transmit based
// on whatever backoff algorithm used
// -> override this in the subclasses
