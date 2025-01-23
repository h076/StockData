#include "Ticker.hpp"
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <string>
#include <unordered_set>

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

    for(int i=0; i<5; i++) {
        strIdx = rand()%tickStr.size();
        while(sampled.find(strIdx) != sampled.end())
            strIdx = rand()%tickStr.size();
        sampled.insert(strIdx);

        t = new Ticker(tickStr[strIdx]);
        t->setMultiplier(30);
        t->getPriceSamples("2024-01-21", "2024-11-21", MINUTE, 126, 80);
        t->saveSamplesTo(file);
        delete t;
    }
    file.close();

    return 0;
}
