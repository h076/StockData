#include "Ticker.hpp"
#include "Sample.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"
#include <__config>
#include <cstdio>

Ticker::Ticker(std::string symbol) {
    m_sSymbol = symbol;
    loadAPIKey();
    setInterval(MONTH);
}

Ticker::~Ticker() {
    clearSpots();
    clearSamples();
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

    if(m_oSpots.size() != 0)
        clearSpots();

    Storage * returnData = downloadHistoricalData(m_sSymbol, from, to, m_sInterval, m_sAPIKey);

    char * memPtr = returnData->memory;
    char * endPtr = returnData->memory+(returnData->size - 1);
    if(memPtr == nullptr) {
        std::cout << "No data retuned so no spots loaded ...\n";
        delete returnData;
        return;
    }

    // Safely find start
    while(memPtr < endPtr && *memPtr != '[') {
        memPtr++;
    }
    memPtr++;

    if(memPtr >= endPtr) {
        std::cout << "Invalid data format - no opening '[' found\n";
        delete returnData;
        return;
    }
    memPtr++;

    //char * endPtr = returnData->memory+(returnData->size - 1);
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

    delete returnData;
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
    for(std::vector<Spot *>::iterator it = m_oSpots.begin(); it != m_oSpots.end(); it++) {
        (*it)->printSpot();
    }
}

void Ticker::clearSpots() {
    for(std::vector<Spot *>::iterator it = m_oSpots.begin(); it != m_oSpots.end(); it++) {
        free(*it);
    }
    m_oSpots.clear();
}

void Ticker::clearSamples() {
    for(std::vector<Sample *>::iterator it = m_oSamples.begin(); it != m_oSamples.end(); it++) {
        delete *it;
    }
    m_oSamples.clear();
}

double * Ticker::getClosePriceArr(int * arrSize) {
    if(m_oSpots.empty()) {
        return nullptr;
    }

    *arrSize = m_oSpots.size();
    double* priceArr = (double*) malloc((*arrSize)*sizeof(double));

    for(int i=0; i<*arrSize; i++) {
        *(priceArr+i) = m_oSpots[i]->getClose();
    }

    return priceArr;
}

double * Ticker::getHighPriceArr(int * arrSize) {
    if(m_oSpots.empty()) {
        return nullptr;
    }

    *arrSize = m_oSpots.size();
    double* priceArr = (double*) malloc((*arrSize)*sizeof(double));

    for(int i=0; i<*arrSize; i++) {
        *(priceArr+i) = m_oSpots[i]->getHigh();
    }

    return priceArr;
}

double * Ticker::getLowPriceArr(int * arrSize) {
    if(m_oSpots.empty()) {
        return nullptr;
    }

    *arrSize = m_oSpots.size();
    double* priceArr = (double*) malloc((*arrSize)*sizeof(double));

    for(int i=0; i<*arrSize; i++) {
        *(priceArr+i) = m_oSpots[i]->getLow();
    }

    return priceArr;
}

void Ticker::getPriceSamples(std::string from, std::string to, Interval interval,
                                      int sampleCount, int sampleLength, int trainSplit) {
    setInterval(interval);
    loadHistoricalSpots(from, to);

    m_oSamples.clear();

    // for efficiency loop once in this function to get priceArrs
    if(m_oSpots.empty()) {
        std::cout << "No spots to derive prices from...\n";
        return;
    }

    int arrSize = m_oSpots.size();
    double* closeArr = (double*) malloc(arrSize*sizeof(double));
    double* highArr = (double*) malloc(arrSize*sizeof(double));
    double* lowArr = (double*) malloc(arrSize*sizeof(double));

    for(int i=0; i<arrSize; i++) {
        *(closeArr+i) = m_oSpots[i]->getClose();
        *(highArr+i) = m_oSpots[i]->getHigh();
        *(lowArr+i) = m_oSpots[i]->getLow();
    }

    std::srand(time(0));
    std::vector<int> startPoints(sampleCount);
    std::generate(startPoints.begin(), startPoints.end(), [&] () mutable {
        return rand()%(arrSize-sampleLength-1);
    });

    // take samples from data
    int start;
    double * closeSampleData;
    double * highSampleData;
    double * lowSampleData;
    for(int i=0; i<sampleCount; i++) {
        start = startPoints[i];

        closeSampleData = (double *) malloc(sampleLength*sizeof(double));
        highSampleData = (double *) malloc(sampleLength*sizeof(double));
        lowSampleData = (double *) malloc(sampleLength*sizeof(double));

        std::memcpy(closeSampleData, closeArr+start, sizeof(double)*sampleLength);
        std::memcpy(highSampleData, highArr+start, sizeof(double)*sampleLength);
        std::memcpy(lowSampleData, lowArr+start, sizeof(double)*sampleLength);

        m_oSamples.push_back(new Sample(closeSampleData, highSampleData, lowSampleData, sampleLength, trainSplit));
    }

    free(closeArr);
    free(highArr);
    free(lowArr);
}

void Ticker::displaySamples() {
    if(m_oSamples.empty()) {
        std::cout << "no samples to display ...\n";
        return;
    }

    int i=1;
    for(auto & s : m_oSamples) {
        std::cout << "Sample " << i++ << ": ";
        s->show();
    }
}

void Ticker::displaySamplesFeatures() {
    if(m_oSamples.empty()) {
        std::cout << "no samples to display features of ...\n";
        return;
    }

    int i=1;
    for(auto & s : m_oSamples) {
        std::cout << "Sample " << i++ << " Features ... \n";
        std::cout << "RSI = " << s->getRSI() << ", ";
        std::cout << "MACD = " << s->getMACD() << ", ";
        std::cout << "Close = " << s->getClose() << ", ";
        std::cout << "Stochastic Fast K = " << s->getStochFastK() << ", ";
        std::cout << "Stochastic Fast D = " << s->getStochFastD() << ", ";
        std::cout << "Williams %R = " << s->getWilliamsR() << ", ";
        std::cout << "Ultimate Oscilator = " << s->getUltimateOscillator() << ", ";
        std::cout << "Time series forecast = " << s->getTSF() << ", ";
        std::cout << "Double exponential MA = " << s->getDEMA() << ", ";
        std::cout << "Commodity channel index = " << s->getCCI() << ", ";
        std::cout << "One day ROC triple smooth EMA = " << s->getTRIX();
        std::cout << std::endl;
    }
}

void Ticker::saveSamplesCSV() {
    std::ofstream file;
    file.open("Samples.csv");
    file << m_oSamples[0]->toCSVHeader();
    for(auto & s : m_oSamples) {
        file << s->toCSVLine();
    }
    file.close();
    return;
}
