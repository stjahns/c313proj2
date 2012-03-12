#ifndef IBSTATION_H
#define IBSTATION_H

#include "station.h"

class IBStation : public Station
{

private:
    int id;
    bool last_tx_success;
    int total_stations;
    int next_attempt;
    
public:
    IBStation(int id, double p, int num_stations);
    virtual bool can_transmit(int slot);
    virtual void tx_collide();
    virtual void tx_success();
};

#endif
