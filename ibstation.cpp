#include "ibstation.h"

IBStation::IBStation(int id, double p) : Station(id, p)
{
}

/**
 * Given the current slot, return true if the station is eligible to transmit
 * based on its assigned slot AND there is at least one frame in queue to
 * transmit
 */
bool IBStation::can_transmit(int slot)
{
    return false;
}

void IBStation::tx_collide()
{
}
