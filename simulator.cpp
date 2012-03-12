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
#include "tbebstation.h"
#include "conf_int.h"

Simulator::Simulator( params &p )
{

    this->num_stations = p.num_stations;
    this->gen_prob = p.gen_prob;
    this->num_slots = p.num_slots;
    this->num_trials = p.num_trials;
    this->seeds = p.seeds;

    // create vector of 'num_stations' stations for each of 'num_trials' trials,
    // according to the type of protocol being tested as per cmdline args
    for (int j = 0; j < num_trials; j++) {

        for (int i = 0; i < num_stations; i++) {
            this->stations.push_back(create_station(p.protocol, i, gen_prob, num_stations));
        }

        this->all_stations.push_back(stations);
        stations.clear();

    }

}

/**
 * Return a new allocated station as per the protocol type being used
 */
Station* Simulator::create_station( char type, int index, double p, int num_stations ) 
{

    Station *station = 0;

    switch (type) {

    case 'T':
        station = new TDMStation(index, p, num_stations);
        break;

    case 'P':
        station = new PBStation(index, p, num_stations);
        break;
        
    case 'I':
        station = new IBStation(index, p, num_stations);
        break;

    case 'B':
        station = new TBEBStation(index, p, num_stations);
        break;

    default:
        cerr << "Invalid protocol: " << type << endl;
        throw 0;
    }

    return station;
}

// Destructor, clean up allocated memory for each station
Simulator::~Simulator()
{
    for (unsigned int i = 0; i < all_stations.size(); i++ ) {
        Stations s = all_stations[i];
        for (unsigned int j = 0; j < s.size(); j++ ) {
            delete s[j];
        }
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
        run_trial(i, seeds[i]);
    }

    // print overall stats
    print_overall_stats();

    // print per station stats
    for (int id = 0; id < num_stations; id++ ) {
        print_station_stats(id);
    }

}

/***********************************************************************
 * Run a single trial of simulation - works the same for all MAC protocols
 * TODO need to collect data!
 ***********************************************************************/
void Simulator::run_trial(unsigned int trial, int seed )
{
	// seed RNG
    srand48(seed);

    // loop for each timeslot 1,2,3 ... R
    for (int slot = 0; slot < this->num_slots; slot++) {
        
        // vector of currently transmitting stations in this slot
        vector<Station*> transmitting;

        // for each station...
        for (int i = 0; i < this->num_stations; i++ ) {
            Station *station = all_stations[trial].at(i);
        
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
                transmitting[i]->tx_collide(i);
            }
        }
    }
}


/***********************************************************************
 * Print overall stats for all stations
 * - CI for thoughput
 * - CI for delay
 ***********************************************************************/
void Simulator::print_overall_stats() 
{
    // calculate mean throughput ( total frames delivered / number of slots )
    // and mean delay

    int total_frames_delivered = 0;
    int total_delay = 0;

    for (unsigned int trial = 0; trial < all_stations.size(); trial++ ) {
        for (int id = 0; id < num_stations; id++) {

            Station *s = all_stations[trial][id]; // get station for that trial

            total_frames_delivered += s->current_stats.delivered_frames;
            total_delay += s->current_stats.total_delay;
        }
    }

    double mean_throughput = (double)total_frames_delivered 
                           / ((double)num_slots * num_trials); // average throughput

    double mean_delay = (double)total_delay 
                 / (double)total_frames_delivered; // average delay per frame delivered
    
    // calculate MSE for throughput and delay 
    double mse_throughput = 0;
    double mse_delay = 0;

    for (unsigned int trial = 0; trial < all_stations.size(); trial++ ) {

        double trial_frames_delivered = 0;
        double total_trial_delay = 0;

        // get frames delivered, total delay for the single trial
        for (int id = 0; id < num_stations; id++) {

            Station *s = all_stations[trial][id]; // get station for that trial

            trial_frames_delivered += s->current_stats.delivered_frames;
            total_trial_delay += s->current_stats.total_delay;
        }

        double trial_throughput = trial_frames_delivered / (double) num_slots;

        mse_throughput += (trial_throughput - mean_throughput) 
                        * (trial_throughput - mean_throughput);

        double trial_delay = total_trial_delay / trial_frames_delivered;

        mse_delay += (trial_delay - mean_delay) 
                   * (trial_delay - mean_delay);

    }

    // calculate ci's for throughput and delay
    conf_int throughput_ci = calc_ci(mean_throughput, mse_throughput, num_trials);
    conf_int delay_ci = calc_ci(mean_delay, mse_delay, num_trials);

    cout << throughput_ci << endl
         << delay_ci << endl;

}

/***********************************************************************
 * Print statistics for a single station over all the trials
 * TODO all on one line!
 * - Station number
 * - CI for throughput
 * - CI for delay
 * - Ratios of undelivered to total generated frames for each trial
 ***********************************************************************/
void Simulator::print_station_stats(int id) 
{

    // calculate mean throughput ( total frames delivered / number of slots )
    // and mean delay
    int total_frames_delivered = 0;
    int total_delay = 0;
    for (unsigned int trial = 0; trial < all_stations.size(); trial++ ) {

        Station *s = all_stations[trial][id]; // get station for that trial

        total_frames_delivered += s->current_stats.delivered_frames;
        total_delay += s->current_stats.total_delay;

    }

    double mean_throughput = (double)total_frames_delivered 
                      / (double)(num_slots * num_trials); // average throughput

    double mean_delay = ((double)total_delay
                 / (double)total_frames_delivered)/all_stations.size(); // average delay per frame delivered

    // calculate MSE for throughput and delay
    double mse_throughput = 0;
    double mse_delay = 0;
    
    for (unsigned int trial = 0; trial < all_stations.size(); trial++ ) {

        Station *s = all_stations[trial][id]; // get station for that trial

        double trial_throughput = (double) s->current_stats.delivered_frames
                                / (double) num_slots;

        mse_throughput += (trial_throughput - mean_throughput) 
                        * (trial_throughput - mean_throughput);

        double trial_delay = ((double) s->current_stats.total_delay
                           / (double) s->current_stats.delivered_frames)/all_stations.size();

        mse_delay += (trial_delay - mean_delay) 
                   * (trial_delay - mean_delay);

    }

    // calculate ci's for throughput and delay
    conf_int throughput_ci = calc_ci(mean_throughput, mse_throughput, num_trials);
    conf_int delay_ci = calc_ci(mean_delay, mse_delay, num_trials);

    cout << id + 1 << endl // our stations indexed from 0, specs want from 1
         << throughput_ci << endl
         << delay_ci << endl;


    // print ratios of undelivered to total generated frames for each trial
    for (unsigned int trial = 0; trial < all_stations.size(); trial++) {

        Station *s = all_stations[trial][id]; // get station instance for that trial
        double ratio = 
              (double) s->current_stats.delay.size()
            / (double) s->current_stats.total_frames_gen;

        cout << " " << ratio;

    }

    cout << endl;

}
