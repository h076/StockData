#include "Ticker.hpp"
#include "ta-lib/ta_libc.h"

int main() {

    Ticker * apple = new Ticker("AAPL");

    int sampleCount = 5000000;
    int sampleLength = 80;
    apple->getPriceSamples("2024-01-21", "2024-11-21",  DAY, 10, 80);
    //apple->displaySamples();
    //apple->displaySamplesFeatures();
    apple->saveSamplesCSV();
    delete apple;

    return 0;
}
