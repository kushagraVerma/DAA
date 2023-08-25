#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

using namespace std::chrono;

/**
 * A class to define a timer object.
*/
class Timer{
    private:
        time_point<high_resolution_clock> start, stop; ///< start and stop timepoints
        microseconds duration; ///< total microsecond duration
    public:
        /**
         * Constructor for Timer class, initializes start, stop and duration
        */
        Timer(){
            start = high_resolution_clock::now();
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
        }

        /**
         * Function to restart the timer
        */
        void restartClock(){
            start = high_resolution_clock::now();
            stop = high_resolution_clock::now();
        }

        /**
         * Function to stop the timer and add duration
        */
        void stopClock(){
            stop = high_resolution_clock::now();
            duration += duration_cast<microseconds>(stop - start);
        }

        /**
         * Function to get total timer duration
        */
        int64_t getDuration(){
            return duration.count();
        }

        /**
         * Overloaded operator for "=" operation
        */
        Timer operator=(Timer timer){
            start = timer.start;
            stop = timer.stop;
            return timer;
        }
};

#endif