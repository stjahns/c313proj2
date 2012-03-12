#include <math.h>
#include <cstdlib>
#include "ibstation.h"

IBStation::IBStation(int id, double p, int num_stations) : Station(id, p)
{
	last_tx_success = true;
	total_stations = num_stations;
}

/**
 * Given the current slot, return true if the station is eligible to transmit
 * based on its assigned slot AND there is at least one frame in queue to
 * transmit
 */
bool IBStation::can_transmit(int slot)
{
	if (tx_queue > 0) {

		if (last_tx_success) {
			// always tx on first slot after success (if queue nonempty)
			return true;
		} else if ( slot == next_attempt ) {
			// check if slot is valid for retransmission attempt
			return true;
		}
	}

	return false;
}

void IBStation::tx_collide(int slot)
{
	last_tx_success = false;

	//randomly select slot for next retransmission attempt
	next_attempt = slot + ceil(drand48() * total_stations);
}

/*
 * On successful transmission, set last_tx_success to true to prevent interval based backoff
 */
void IBStation::tx_success()
{
    Station::tx_success(); // call superclass method
    last_tx_success = true;
}
