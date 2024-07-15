#include "Ticker.hpp"

int main() {

    Ticker * apple = new Ticker("AAPL");
    apple->loadHistoricalSpots("2020-02-23", "2023-08-23");
    apple->displaySpots();
    return 0;
}
