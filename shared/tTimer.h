#ifndef TTIMER_H
#define TTIMER_H

#include <chrono>
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

struct tTimer
{
	// Constructor starts the timer
    inline tTimer(void)
    {
        startTimer();
        return;
    }

    inline virtual ~tTimer(void) { return; }

    // Method to start the timer
    inline void startTimer(void)
    {
        startTime = high_resolution_clock::now();
        return;
    }

    // Calculates the time since the invocation of the constructor till the method call
    inline double getTime(void)
    {
        endTime = high_resolution_clock::now();
        return duration_cast<duration<double>>(endTime - startTime).count();
    }

private:
    high_resolution_clock::time_point startTime;
    high_resolution_clock::time_point endTime;
};

#endif
