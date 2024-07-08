#include "time_utils.hpp"
#include <ctime>

std::time_t currentEpoch() {
    return std::time(NULL);
}

std::time_t dateToEpoch(const char* date) {
    char year[5] = {'\0'};
    std::strncpy(year, date, 4);

    char month[3] = {'\0'};
    std::strncpy(month, date + 5, 2);

    char day[3] = {'\0'};
    std::strncpy(day, date + 8, 2);

    char hour[3] = {'\0'};
    std::strncpy(hour, date + 11, 2);

    char minute[3] = {'\0'};
    std::strncpy(minute, date + 13, 2);

    char second[3] = {'\0'};
    std::strncpy(second, date + 15, 2);

    struct std::tm t;
    memset(&t, 0, sizeof(t));

    t.tm_year = atoi(year) - 1900;
    t.tm_mon = atoi(month) - 1;
    t.tm_mday = atoi(day);
    t.tm_hour = atoi(hour);
    t.tm_min = atoi(minute);
    t.tm_sec = atoi(second);

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
