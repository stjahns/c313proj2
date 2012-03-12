#ifndef TDMSTATION_H
#define TDMSTATION_H

#include "station.h"

class TDMStation : public Station
{

private:
    int id;
    int assigned_slot;
    int total_slots;
    
public:
    TDMStation(int id, double p, int total_slots);
    virtual bool can_transmit(int slot);
    virtual void tx_collide(int slot);
    virtual void tx_success();
};

#endif
