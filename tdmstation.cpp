#include "tdmstation.h"

TDMStation::TDMStation(int id, double p, int total_slots) : Station(id, p)
{
    this->assigned_slot = id;
    this->total_slots = total_slots;
}

/**
 * Given the current slot, return true if the station is eligible to transmit
 * based on its assigned slot AND there is at least one frame in queue to
 * transmit
 */
bool TDMStation::can_transmit(int slot)
{
    if ( (slot % total_slots) == assigned_slot && this->tx_queue > 0 )
        return true;
    else
        return false;
}

void TDMStation::tx_collide(int slot)
{
    // For TDM, don't really have any backoff mechanism for handling collisions...
}

void TDMStation::tx_success()
{
    Station::tx_success(); // dont really need to do anything more..
}
