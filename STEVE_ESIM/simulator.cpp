#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#include "simulator.h"

/***********************************************************************
 * Constructor - initialize the simulator with the given parameter 
 * struct
 ***********************************************************************/
Simulator::Simulator( SimParams &params )
{
    feedback_time = params.feedback_time;
    num_blocks = params.num_blocks;
    frame_size = params.frame_size;
    error_prob = params.error_prob;
    sim_length = params.sim_length;
    num_trials = params.num_trials;

    block_size = calc_block_size();

    seeds = params.seeds;

}

/***********************************************************************
 * Calculate the block size required for the simulation's frame size
 * and number of correction blocks. If num_blocks = 0, assume no error 
 * correction bits so block_size = frame_size
 ***********************************************************************/
int Simulator::calc_block_size()
{

    int block_size = 0;

    if (num_blocks > 0)  {
        block_size = frame_size /num_blocks;

        // calculate number of check bits, k, to add to block size
        int k = 1;
        while ( exp2( k ) < k + block_size ) k++;
        block_size += k;

    } else  {
        /* no correction, just send frames */
        block_size = frame_size;
    }

    return block_size;
    
}

/***********************************************************************
 * Run num_trials trials of the simulation, then calculate and print
 * result statstics.
 ***********************************************************************/
void Simulator::run()
{

    /* Run T simulations of length R */
    int total_time = 0;

    vector<int> total_frames;
    vector<int> correct_frames;

    int num_frames, num_corr_frames, time;

    int i;
    for (i = 0; i < num_trials; i++) {

        run_trial( time,
                   num_frames, 
                   num_corr_frames,
                   seeds[i] );

        total_frames.push_back( num_frames );
        correct_frames.push_back( num_corr_frames );
        total_time += time;

    }

    print_frame_statistics( total_frames, correct_frames );
    print_throughput_statistics( correct_frames, time );

}

/***********************************************************************
 * Run a single simulation trial of up to sim_length bits transmitted,
 * stopping after the last frame is transmitted before the next frame 
 * cannot be confirmed within the simulation time.
 ***********************************************************************/
void Simulator::run_trial( int &time, int &frames, int &corr_frames, int seed )
{

    srand48(seed); /* seed RNG */
    time = 0;
    frames = 0;
    corr_frames = 0;

    Frame current_frame;
    new_frame(current_frame);

    /* transmit bits until end of trial */
    bool running = true;
    while ( running ) {

        bool frame_complete = transmit_bit( current_frame ); 
        time++;

        if (frame_complete) {

            time += feedback_time; /* 'wait' for ACK */
            
            frames++;
            if (current_frame.lost == false)
                corr_frames++; /* count frame if not lost */

            if ( sim_length - time >= frame_size + feedback_time ) {
                new_frame(current_frame);
            } else {
                running = false; /* stop sim */
            }
        }
    }

}

/***********************************************************************
 * Clear the fields of a Frame struct for simulating the transmission
 * of a new frame starting with 0 bits sent, 0 errors, and no blocks sent.
 ***********************************************************************/
void Simulator::new_frame( Frame &frame )
{
    frame.bits_sent = 0;
    frame.block_errors = 0;
    frame.block_index = 0;
    frame.lost = false;
}

/***********************************************************************
 * Simulate the transmission of a single bit, returning true if the 
 * a full frame is finished transmission after bit transmission. 
 * Bit will be in error with probability as per simulation parameter
 ***********************************************************************/
bool Simulator::transmit_bit( Frame &frame )
{
    bool frame_complete = false;

    frame.bits_sent++;

    /* check if bit error */
    if (drand48() < error_prob )
        frame.block_errors++;

    /* if no error correction and frame finished... */
    if (num_blocks == 0 && frame.bits_sent % frame_size == 0 ) {

        if (frame.block_errors > 0)
            frame.lost = true; /* no error correction */

        frame_complete = true;

    } else if (frame.bits_sent % block_size == 0) {
        /* block finished ... */

        if (frame.block_errors > 1)
            frame.lost = true;

        frame.block_index++;
        frame.block_errors = 0;

        if (frame.block_index == num_blocks )
            frame_complete = true;

    }

    return frame_complete;


}

/***********************************************************************
 * Output mean and confidence interval for average total frames sent
 * per correct frame
 ***********************************************************************/
void Simulator::print_frame_statistics( const vector<int> &all_frames, 
                                      const vector<int> &corr_frames )
{

    vector<double> ratio;
    double sum = 0;

    for (int i = 0; i < num_trials; i++) {
        ratio.push_back( (double)all_frames[i] / (double)corr_frames[i] );
        sum += ratio[i];
    }

    double mean = sum / (double)num_trials;

    double mse = 0;
    for (int i = 0; i < num_trials; i++ ) {
        double diff = (ratio[i] - mean);
        mse += diff * diff;
    }

    print_ci( mean, mse );

}

/***********************************************************************
 * Output mean and confidence interval for bit throughput
 ***********************************************************************/
void Simulator::print_throughput_statistics( const vector<int> &corr_frames,
                                             int time )
{

    int total_frames = 0;

    for (int i = 0; i < num_trials; i++) {
        total_frames += corr_frames[i];
    }

    double mean = (double)( frame_size * total_frames ) 
                / (double)( time * num_trials );

    double mse = 0;
    for (int i = 0; i < num_trials; i++ ) {
        double diff = (double)corr_frames[i] * (double)frame_size
                    / (double)time - mean;
        mse += diff * diff;
    }

    print_ci( mean, mse );

}

/***********************************************************************
 * Output the mean, and lower and upper bounds of 95% confidence interval 
 * with given mean and mean squared error.
 ***********************************************************************/
void Simulator::print_ci( double mean, double mse )
{

    double stddev = sqrt( mse / (double)(num_trials - 1) );

    // calculate bounds upper/lower bounds of 95% confidence interval
    double tstat = get_t_value( num_trials - 1 );

    double error = tstat * stddev / sqrt(num_trials);
    double lower = mean - error;
    double upper = mean + error;

    cout << mean << " " << lower << " " << upper << endl;

}

/***********************************************************************
 * Open file containing a range of t values for calculating 95% CI's, and 
 * return an appropriate t-value for the given degrees of freedom
 ***********************************************************************/
double Simulator::get_t_value( int df )
{
    double t;

    ifstream tfile;
    tfile.open( "tvalues.dat" );

    if ( tfile.is_open() ) {

        int i = 1;

        while ( i <= df && i <= 30 ) {
            tfile >> t;
            i++;
        }

        /* file only stores every 2 values from 30 - 50 */
        while ( i <= df && i <= 50 ) {
            tfile >> t;
            i+=2;
        }

        /* for anything larger, just use 1.972 */
        if ( df > i )
            t = 1.972;
        
        tfile.close();

    } else  {

        cerr << "Could open t-statistic file: tvalues.dat. ";
        cerr << "Using t=1.972." << endl;
        t = 1.972;

    }

    return t;

}
