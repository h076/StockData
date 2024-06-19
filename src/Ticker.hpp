#ifndef TICKER_H_
#define TICKER_H_

#include "Spot.hpp"

class Ticker
{
    public:
        Ticker(std::string symbol);

        ~Ticker();

    private:
        std::string symbol;
        std::vector<Spot> spots;
};

#endif // TICKER_H_
