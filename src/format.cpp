#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string res;
    int hours = seconds / 3600;
    int mins = ( seconds - (3600 * hours)  ) / 60;
    int secs = seconds - 3600 * hours - 60 * mins;

    res = to_string(hours) + ":" + to_string(mins) + ":" + to_string(secs);

    return res;
}