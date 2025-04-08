#ifndef MULTITICKER_H_
#define MULTITICKER_H_

#include "Ticker.hpp"

class MultiTicker {
    public:
        MultiTicker();

        ~MultiTicker() {
            for(Ticker * t : m_vtTickers)
                delete t;
        }

        bool saveTicker(Ticker * t);

        void saveToFile(std::ofstream file);

        std::string minRangeToCSV();
        std::string maxRangeToCSV();

    private:
        std::vector<Ticker *> m_vtTickers;
        std::vector<double> m_vdMinRanges;
        std::vector<double> m_vdMaxRanges;
};

#endif // MULTITICKER_H_
