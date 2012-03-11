#ifndef PBSTATION_H
#define PBSTATION_H

#include "station.h"

class PBStation : public Station
{

private:
    int id;
    int assigned_slot;
    int total_slots;
    
public:
    PBStation(int id, double p);
    virtual bool can_transmit(int slot);
    virtual void tx_collide();
};

#endif
