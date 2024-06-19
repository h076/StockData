#ifndef SPOT_H_
#define SPOT_H_

#include <iostream>

class Spot
{
    public:
        Spot(std::time_t date, double open, double high, double low, double close);

        Spot(std::string date, double open, double high, double low, double close);

        Spot(std::time_t date, double price);

        Spot(std::string, double price);

        ~Spot();

        time_t getDate() const;

        double getOpen() const;

        double getHigh() const;

        double getLow() const;

        double getClose() const;

        std::string getDateToString();

        std::string toString();

        void printSpot();

    private:
        std::time_t m_tDate;
        double m_dOpen;
        double m_dHigh;
        double m_dLow;
        double m_dClose;

};

#endif // SPOT_H_
