#include "Ticker.hpp"

int main() {
    std::cout << "hello\n";
    Ticker * apple = new Ticker("AAPL");
    apple->getCurrentSpot();
    return 0;
}
