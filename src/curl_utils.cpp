#include "curl_utils.hpp"
#include <curl/curl.h>
#include <curl/easy.h>

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata) {
    Spot ** d_pSpot = static_cast<Spot **>(userdata);

    size_t realsize = size * nmemb;

    // return content size
    return size * nmemb;
}

Spot * getLiveSpot(std::string symbol) {
    std::string key;
    std::fstream file;

    file.open("./APIkey.txt");
    if(file.is_open()) {
        getline(file, key);
        file.close();
    }else {
        std::cout << "Error opening api key file ...\n";
        return nullptr;
    }

    std::string url = "https://finnhub.io/api/v1/quote?symbol="
        + symbol
        + "&token="
        + key;

    CURL * curl = curl_easy_init();
    CURLcode res;

    Spot ** d_pSpot = (Spot **) malloc(sizeof(Spot *));

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, d_pSpot);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_preform() returned %s", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();


}
