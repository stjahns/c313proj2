#include <vector>

using namespace std;

#ifndef SIMULATOR_H
#define SIMULATOR_H

/**
 * Abstract simulator class. Subclass for the various TDM and ALOHA protocols 
 * to simulate
 */

class Simulator 
{

private:

    int num_stations;
    double gen_prob;
    int num_slots;
    int num_trials;
    vector<int> seeds;

#if 0
    /***********************************************************************
     * Calculate the block size required for the simulation's frame size
     * and number of correction blocks. If num_blocks = 0, assume no error 
     * correction bits so block_size = frame_size
     ***********************************************************************/
    int calc_block_size();

    /***********************************************************************
     * Run num_trials trials of the simulation, then calculate and print
     * result statstics.
     ***********************************************************************/
    void run_trial( int &time, int &frames, int &corr_frames, int seed );


    /***********************************************************************
     * Output mean and confidence interval for average total frames sent
     * per correct frame
     ***********************************************************************/
    void print_frame_statistics( const vector<int> &all_frames, 
                                 const vector<int> &corr_frames );

    /***********************************************************************
     * Output mean and confidence interval for bit throughput
     ***********************************************************************/
    void print_throughput_statistics( const vector<int> &corr_frames, 
                                      int time );

    /***********************************************************************
     * Output the mean, and lower and upper bounds of 95% confidence interval 
     * with given mean and mean squared error.
     ***********************************************************************/
    void print_ci( double mean, double mse );

    /***********************************************************************
     * Open file containing a range of t values for calculating 95% CI's, and 
     * return an appropriate t-value for the given degrees of freedom
     ***********************************************************************/
    double get_t_value( int df );

#endif

public:

    struct params {
        char protocol;
        int num_stations;
        double gen_prob;
        int num_slots;
        int num_trials;
        vector<int> seeds;
    };

    /***********************************************************************
     * Constructor - initialize the simulator with the given parameter 
     * struct
     ***********************************************************************/
    Simulator( params &p );
    void run();
    
};

#endif
