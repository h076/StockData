#ifndef CURL_UTILS_H_
#define CURL_UTILS_H_

#include "Spot.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <stdio.h>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

Spot * getLiveSpot(std::string symbol);

#endif // CURL_UTILS_H_
