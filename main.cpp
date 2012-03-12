#include <iostream>
#include <vector>
#include <cstdlib>
#include "simulator.h"

using namespace std;

int read_params( Simulator::params &p, int argc, char **argv );
void print_params( Simulator::params &p );

/***********************************************************************
 * Take simulation parameters from command line and create an appropriate 
 * instance of Simulator, then run the simulator.
 ***********************************************************************/
int main( int argc, char **argv )
{

    Simulator::params p;

    int valid = read_params(p, argc, argv);
    if (!valid) {
        cerr << "Error: Invalid parameters!" << endl;
        return -1;
    }

    print_params(p);

    try {

        Simulator sim(p);
        sim.run();

    } catch (int e) {
        cerr << "Something went horribly wrong!" << endl;
    }

    return 0;
}

/***********************************************************************
 * Read the command line arguments into the SimParams struct
 ***********************************************************************/
int read_params( Simulator::params &p, int argc, char **argv ) 
{

    int valid = 1;
    int i;

    if (argc > 6) {
        p.protocol = argv[1][0];
        p.num_stations = atoi(argv[2]);
        p.gen_prob = atof(argv[3]);
        p.num_slots = atoi(argv[4]);
        p.num_trials = atoi(argv[5]);

        if (argc == p.num_trials + 6 ) {
            for ( i = 0; i < p.num_trials; i++ ) {
                p.seeds.push_back( atoi( argv[ 6 + i ] ) );
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
void print_params( Simulator::params &p ) {

    cout << p.protocol << " " <<
            p.num_stations << " " <<
            fixed << p.gen_prob << " " <<
            p.num_slots << " " <<
            p.num_trials  << " ";

    for (int k = 0; k < p.num_trials - 1; k++) {
    		cout << p.seeds.at(k) << " ";
    	}
    	cout << p.seeds.back() << endl;
}
