#include <vector>
#include <ostream>
#include "station.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H


/**
 * Abstract simulator class. Subclass for the various TDM and ALOHA protocols 
 * to simulate
 */

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
    Station* create_station(char type, int index, double p, int num_stations);


public:

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
    ~Simulator();
    void run();
    
    void print_overall_stats();
    void print_station_stats(int id); // print stats for single station over all trials
    void calc_throughput(); // throughput is # frames delivered / number of slots
	void calc_throughput_CI();
	void calc_avg_delay();
	void calc_avg_delay_CI();

};


#endif
