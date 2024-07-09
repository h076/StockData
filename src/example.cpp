#include "Ticker.hpp"

int main() {
    std::cout << "hello\n";
    Ticker * apple = new Ticker("AAPL");
    apple->loadHistoricalSpots("2020-02-23", "2023-08-23");

    return 0;
}
