#include <fstream>
#include <iostream>
#include <cmath>
#include "conf_int.h"

using namespace std;

// for printing easily with cout
std::ostream& operator << (std::ostream& output, const conf_int& ci)
{
    output << ci.mean << " " << ci.lower << " " << ci.upper;
    return output;
}

/***********************************************************************
 * Output the mean, and lower and upper bounds of 95% confidence interval 
 * with given mean and mean squared error.
 ***********************************************************************/
struct conf_int calc_ci( double mean, double mse, int num_trials)
{

    conf_int ci;

    ci.mean = mean;

    double stddev = sqrt( mse / (double)(num_trials - 1) );

    // calculate bounds upper/lower bounds of 95% confidence interval
    double tstat = get_t_value( num_trials - 1 );

    double error = tstat * stddev / sqrt(num_trials);
    ci.lower = mean - error;
    ci.upper = mean + error;

    return ci;
}

/***********************************************************************
 * Open file containing a range of t values for calculating 95% CI's, and 
 * return an appropriate t-value for the given degrees of freedom
 ***********************************************************************/
double get_t_value( int df )
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

        cerr << "Couldn't open t-statistic file: tvalues.dat. ";
        cerr << "Using t=1.972." << endl;
        t = 1.972;

    }

    return t;

}
