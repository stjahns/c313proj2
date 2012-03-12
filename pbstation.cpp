#include <cstdlib>
#include "pbstation.h"

PBStation::PBStation(int id, double p, int num_stations) : Station(id, p)
{
    last_tx_success = true;
    backoff_prob = 1 / (double)num_stations;
}

/**
 * Given the current slot, return true if the station is eligible to transmit
 * based on its assigned slot AND there is at least one frame in queue to
 * transmit
 */
bool PBStation::can_transmit(int slot)
{
    if (tx_queue > 0) {

        if (last_tx_success) {
            // always tx on first slot after success (if queue nonempty)
            return true;
        } else if ( drand48() < backoff_prob ) {
            // probabilistic backoff
            return true;
        }
    }
    
    return false;
}

/*
 * On transmission collision, set last_tx_success to false to enable
 * probabilistic backoff for subsequent tx attempts
 */
void PBStation::tx_collide(int slot)
{
    last_tx_success = false;
}

/*
 * On successful transmission, set last_tx_success to true to prevent 
 * probabilistic backoff
 */
void PBStation::tx_success()
{
    Station::tx_success(); // call superclass method
    last_tx_success = true;
}
