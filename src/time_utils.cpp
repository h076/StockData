#include "time_utils.hpp"
#include "spdlog/spdlog.h"
#include <ctime>
#include <iostream>

std::time_t timeUtils::currentEpoch() {
    return std::time(NULL);
}

// date format YYYY-MM-DD
std::time_t timeUtils::dateToEpoch(const char* date) {
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

std::string timeUtils::epochToDate(const std::time_t epoch) {
    char buf[20] = {'\0'};
    struct std::tm *t = gmtime(&epoch);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    std::string rtn(buf);
    return buf;
}

bool timeUtils::preceedDate(const std::time_t check, const std::time_t against, std::string interval, int multiplier) {
    struct tm * time = localtime(&check);
    int partial = 0;

    if(interval == "minute") {
        time->tm_min += multiplier;
        partial += multiplier * 60;
    }else if(interval == "hour") {
        time->tm_hour += multiplier;
        partial += multiplier * 3600;
    }else if(interval == "day") {
        time->tm_mday += multiplier;
        partial += multiplier * 86400;
    }else if(interval == "month") {
        time->tm_mon += multiplier;
        partial += multiplier * 2678400;
    }else if(interval == "year") {
        time->tm_year += multiplier;
        partial += multiplier * 32140800;
    }else {
        spdlog::error("timeUtils : Cannot handle date check, invalid interval given");
        return false;
    }

    std::time_t newTime = mktime(time);
    double diff = difftime(against, newTime);

    if(diff > partial || diff > 0)
        return true;
    return false;
}

bool timeUtils::preceedDate(const char* check, const char* against, std::string interval, int multiplier) {
    return timeUtils::preceedDate(timeUtils::dateToEpoch(check), timeUtils::dateToEpoch(against), interval, multiplier);
}
