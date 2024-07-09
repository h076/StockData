#include "time_utils.hpp"
#include <ctime>

std::time_t currentEpoch() {
    return std::time(NULL);
}

// date format YYYY-MM-DD
std::time_t dateToEpoch(const char* date) {
    char year[5] = {'\0'};
    std::strncpy(year, date, 4);

    char month[3] = {'\0'};
    std::strncpy(month, date + 5, 2);

    char day[3] = {'\0'};
    std::strncpy(day, date + 8, 2);

    struct std::tm t;
    memset(&t, 0, sizeof(t));

    t.tm_year = atoi(year) - 1900;
    t.tm_mon = atoi(month) - 1;
    t.tm_mday = atoi(day);

    time_t epoch = mktime(&t);
    return epoch;
}

std::string epochToDate(const std::time_t epoch) {
    char buf[20] = {'\0'};
    struct std::tm *t = gmtime(&epoch);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    std::string rtn(buf);
    return buf;
}
