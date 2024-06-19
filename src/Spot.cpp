#include "Spot.hpp"
#include "time_utils.hpp"
#include <sstream>

Spot::Spot(std::time_t date, double open, double high, double low, double close) {
    m_tDate = date;
    m_dOpen = open;
    m_dHigh = high;
    m_dLow = low;
    m_dClose = close;
}

Spot::Spot(std::string date, double open, double high, double low, double close) {
    m_tDate = dateToEpoch(date.c_str());
    m_dOpen = open;
    m_dHigh = high;
    m_dLow = low;
    m_dClose = close;
}

Spot::Spot(std::time_t date, double price) {
    m_tDate = date;
    m_dOpen = price;
    m_dHigh = price;
    m_dLow = price;
    m_dClose = price;
}

Spot::Spot(std::string date, double price) {
    m_tDate = dateToEpoch(date.c_str());
    m_dOpen = price;
    m_dHigh = price;
    m_dLow = price;
    m_dClose = price;
}

double Spot::getOpen() const {return m_dOpen;}

double Spot::getHigh() const {return m_dHigh;}

double Spot::getLow() const {return m_dLow;}

double Spot::getClose() const {return m_dClose;}

std::string Spot::getDateToString() {
    return epochToDate(m_tDate);
}

std::string Spot::toString() {
    std::stringstream sOpen;
    sOpen << m_dOpen;
    std::stringstream sHigh;
    sHigh << m_dHigh;
    std::stringstream sLow;
    sLow << m_dLow;
    std::stringstream sClose;
    sClose << m_dClose;
    return "{date: " + this->getDateToString()
        + "Low : " + sLow.str()
        + "High : " + sHigh.str()
        + "Open : " + sOpen.str()
        + "Close :" + sClose.str() + "}";
}

void Spot::printSpot() {
    std::cout << this->toString() << std::endl;
}
