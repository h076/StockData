#include "Ticker.hpp"
#include "ta-lib/ta_libc.h"

int main() {

    Ticker * apple = new Ticker("AAPL");

    int sampleCount = 50;
    int sampleLength = 120;
    apple->setMultiplier(30);
    apple->getPriceSamples("2023-03-01", "2023-03-21", MINUTE, sampleCount, sampleLength);
    //apple->displaySamples();
    //apple->displaySamplesFeatures();
    apple->saveSamplesCSV();
    delete apple;

    return 0;
}
