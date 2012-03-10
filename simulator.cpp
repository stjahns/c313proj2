#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#include "simulator.h"

Simulator::Simulator( params &p )
{

    this->num_stations = p.num_stations;
    this->gen_prob = p.gen_prob;
    this->num_slots = p.num_slots;
    this->num_trials = p.num_trials;
    this->seeds = p.seeds;

}

/***********************************************************************
 * Run num_trials trials of the simulation, then calculate and print
 * result statstics.
 ***********************************************************************/
void Simulator::run()
{

    cout << "RUNNING SIM..." << endl;

    /* Run T simulations of length R */
#if 0
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
#endif

}

#if 0
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
#endif
