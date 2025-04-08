#include "Ticker.hpp"
#include "ta-lib/ta_libc.h"

int main() {

    Ticker * stock = new Ticker("JPM");

    int sampleCount = 800;
    int sampleLength = 60;
    stock->setMultiplier(30);
    stock->getPriceSamples("2023-02-01", "2023-07-01", MINUTE, sampleCount, sampleLength, 85);
    //apple->displaySamples();
    //apple->displaySamplesFeatures();
    stock->saveSamplesCSV();
    delete stock;

    return 0;
}
