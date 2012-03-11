#ifndef STATION_H
#define STATION_H

/**
 * Abstract Station class. Extend for each of the MAC protocols to implement
 * the can_transmit(), and tx_collide() methods
 */
class Station
{

private:
    int id;
    double gen_prob;
    int delivered;

protected:
    int tx_queue; // size of transmission queue in number of frames
    
public:

    Station(int id, double p);
    virtual bool can_transmit(int slot) = 0; // determine if station will tx in this slot
    virtual void tx_collide() = 0; // called when collision detected, protocol-specific imp
    void generate_frame(); // generate a frame to add to queue with prob. gen_prob
    virtual void tx_success(); // successful tx removes frame from tx queue

};

#endif
