#include "Ticker.hpp"
#include <chrono>
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <string>
#include <unordered_set>
#include <thread>

int main() {
    std::ifstream tickTxt ("SP500Tickers.txt");
    std::string line;
    std::vector<std::string> tickStr;

    if(tickTxt.is_open()) {
        getline(tickTxt, line);
        tickTxt.close();
    }else {
        spdlog::error("Cannot open file : 'SP500Tickers.txt'");
        return 1;
    }

    boost::char_separator<char> sep (", ");
    boost::tokenizer<boost::char_separator<char>> tokens (line, sep);
    for(const auto& t : tokens) {
        tickStr.push_back(t);
    }

    Ticker* t;
    std::ofstream file;
    //file.open("Samples.csv");

    std::unordered_set<int> sampled;
    int strIdx;
    int requests = 0;

    srand(time(NULL));

    for(int i=0; i<50; i++) {
        if (requests > 0 && requests % 5 == 0) // then we wait 1 minute as polygon only allows 5 requests a minute
            std::this_thread::sleep_for(std::chrono::minutes(1));

        strIdx = rand()%tickStr.size();
        while(sampled.find(strIdx) != sampled.end())
            strIdx = rand()%tickStr.size();
        sampled.insert(strIdx);

        t = new Ticker(tickStr[strIdx]);
        t->setMultiplier(30);
        t->getPriceSamples("2024-01-21", "2024-11-21", MINUTE, 4, 160);
        requests++;

        // if false the no samples have been loaded
        if(!t->saveSamplesTo(file)) {
            // remove ticker index from sampled as no samples saved
            sampled.erase(strIdx);
            i--;
        }

        delete t;
    }
    file.close();

    return 0;
}
