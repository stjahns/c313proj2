#ifndef PBSTATION_H
#define PBSTATION_H

#include "station.h"

class PBStation : public Station
{

private:
    int id;
    bool last_tx_success;
    double backoff_prob;
    
public:
    PBStation(int id, double p, int num_stations);
    virtual bool can_transmit(int slot);
    virtual void tx_collide();
    virtual void tx_success();
};

#endif
