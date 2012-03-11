#ifndef TBEBSTATION_H
#define TBEBSTATION_H

#include "station.h"

class TBEBStation : public Station
{

private:
    int id;
    
public:
    TBEBStation(int id, double p);
    virtual bool can_transmit(int slot);
    virtual void tx_collide();
};

#endif
