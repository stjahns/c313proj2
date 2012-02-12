#include <iostream>
#include <vector>
#include <cstdlib>
#include "simulator.h"

using namespace std;

int read_params( SimParams &params, int argc, char **argv );
void print_params( SimParams &params );

/***********************************************************************
 * Take simulation parameters from command line and create an appropriate 
 * instance of Simulator, then run the simulator.
 ***********************************************************************/
int main( int argc, char **argv )
{
    SimParams params;

    int valid = read_params(params, argc, argv);
    if (!valid) {
        cerr << "Error: Invalid parameters!" << endl;
        return -1;
    }

    print_params(params);
    
    Simulator sim(params);
    sim.run();

    return 0;
}

/***********************************************************************
 * Read the command line arguments into the SimParams struct
 ***********************************************************************/
int read_params( SimParams &params, int argc, char **argv ) 
{

    int valid = 1;
    int i;

    if (argc > 7) {
        params.feedback_time = atoi(argv[1]);
        params.num_blocks = atoi(argv[2]);
        params.frame_size = atoi(argv[3]);
        params.error_prob = atof(argv[4]);
        params.sim_length = atoi(argv[5]);
        params.num_trials = atoi(argv[6]);

        if (argc == params.num_trials + 7 ) {
            for ( i = 0; i < params.num_trials; i++ ) {
                params.seeds.push_back( atoi( argv[ 7 + i ] ) );
            }

        } else {
            valid = 0;
        }

    } else {
        valid = 0;
    }

    return valid;

}

/***********************************************************************
 * Print the simulation params as parsed from the command line args
 ***********************************************************************/
void print_params( SimParams &params ) {

    cout << params.feedback_time << " " <<
            params.num_blocks << " " <<
            params.frame_size << " " <<
            fixed << params.error_prob << " " <<
            params.sim_length << " " <<
            params.num_trials;

    int i;
    for ( i = 0; i < params.num_trials; i++ ) {
        cout << " " << params.seeds[i];
    }
    cout << endl;

}
