#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

#include "station.h"

Station::Station(int id, double p)
{
    this->id = id;
    this->tx_queue = 0;
    this->gen_prob = p;
}

// generate a frame to add to queue with prob. gen_prob
void Station::generate_frame()
{
    // use RNG to increment queue size with prob gen_prob
    if ( drand48() < gen_prob )
        tx_queue++;

}

// successful tx removes frame from tx queue
void Station::tx_success()
{
    tx_queue--;
    delivered++;
}

// print stats for station
void Station::print_stats()
{
	// cout << id << endl; //tested vector structure

//	cout << id << " " <<
//			throughput-of-n1 	<< " " <<
//			confidence-interval << " " <<
//			avg-delay-of-n1 	<< " " <<
//			confidence-interval << " " <<
//
//			undelivered1 << "/" << total1 << " " <<
//			undelivered2 << "/" << total2 << " " <<
//			undelivered3 << "/" << total3 << " " <<
//			undelivered4 << "/" << total4 << " " <<
//			undelivered5 << "/" << total5 << " " <<
//
//	    cout << endl;
}
