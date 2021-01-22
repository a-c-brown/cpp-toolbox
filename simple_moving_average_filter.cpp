#include <stdio.h>
#include <time.h>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;

/* Simple moving average filter */

// GLOBAL VARS

// tracks the number of times moving average is invoked - will be reset periodically to prevent overflow
unsigned char gModCallNum = 0;
// the upper bound of the sample array - given in requirements elicitation
const unsigned char gMaxSamples = 64;
// holds input signals
double gSamples[gMaxSamples];
// the sum of all samples in gSamples at any point in time
double gSum = 0;

// FUNCTION PROTOTYPES

// return average of past n samples where n == 64
double filter(double);
// return average of past n samples where 1 <= n <= 63
double initFilter(double);

int main()
{
    // initialize random seed
    srand(time(NULL));

    while (true)
    {
        // get a random signal between 0 and 100
        double sig = (double)rand() / RAND_MAX * (100);
        // pass signal to moving average filter
        double ave = filter(sig);
        // print signal and moving average
        printf("Signal %.2f :: Average %.2f\n", sig, ave);
        // sleep for 12 ms between samplings to emulate processing conditions
        this_thread::sleep_for(chrono::milliseconds(12));
    }
    return 0;
}

double filter(double sig)
{
    double ave = 0;
    double sample = 0;
    unsigned char index = 0;

    if (gModCallNum < gMaxSamples)
    {
        ave = initFilter(sig);
    }
    else
    {
        // get the position of the oldest sample
        index = gModCallNum % gMaxSamples;
        // get the value of the oldest sample
        sample = gSamples[index];
        // overwrite oldest sample with signal
        gSamples[index] = sig;
        // subtract oldest sample from sum and add signal
        gSum = gSum - sample + sig;
        // get new average
        ave = gSum / gMaxSamples;
        // prevent call num from overflowing
        if (index == 0)
        {
            gModCallNum = gMaxSamples;
        }
    }
    // increment call number
    gModCallNum++;
    return ave;
}

double initFilter(double sig)
{
    double ave;
    if (gModCallNum == 0)
    {
        // set sum to first signal
        gSum = sig;
        // set average to first signal
        ave = sig;
    }
    else if (gModCallNum < gMaxSamples)
    {
        // add signal to sum
        gSum += sig;
        // gModCallNum begins at zero so add one to divisor
        ave = gSum / (gModCallNum + 1);
    }
    // store signal to sample array
    gSamples[gModCallNum] = sig;
    return ave;
}