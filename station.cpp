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
    // TODO use RNG to increment queue size with prob gen_prob
    tx_queue++;

}

// successful tx removes frame from tx queue
void Station::tx_success()
{
    tx_queue--;
}
