#ifndef CONF_INT_H
#define CONF_INT_H

#include <ostream>

// Struct to contain, print a confidence interval
struct conf_int {

    double mean;
    double upper;
    double lower;

};

struct conf_int calc_ci( double mean, double mse, int num_trials );
double get_t_value( int df );

// for printing easily with cout
std::ostream& operator << (std::ostream& output, const conf_int& ci);

#endif
