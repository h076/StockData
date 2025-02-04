#ifndef TIME_UTILS_H_
#define TIME_UTILS_H_

#include <string>
#include <ctime>

namespace timeUtils {

  std::time_t currentEpoch();

  std::time_t dateToEpoch(const char* date);

  std::string epochToDate(const std::time_t epoch);

  bool preceedDate(const std::time_t check, const std::time_t against, std::string interval, int multiplier);

  bool preceedDate(const char* check, const char* against, std::string interval, int multiplier);

}

#endif // TIME_UTILS_H_
