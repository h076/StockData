#include "curl_utils.hpp"
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <sstream>

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata) {

    size_t realsize = size * nmemb;
    Storage * mem = static_cast<Storage *>(userdata);

    char * ptr = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == nullptr)
        return 0;

    mem->memory = ptr;
    memcpy(mem->memory + mem->size, contents, realsize);
    mem->size += realsize;

    // zero terminate end of memory
    mem->memory[mem->size] = '\0';

    // return content size
    return realsize;
}

Storage * downloadHistoricalData(std::string symbol,
                         std::time_t from,
                         std::time_t to,
                         std::string interval,
                         std::string APIkey) {
    /**
     * example url :
     * https://api.polygon.io/v2/aggs/ticker/AAPL/range/1/month/2023-01-09/1720209601000000?adjusted=true&sort=asc&apiKey=U1s5uQq9FLg4f6IyYpMF8zj4_YZOaYyn
     */

    // add 3 0's to each time_t to make them millisecond timestamps
    std::stringstream sst1;
    sst1 << from << "000";
    std::stringstream sst2;
    sst2 << to << "000";
    // temporarily this woudl only return in monthluy intervals
    std::string url = "https://api.polygon.io/v2/aggs/ticker/"
        + symbol
        + "/range/1/month/"
        + sst1.str()
        + "/"
        + sst2.str()
        + "?adjusted=true&sort=asc&apiKey="
        + APIkey;

    CURL * curl = curl_easy_init();
    CURLcode res;

    Storage * chunk = (Storage *) malloc(sizeof(Storage));
    chunk->memory = nullptr;
    chunk->size = 0;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, chunk);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_preform() returned %s", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return chunk;
}
