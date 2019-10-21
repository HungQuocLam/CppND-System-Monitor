#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long format_minutes = seconds / 60;
    long format_hours = format_minutes / 60;
    long format_seconds = int(seconds % 60);
    format_minutes = int(format_minutes % 60);
    std::string result = ((format_hours < 10)?("0"+std::to_string(format_hours)):std::to_string(format_hours)) + ":" +
                         ((format_minutes < 10)?("0"+std::to_string(format_minutes)):std::to_string(format_minutes)) + ":" +
                         ((format_seconds < 10)?("0"+std::to_string(format_seconds)):std::to_string(format_seconds));
  return result;
}