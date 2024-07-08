#ifndef CURL_UTILS_H_
#define CURL_UTILS_H_

#include "Spot.hpp"
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <stdio.h>

struct Storage {
    char * memory;
    size_t size;
};


static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

Storage * downloadHistoricalData(std::string symbol, std::time_t from, std::time_t to,
                                 std::string interval, std::string APIkey);

#endif // CURL_UTILS_H_
