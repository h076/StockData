#ifndef TICKER_H_
#define TICKER_H_

#include "Spot.hpp"

class Ticker
{
    public:
        Ticker(std::string symbol);

        ~Ticker();

        void getCurrentSpot();

        void printCurrentSpot();

    private:
        std::string m_sSymbol;
        std::vector<Spot> m_oSpots;
};

#endif // TICKER_H_
