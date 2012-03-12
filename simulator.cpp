#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#include "simulator.h"
#include "station.h"
#include "pbstation.h"
#include "tdmstation.h"
#include "ibstation.h"

Simulator::Simulator( params &p )
{

    this->num_stations = p.num_stations;
    this->gen_prob = p.gen_prob;
    this->num_slots = p.num_slots;
    this->num_trials = p.num_trials;
    this->seeds = p.seeds;

    // create vector of 'num_stations' stations
    switch( p.protocol ) {

    case 'T':
        for (int i = 0; i < num_stations; i++) {
            this->stations.push_back(new TDMStation(i, gen_prob, num_stations));
        }
        break;

    case 'P':
        for (int i = 0; i < num_stations; i++) {
            this->stations.push_back(new PBStation(i, gen_prob, num_stations));
        }
        break;

    case 'I':
        for (int i = 0; i < num_stations; i++) {
            this->stations.push_back(new IBStation(i, gen_prob, num_stations));
        }
        break;

    case 'B':
        for (int i = 0; i < num_stations; i++) {
            //this->stations.push_back( new TBEBStation(i, gen_prob) );
        }
        break;

    default:
        cerr << "Invalid protocol: " << p.protocol << endl;
        throw 0;
    }


}

// Destructor, clean up allocated memory
Simulator::~Simulator()
{
    // delete all the allocated stations
    for (unsigned int i = 0; i < stations.size(); i++ ) {
        delete stations[i];
    }

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
 * Run a single trial of simulation - works the same for all MAC protocols
 * TODO need to collect data!
 ***********************************************************************/
void Simulator::run_trial( int seed )
{

    // seed RNG
    srand48(seed);

    // loop for each timeslot 1,2,3 ... R
    for (int slot = 0; slot < this->num_slots; slot++) {
        
        // vector of currently transmitting stations in this slot
        vector<Station*> transmitting;

        // for each station...
        for (int i = 0; i < this->num_stations; i++ ) {
            Station *station = stations[i];
        
            // probabilistically generate a frame to tx 
            station->generate_frame();
                  
            // determine if station should transmit or not - POLYMORPHISM YO
            if (station->can_transmit(slot)) {
                // add to vector of currently transmitting stations
                transmitting.push_back(station);
            }

        }

        // check if only 1 station attempted transmission
        if (transmitting.size() == 1) {
            // transmission is not colliding! Success!
            transmitting.front()->tx_success();

        } else if (transmitting.size() > 1) {
            // trasmissions collide!
            for (unsigned int i = 0; i < transmitting.size(); i++) {
                transmitting[i]->tx_collide();
            }
        }
    }
}
