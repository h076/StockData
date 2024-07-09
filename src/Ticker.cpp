#include "Ticker.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"
#include <cstdio>

Ticker::Ticker(std::string symbol) {
    m_sSymbol = symbol;
    loadAPIKey();
}

void Ticker::loadAPIKey() {
    std::ifstream file ("APIkey.txt");
    if(file.is_open()) {
        getline(file, m_sAPIKey);
        file.close();
    }else {
        fprintf(stderr, "API key file could not be opened.\n");
        return;
    }
}
// add and check intervals
void Ticker::loadHistoricalSpots(std::string from, std::string to) {
    loadHistoricalSpots(dateToEpoch(from.c_str()), dateToEpoch(to.c_str()));
}

void Ticker::loadHistoricalSpots(std::time_t from, std::time_t to) {

    Storage * returnData = downloadHistoricalData(m_sSymbol, from, to, "month", m_sAPIKey);
    std::cout << returnData->memory << std::endl;
}
