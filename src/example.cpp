#include "Ticker.hpp"
#include "ta-lib/ta_libc.h"

int main() {

    Ticker * apple = new Ticker("AAPL");

    int sampleCount = 5000;
    int sampleLength = 120;
    apple->setMultiplier(30);
    apple->getPriceSamples("2023-01-01", "2023-11-01", MINUTE, sampleCount, sampleLength);
    //apple->displaySamples();
    //apple->displaySamplesFeatures();
    apple->saveSamplesCSV();
    delete apple;

    return 0;
}
