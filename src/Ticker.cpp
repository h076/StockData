#include "Ticker.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"
#include <cstdio>

Ticker::Ticker(std::string symbol) {
    m_sSymbol = symbol;
    loadAPIKey();
    setInterval(MONTH);
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

    Storage * returnData = downloadHistoricalData(m_sSymbol, from, to, m_sInterval, m_sAPIKey);

    char * memPtr = returnData->memory;
    while(*memPtr != '[') {
        memPtr++;
    }
    memPtr++;

    char * endPtr = returnData->memory+(returnData->size - 1);
    while(*endPtr != ']') {
        endPtr--;
    }

    char timeBuffer[14];
    std::time_t time;

    char openBuffer[10];
    char closeBuffer[10];
    char highBuffer[10];
    char lowBuffer[10];

    int valueSize = 0;

    for(char currentChar; memPtr != endPtr; memPtr++) {
        currentChar = *memPtr;
        if(currentChar == 'o') {
            memPtr = memPtr + 3;
            for(valueSize = 0; *(memPtr+valueSize) != ','; valueSize++) {
                continue;
            }
            strncpy(openBuffer, memPtr, valueSize);
            *(openBuffer+valueSize) = '\0';
        }else if(currentChar == 'c') {
            memPtr = memPtr + 3;
            for(valueSize = 0; *(memPtr+valueSize) != ','; valueSize++) {
                continue;
            }
            strncpy(closeBuffer, memPtr, valueSize);
            *(closeBuffer+valueSize) = '\0';
        }else if(currentChar == 'h') {
            memPtr = memPtr + 3;
            for(valueSize = 0; *(memPtr+valueSize) != ','; valueSize++) {
                continue;
            }
            strncpy(highBuffer, memPtr, valueSize);
            *(highBuffer+valueSize) = '\0';
        }else if(currentChar == 'l') {
            memPtr = memPtr + 3;
            for(valueSize = 0; *(memPtr+valueSize) != ','; valueSize++) {
                continue;
            }
            strncpy(lowBuffer, memPtr, valueSize);
            *(lowBuffer+valueSize) = '\0';
        }else if(currentChar == 't') {
            memPtr = memPtr + 3;
            for(valueSize = 0; *(memPtr+valueSize) != ','; valueSize++) {
                continue;
            }
            strncpy(timeBuffer, memPtr, valueSize);
            *(timeBuffer+valueSize-3) = '\0';
            time = atol(timeBuffer);
        }else if(currentChar == '}') {
            m_oSpots.push_back(new Spot(time, atof(openBuffer), atof(highBuffer), atof(lowBuffer), atof(closeBuffer)));
        }
    }
}

void Ticker::setInterval(Interval interval) {
    switch(interval) {
        case MINUTE:
            m_sInterval = "minute";
            break;
        case HOUR:
            m_sInterval = "hour";
            break;
        case DAY:
            m_sInterval = "day";
            break;
        case MONTH:
            m_sInterval = "month";
            break;
        case YEAR:
            m_sInterval = "year";
            break;
        default:
            m_sInterval = "month";
            break;
    }
}

void Ticker::displaySpots() {
    for(auto it = m_oSpots.begin(); it != m_oSpots.end(); it++) {
        (*it)->printSpot();
    }
}
