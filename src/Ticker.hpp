#ifndef TICKER_H_
#define TICKER_H_

#include "Spot.hpp"

enum Interval {MINUTE, HOUR, DAY, MONTH, YEAR};

class Ticker
{
    public:
        Ticker(std::string symbol);

        ~Ticker();

        void loadAPIKey();

        void loadHistoricalSpots(std::string from, std::string to);

        void loadHistoricalSpots(std::time_t from, std::time_t to);

        void setInterval(Interval interval);

        void displaySpots();

        void clearSpots();

    private:
        std::string m_sSymbol;
        std::vector<Spot *> m_oSpots;
        std::string m_sAPIKey;
        std::string m_sInterval;
};

#endif // TICKER_H_
