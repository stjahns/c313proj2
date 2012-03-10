#include <vector>

using namespace std;

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
    vector<int> seeds;

    /***********************************************************************
     * Run a single trial of simulation
     ***********************************************************************/
    void run_trial( int seed );

#endif

public:

    struct params {
        char protocol;
        int num_stations;
        double gen_prob;
        int num_slots;
        int num_trials;
        vector<int> seeds;
    };

    /***********************************************************************
     * Constructor - initialize the simulator with the given parameter 
     * struct
     ***********************************************************************/
    Simulator( params &p );
    void run();
    
};

#endif
