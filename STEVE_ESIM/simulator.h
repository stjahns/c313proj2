#include <vector>

using namespace std;

#ifndef SIMULATOR_H
#define SIMULATOR_H

/**
 * Simulator class. A simulator object can be initialized in the constructor 
 * with a given SimParams struct containing the feedback time to acknowledge 
 * a correct frame sent, the desired number of error correction blocks per frame,
 * the probability of error for each bit transmitted, the length of the simulation
 * in number of bits, the number of trial repititions, and a vector of seeds 
 * for the RNG for each trial.
 *
 * On construction, Simulator will calculate the number of error correction bits 
 * needed for the parameters and calculate the correction block size.
 *
 * Calling simulator.run() will simulate the desired number of trials and 
 * output mean and 95% confidence intervals for the average total frames sent 
 * per correct frame, and for the average throughput.
 */

/*
 * Struct to represent the current frame information during simulation
 */
struct Frame {
    int bits_sent;
    int block_errors;
    int block_index; // which block within the frame is currently being txed
    bool lost;
};

/*
 * Struct to contain parameters to pass to Simulator constructor
 */
struct SimParams {
    int feedback_time;
    int num_blocks;
    int frame_size;
    double error_prob;
    int sim_length;
    int num_trials;
    vector<int> seeds;
};

class Simulator 
{

private:

    int feedback_time;
    int num_blocks;
    int frame_size;
    double error_prob;
    int sim_length;
    int num_trials;
    int block_size;
    vector<int> seeds;

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
     * Clear the fields of a Frame struct for simulating the transmission
     * of a new frame starting with 0 bits sent, 0 errors, and no blocks sent.
     ***********************************************************************/
    void new_frame( Frame &frame );

    /***********************************************************************
     * Run a single simulation trial of up to sim_length bits transmitted,
     * stopping after the last frame is transmitted before the next frame 
     * cannot be confirmed within the simulation time.
     ***********************************************************************/
    bool transmit_bit( Frame &frame );

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


public:

    /***********************************************************************
     * Constructor - initialize the simulator with the given parameter 
     * struct
     ***********************************************************************/
    Simulator( SimParams &params );
    void run();
    
};

#endif
