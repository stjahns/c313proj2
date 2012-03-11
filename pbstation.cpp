#include "pbstation.h"

PBStation::PBStation(int id, double p) : Station(id, p)
{
}

/**
 * Given the current slot, return true if the station is eligible to transmit
 * based on its assigned slot AND there is at least one frame in queue to
 * transmit
 */
bool PBStation::can_transmit(int slot)
{
    return false;
}

void PBStation::tx_collide()
{
}
