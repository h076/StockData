#include "Ticker.hpp"
#include "ta-lib/ta_libc.h"

int main() {

    Ticker * apple = new Ticker("TFX");

    int sampleCount = 100;
    int sampleLength = 80;
    apple->setMultiplier(30);
    apple->getPriceSamples("2024-01-21", "2024-11-21", MINUTE, sampleCount, sampleLength);
    //apple->displaySamples();
    //apple->displaySamplesFeatures();
    apple->saveSamplesCSV();
    delete apple;

    return 0;
}
