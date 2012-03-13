#include <cstdlib>
#include <iostream>

using namespace std;

#include "station.h"

Station::Station(int id, double p)
{
    this->id = id;
    this->tx_queue = 0;
    this->gen_prob = p;

    current_stats.delivered_frames = 0;
    current_stats.total_frames_gen = 0;
    current_stats.total_delay = 0;
}

// generate a frame to add to queue with prob. gen_prob
void Station::generate_frame()
{
    // use RNG to increment queue size with prob gen_prob
    if ( drand48() < gen_prob ) {
        tx_queue++;
        current_stats.total_frames_gen++;
        current_stats.delay.push_back(0);
    }

}

// successful tx removes frame from tx queue
void Station::tx_success()
{
    tx_queue--;
    current_stats.delivered_frames++;

    current_stats.total_delay += current_stats.delay.front();
    current_stats.delay.pop_front();

}

// successful tx removes frame from tx queue
void Station::tx_collide(int slot)
{
}

void Station::increment_delays()
{
    // increment delays for each queued frame
	for (unsigned int i = 0; i < current_stats.delay.size(); i++) {
	    	current_stats.delay.at(i)++;
	}
}
