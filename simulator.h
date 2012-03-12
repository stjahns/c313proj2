#include <vector>
#include <ostream>
#include "station.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H


/**
 * Simulator  - TODO documentation
 **/
class Simulator 
{

private:

    int num_stations;
    double gen_prob;
    int num_slots;
    int num_trials;
    std::vector<int> seeds;
    typedef std::vector<Station*> Stations;
    Stations stations;
    std::vector<Stations> all_stations;

    /***********************************************************************
     * Run a single trial of simulation
     ***********************************************************************/
    void run_trial(unsigned int trial, int seed);

    // allocate a new station object for the correct MAC protocol type
    Station* create_station(char type, int index, double p, int num_stations);
    
    // print stats for the entire hub over all trials
    void print_overall_stats(); 
    
    // print stats for single station over all trials
    void print_station_stats(int id); 


public:

    // structure to contain simulation params to pass to constructor, parse
    // into from cmdline
    struct params {
        char protocol;
        int num_stations;
        double gen_prob;
        int num_slots;
        int num_trials;
        std::vector<int> seeds;
    };

    /***********************************************************************
     * Constructor - initialize the simulator with the given parameter 
     * struct
     ***********************************************************************/
    Simulator( params &p );
    
    // free mem associated with station vectors
    ~Simulator();

    // run the simulation with given params, output statistics to stdout
    void run();

};


#endif
