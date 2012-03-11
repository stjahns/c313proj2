#ifndef IBSTATION_H
#define IBSTATION_H

#include "station.h"

class IBStation : public Station
{

private:
    int id;
    int assigned_slot;
    int total_slots;
    
public:
    IBStation(int id, double p);
    virtual bool can_transmit(int slot);
    virtual void tx_collide();
};

#endif
