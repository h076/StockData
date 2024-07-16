#include "Ticker.hpp"

int main() {

    Ticker * apple = new Ticker("AAPL");
    apple->setInterval(MONTH);
    apple->loadHistoricalSpots("2020-02-23", "2023-09-23");
    apple->displaySpots();
    return 0;
}
