#ifndef TICKER_H_
#define TICKER_H_

#include "Spot.hpp"

class Ticker
{
    public:
        Ticker(std::string symbol);

        ~Ticker();

        void loadAPIKey();

        void loadHistoricalSpots(std::string from, std::string to);

        void loadHistoricalSpots(std::time_t from, std::time_t to);

        void displaySpots();

    private:
        std::string m_sSymbol;
        std::vector<Spot *> m_oSpots;
        std::string m_sAPIKey;
};

#endif // TICKER_H_
