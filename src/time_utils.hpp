#ifndef TIME_UTILS_H_
#define TIME_UTILS_H_

#include <string>
#include <ctime>

std::time_t currentEpoch();

std::time_t dateToEpoch(const char* date);

std::string epochToDate(const std::time_t epoch);

#endif // TIME_UTILS_H_
