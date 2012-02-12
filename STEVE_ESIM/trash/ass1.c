#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct parameters {
    int feedback_time;
    int num_blocks;
    int frame_size;
    double error_prob;
    int sim_length;
    int num_trials;
    int block_size;
    int *seeds;
};

struct frame {
    int bits_sent;
    int block_errors;
    int num_blocks;
    int block_index;
    int lost;
};

void runSimulation( int *time, 
                    int *frames, 
                    int *corr_frames, 
                    struct parameters *params, 
                    int seed );

void new_frame( struct frame *newFrame );
int tx_bit( struct frame *currentFrame, struct parameters *params );
int read_params( struct parameters *params, int argc, char **argv );
void print_params( struct parameters *params );
void get_block_size( struct parameters *params );

void print_frames_statistics( struct parameters *params, 
                              int *num_frames, 
                              int *num_corr_frames );

void print_throughput_statistics( struct parameters *params, 
                                  int *num_frames, 
                                  int time );
double get_t_value( int df );

int main( int argc, char **argv )
{
    struct parameters params;

    int valid = read_params(&params, argc, argv);
    if (!valid) {
        printf("Error: Invalid parameters!\n");
        exit(EXIT_FAILURE);
    }

    print_params(&params);

    get_block_size(&params);
    
    int *num_frames = calloc( params.num_trials, sizeof(int) );
    int *num_corr_frames = calloc( params.num_trials, sizeof(int) );
    int time = 0;
    int total_time = 0;

    /* Run T simulations of length R */
    int i;
    for (i = 0; i < params.num_trials; i++) {
        runSimulation( &time, 
                       num_frames+i, 
                       num_corr_frames+i,
                       &params, 
                       params.seeds[i] );
        total_time += time;

    }

    print_frames_statistics( &params, num_frames, num_corr_frames );
    print_throughput_statistics( &params, num_corr_frames, time );

    /* free allocated mem */
    free(params.seeds);
    free(num_frames);
    free(num_corr_frames);

    return 0;
}

int read_params( struct parameters *params, int argc, char **argv ) {

    int valid = 1;
    int i;

    if (argc > 7) {
        params->feedback_time = atoi(argv[1]);
        params->num_blocks = atoi(argv[2]);
        params->frame_size = atoi(argv[3]);
        params->error_prob = atof(argv[4]);
        params->sim_length = atoi(argv[5]);
        params->num_trials = atoi(argv[6]);

        if (argc == params->num_trials + 7 ) {
            params->seeds = calloc( params->num_trials, sizeof(int) );
            for ( i = 0; i < params->num_trials; i++ ) {
                params->seeds[i] = atoi( argv[ 7 + i ] );
            }

        } else {
            valid = 0;
        }

    } else {
        valid = 0;
    }

    return valid;

}

void print_params( struct parameters *params ) {

    printf("%d %d %d %lf %d %d", 
            params->feedback_time,
            params->num_blocks,
            params->frame_size,
            params->error_prob,
            params->sim_length,
            params->num_trials );

    int i;
    for ( i = 0; i < params->num_trials; i++ ) {
        printf(" %d", params->seeds[i] );
    }
    printf("\n");
}

void get_block_size( struct parameters *params ) {

    if (params->num_blocks > 0)  {
        params->block_size = (params->frame_size / params->num_blocks);

        // calculate number of check bits, k, to add to block size
        int k = 1;
        while ( exp2( k ) < k + params->block_size ) k++;
        params->block_size += k;

    } else  {
        /* no correction, just send frames */
        params->block_size = params->frame_size;
    }

}

void runSimulation( int *time, 
                    int *frames, 
                    int *corr_frames, 
                    struct parameters *params, 
                    int seed )
{
    srand48(seed); /* seed RNG */
    (*time) = 0;
    (*frames) = 0;
    (*corr_frames) = 0;

    struct frame currentFrame;
    new_frame( &currentFrame );

    /* transmit bits until end of trial */
    int running = 1;
    while ( running ) {

        int frameComplete = tx_bit( &currentFrame, params );
        (*time)++;

        if (frameComplete) {

            (*time) += params->feedback_time; /* 'wait' for ACK */
            
            (*frames)++;
            if (currentFrame.lost == 0)
                (*corr_frames)++; /* count frame if not lost */

            if ( params->sim_length - (*time) 
                 >= params->frame_size + params->feedback_time ) {
                new_frame( &currentFrame );
            } else {
                running = 0; /* stop sim */
            }

        }
    }
}

void new_frame( struct frame *newFrame ) {

    newFrame->bits_sent = 0;
    newFrame->block_errors = 0;
    newFrame->block_index = 0;
    newFrame->lost = 0;

}

/*
 * Transmit a bit from the frame. Return 1 if frame complete.
 */
int tx_bit( struct frame *currentFrame, struct parameters *params ) {

    int frameComplete = 0;

    currentFrame->bits_sent++;

    /* check if bit error */
    if (drand48() < params->error_prob )
        currentFrame->block_errors++;

    /* if no error correction and frame finished... */
    if (params->num_blocks == 0 
        && currentFrame->bits_sent % params->frame_size == 0 ) {

        if (currentFrame->block_errors > 0)
            currentFrame->lost = 1; /* no error correction */

        frameComplete = 1;

    } else if (currentFrame->bits_sent % params->block_size == 0) {
        /* block finished ... */

        if (currentFrame->block_errors > 1)
            currentFrame->lost = 1;

        currentFrame->block_index++;
        currentFrame->block_errors = 0;

        if (currentFrame->block_index == params->num_blocks )
            frameComplete = 1;

    }

    return frameComplete;

}

/* print average number of frame transmissions per trial followed by 
 * confidence interval */
void print_frames_statistics( struct parameters *params, 
                              int *num_frames, 
                              int *num_corr_frames )
{

    int total_frames = 0;
    int i;
    for (i = 0; i < params->num_trials; i++) {
        total_frames += num_frames[i];
    }


    double ratio[params->num_trials];
    double sum = 0;

    for (i =0; i < params->num_trials; i++) {
        ratio[i] = (double)num_frames[i] / (double)num_corr_frames[i];
        sum += ratio[i];
    }

    //double mean = (double)total_frames / (double)params->num_trials;
    double mean = sum / (double)params->num_trials;

    /*
    double mse = 0;
    for (i = 0; i < params->num_trials; i++ ) {
        double diff = (num_frames[i] - mean);
        mse += diff * diff;
    }
    */

    double mse = 0;
    for (i = 0; i < params->num_trials; i++ ) {
        double diff = (ratio[i] - mean);
        mse += diff * diff;
    }

    double stddev = sqrt( mse / (double)(params->num_trials - 1) );

    // calculate bounds upper/lower bounds of 95% confidence interval
    double tstat = get_t_value( params->num_trials - 1 );

    double error = tstat * stddev / sqrt(params->num_trials);
    double lower = mean - error;
    double upper = mean + error;

    printf("%lf %lf %lf\n", mean, lower, upper);

}

void print_throughput_statistics( struct parameters *params, 
                                  int *num_frames, 
                                  int time )
{

    int total_frames = 0;
    int i;
    for (i = 0; i < params->num_trials; i++) {
        total_frames += num_frames[i];
    }

    double mean = (double)(params->frame_size * total_frames) 
                / (double)(time * params->num_trials);

    double mse = 0;
    for (i = 0; i < params->num_trials; i++ ) {
        double diff = (double)num_frames[i] * (double)params->frame_size 
                    / (double)time - mean;
        mse += diff * diff;
    }

    double stddev = sqrt( mse / (double)(params->num_trials - 1) );

    // calculate bounds upper/lower bounds of 95% confidence interval
    double tstat = get_t_value( params->num_trials - 1 );

    double error = tstat * stddev / sqrt(params->num_trials);
    double lower = mean - error;
    double upper = mean + error;

    printf("%lf %lf %lf\n", mean, lower, upper);


}

/*
 * Open file containing a range of t values for calculating 95% CI's, and return
 * an appropriate t-value for the given degrees of freedom
 */
double get_t_value( int df )
{
    double t;

    FILE *tfile = fopen("tvalues","r");

    int i = 1;
    while ( i <= df && i <= 30 ) {
        fscanf(tfile, "%lf", &t);
        i++;
    }

    /* file only stores every 2 values from 30 - 50 */
    while ( i <= df && i <= 50 ) {
        fscanf(tfile, "%lf", &t);
        i+=2;
    }

    /* for anything larger, just use 1.972 */
    if ( df > i )
        t = 1.972;

    fclose(tfile);

    return t;
}
