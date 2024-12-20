#ifndef TICKER_H_
#define TICKER_H_

#include "Spot.hpp"
#include "Sample.hpp"

enum Interval {MINUTE, HOUR, DAY, MONTH, YEAR};

class Ticker
{
    public:
        Ticker(std::string symbol);

        ~Ticker();

        void loadAPIKey();
        std::string getAPIKey() const {return m_sAPIKey;}

        void loadHistoricalSpots(std::string from, std::string to);

        void loadHistoricalSpots(std::time_t from, std::time_t to);

        void setInterval(Interval interval);
        std::string getInterval() const {return m_sInterval;}

        void displaySpots();

        void clearSpots();
        void clearSamples();

        std::vector<Spot *> getSpots() const {return m_oSpots;}
        std::vector<Sample *> getSamples() const {return m_oSamples;}

        double * getClosePriceArr(int * arrSize);
        double * getHighPriceArr(int * arrSize);
        double * getLowPriceArr(int * arrSize);

        void getPriceSamples(std::string from, std::string to, Interval interval, int sampleCount,
                                      int sampleLength, int trainSplit = 70);

        void displaySamples();

        void displaySamplesFeatures();

        void saveSamplesCSV();

    private:
        std::string m_sSymbol;
        std::vector<Spot *> m_oSpots;
        std::string m_sAPIKey;
        std::string m_sInterval;
        std::vector<Sample *> m_oSamples;
};

#endif // TICKER_H_
