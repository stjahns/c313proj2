#ifndef TBEBSTATION_H
#define TBEBSTATION_H

#include "station.h"

class TBEBStation : public Station
{

private:
    int id;
    bool last_tx_success;
    int next_attempt;
    int collisions;
    
public:
    TBEBStation(int id, double p, int num_stations);
    virtual bool can_transmit(int slot);
    virtual void tx_collide(int slot);
    virtual void tx_success();
};

#endif
