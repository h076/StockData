#include "Ticker.hpp"
#include "Sample.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"
#include <__config>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <time.h>
#include <unordered_set>
#include <boost/tokenizer.hpp>

Ticker::Ticker(std::string symbol) {
    spdlog::info ("Initialising {}", symbol);
    m_sSymbol = symbol;
    loadAPIKey();
    setInterval(MONTH);
    setMultiplier(1);
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
    loadHistoricalSpots(timeUtils::dateToEpoch(from.c_str()), timeUtils::dateToEpoch(to.c_str()), false);
}

void Ticker::loadHistoricalSpots(std::time_t from, std::time_t to, bool recursiveCall) {

    if(m_oSpots.size() != 0 && !recursiveCall)
        clearSpots();

    Storage * returnData = downloadHistoricalData(m_sSymbol, from, to, m_sInterval, m_sAPIKey, getMultiplier());

    char * memPtr = returnData->memory;
    char * endPtr = returnData->memory+(returnData->size - 1);
    if(memPtr == nullptr) {
        spdlog::error("No data retuned so no spots loaded.");
        delete returnData;
        return;
    }

    // Safely find start
    while(memPtr < endPtr && *memPtr != '[') {
        memPtr++;
    }
    memPtr++;
    if(memPtr >= endPtr) {
        spdlog::error("Invalid data format - no opening '[' found");
        spdlog::info ("data : {}", returnData->memory);

        std::string error(returnData->memory);
        delete returnData;

        // most likely that too many requests have been made
        if(error.substr(11, 5) == "ERROR") {
            spdlog::info("Sleeping for 1 minute to avoid maximum request error.");
            std::this_thread::sleep_for(std::chrono::minutes(1));
            if(m_oSpots.empty()) {
                loadHistoricalSpots(from, to, true);
                spdlog::info("Loaded historical spots from {} to {}", ctime (&from), ctime (&to));
                return;
            }

            std::time_t last_date = m_oSpots.back()->getEpochDate();
            if(timeUtils::preceedDate(last_date, to, m_sInterval, getMultiplier())) {
                loadHistoricalSpots(last_date, to, true);
                m_oSpots.pop_back(); // prevent double load of last spot
                                     //
                spdlog::info("Loaded historical spots from {} to {}", ctime (&from), ctime (&to));
            }
        }
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

    // Add check that the time of the last given spot is not before the 'to' date
    std::time_t last_date = m_oSpots.back()->getEpochDate();
    if(timeUtils::preceedDate(last_date, to, m_sInterval, getMultiplier())) {
        loadHistoricalSpots(last_date, to, true);
        m_oSpots.pop_back(); // prevent double load of last spot
    }

    //displaySpots();
    //std::cout << "number of spots : " << m_oSpots.size() << std::endl;

    spdlog::info("Loaded historical spots from {} to {}", ctime (&from), ctime (&to));
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
            throw Ticker::TickerException("Invalid interval, interval has been set to month.");
    }
    spdlog::info("Interval set to {}", m_sInterval);
}

void Ticker::setMultiplier(int m) {
    if(m > 60 || m < 1) {
        spdlog::error("Invalid interval multiplier");
        return;
    }
    m_nMultiplier = m;
    spdlog::info("Interval multiplier set to {}", m);
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
    if(sampleLength < 48) {
        spdlog::error("getPriceSamples: SampleLength of {} is not greater than or equal to 48\n", sampleLength);
        return;
    }

    try {
        setInterval(interval);
    }catch (const std::exception& e) {
        spdlog::error("setInterval: {}", e.what());
        return;
    }

    loadHistoricalSpots(from, to);

    m_oSamples.clear();

    if(m_oSpots.empty()) {
        spdlog::error("getPriceSamples: No spots to derive prices from");
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
    //std::generate(startPoints.begin(), startPoints.end(), [&] () mutable {
      //  return rand()%(arrSize-sampleLength-1);
    //});

    // generate points manually to ensure no repeating samples
    std::unordered_set<int> startSeen;
    int randStart;
    for(auto it = startPoints.begin(); it != startPoints.end(); it++) {
        randStart = rand()%(arrSize-sampleLength-1);
        while(startSeen.count(randStart))
            randStart = rand()%(arrSize-sampleLength-1);
        startSeen.insert(randStart);
        *it = randStart;
    }

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

    std::time_t tFrom = timeUtils::dateToEpoch(from.c_str());
    std::time_t tTo = timeUtils::dateToEpoch(to.c_str());

    spdlog::info("Loaded {} samples of length {} from {} to {}",
                 sampleCount, sampleLength, ctime(&tFrom), ctime(&tTo));
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
    if(m_oSamples.empty()) {
        spdlog::error("Ticker::saveSamplesCSV : No samples present, cannot save to csv.");
        return;
    }

    std::ofstream file;
    file.open("Samples.csv");
    file << m_oSamples[0]->toCSVHeader();
    file << m_oSamples[0]->minRangeToCSV();
    file << m_oSamples[0]->maxRangeToCSV();
    for(auto & s : m_oSamples) {
        file << s->toCSVLine();
    }
    file.close();
    return;
}

bool Ticker::saveSamplesTo(std::ofstream& file) {
    if(m_oSamples.empty()) {
        spdlog::error("Ticker::saveSamplesTo : No samples present, cannot save to csv.");
        return false;
    }

    if(!file.is_open()) {
        file.open("Samples.csv");
        file << m_oSamples[0]->toCSVHeader();
        file << m_oSamples[0]->minRangeToCSV();
        file << m_oSamples[0]->maxRangeToCSV();
    }

    if(file.is_open()) {
        for(auto & s : m_oSamples) {
            file << s->toCSVLine();
        }
    }else {
        spdlog::error("Ticker::saveSamplesTo : File not opened, nothing saved");
    }

    return true;
}

void TickerUtil::addSampleRanges() {
    std::ifstream file ("Samples.csv");

    if(!file.is_open()) {
        spdlog::error("TickerUtil::addSampleRanges : Cannot open file : 'Samples.csv'");
        return;
    }
    spdlog::info("TickerUtil::addSampleRanges : Adding ranges to : 'Samples.csv'");

    std::vector<std::string> sample_strings;

    std::string header;
    getline(file, header);

    std::string min_range;
    getline(file, min_range);

    std::string max_range;
    getline(file, max_range);

    std::string line;
    while(getline(file, line)) {
        sample_strings.push_back(line);
        min_range = getNewMinRange(min_range, line);
        max_range = getNewMaxRange(max_range, line);
    }

    std::ofstream samples_file;
    samples_file.open("Samples.csv");
    samples_file << header << "\n";
    samples_file << min_range;
    samples_file << max_range;
    for(std::string s : sample_strings)
        samples_file << s << "\n";

}

std::string TickerUtil::getNewMinRange(std::string previous, std::string current) {
    if (previous.empty())
        return current;

    std::string new_header = "";

    boost::char_separator<char> sep (",");

    boost::tokenizer<boost::char_separator<char>> prev_tokens (previous, sep);
    boost::tokenizer<boost::char_separator<char>> curr_tokens (current, sep);

    auto prev_iter = prev_tokens.begin();
    auto curr_iter = curr_tokens.begin();

    while(prev_iter != prev_tokens.end() && curr_iter != curr_tokens.end()) {
        new_header += std::to_string(std::min(std::stof(*prev_iter++), std::stof(*curr_iter++)));
        new_header += ",";
    }

    if(!new_header.empty())
        new_header.pop_back();

    new_header += "\n";

    return new_header;
}

std::string TickerUtil::getNewMaxRange(std::string previous, std::string current) {
    if (previous.empty())
        return current;

    std::string new_header = "";

    boost::char_separator<char> sep (",");

    boost::tokenizer<boost::char_separator<char>> prev_tokens (previous, sep);
    boost::tokenizer<boost::char_separator<char>> curr_tokens (current, sep);

    auto prev_iter = prev_tokens.begin();
    auto curr_iter = curr_tokens.begin();

    while(prev_iter != prev_tokens.end() && curr_iter != curr_tokens.end()) {
        new_header += std::to_string(std::max(std::stof(*prev_iter++), std::stof(*curr_iter++)));
        new_header += ",";
    }

    if(!new_header.empty())
        new_header.pop_back();

    new_header += "\n";

    return new_header;
}
