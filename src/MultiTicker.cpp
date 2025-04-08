#include "MultiTicker.hpp"
#include "Ticker.hpp"

bool MultiTicker::saveTicker(Ticker * t) {
    t->setRanges();

    int numIndicators = t->getNumIndicators();
    double min, max;

    if(m_vdMinRanges.empty()) {
        for(int i=0; i < numIndicators; i++) {
            m_vdMinRanges.push_back(t->getMinRange(i));
            m_vdMaxRanges.push_back(t->getMaxRange(i));
        }
    }else {
        for(int i=0; i < numIndicators; i++) {
            min = t->getMinRange(i);
            max = t->getMaxRange(i);

            if(min < m_vdMinRanges[i])
                m_vdMinRanges[i] = min;

            if(max > m_vdMaxRanges[i])
                m_vdMaxRanges[i] = max;
        }
    }

    return true;
}

void MultiTicker::saveToFile(std::ofstream file) {
    if(m_vtTickers.empty()) {
        spdlog::error("MultiTicker::saveToFile : no tickers to be saved.");
        return;
    }

    if(!file.is_open()) {
        file.open("Samples.csv");
        file << m_vtTickers[0]->getSampleCSVHeader();
        file << minRangeToCSV();
        file << maxRangeToCSV();
    }
}

std::string MultiTicker::minRangeToCSV() {
    if(m_vdMinRanges.empty()) {
        spdlog::error("MultiTicker::minRangeToCSV : no ranges present.");
        return "";
    }

    std::string ranges = "";
    for(double d : m_vdMinRanges) {
        ranges += std::to_string(d) + ",";
    }
    ranges.pop_back();
    ranges += "\n";

    return ranges;
}

std::string MultiTicker::maxRangeToCSV() {
    if(m_vdMaxRanges.empty()) {
        spdlog::error("MultiTicker::minRangeToCSV : no ranges present.");
        return "";
    }

    std::string ranges = "";
    for(double d : m_vdMaxRanges) {
        ranges += std::to_string(d) + ",";
    }
    ranges.pop_back();
    ranges += "\n";

    return ranges;
}
