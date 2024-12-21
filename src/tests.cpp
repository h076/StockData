#include "Ticker.hpp"
#include "gtest/gtest.h"
#include <ctime>

// Test fixture for Ticker
class TickerTest : public ::testing::Test {
protected:
    Ticker* ticker;
    void SetUp() override {
        ticker = new Ticker("AAPL");
    }

    void TearDown() override {
        delete ticker;
    }
};

// Test constructor and loadAPIKey
TEST_F(TickerTest, ConstructorAndAPIKey) {
    EXPECT_NE(ticker, nullptr);
    // Check if API Key is loaded, assumes API key file contains valid key
    ticker->loadAPIKey();
    EXPECT_FALSE(ticker->getAPIKey().empty());
}

// Test setInterval function
TEST_F(TickerTest, SetInterval) {
    ticker->setInterval(DAY);
    EXPECT_EQ(ticker->getInterval(), "day");

    ticker->setInterval(HOUR);
    EXPECT_EQ(ticker->getInterval(), "hour");

    ticker->setInterval(YEAR);
    EXPECT_EQ(ticker->getInterval(), "year");

    // Default case
    try {
        ticker->setInterval(static_cast<Interval>(999)); // Invalid interval
    }catch (const std::exception& e) {
        EXPECT_EQ(ticker->getInterval(), "month"); // Default fallback
    }
}

// Test clearSpots and clearSamples
TEST_F(TickerTest, ClearSpotsAndSamples) {
    // Assume after loading, spots are not empty and clear them
    std::time_t from = std::time(nullptr) - 2000000;
    std::time_t to = std::time(nullptr) - 2000;

    ticker->loadHistoricalSpots(from, to);
    EXPECT_FALSE(ticker->getSpots().empty());
    ticker->clearSpots();
    EXPECT_TRUE(ticker->getSpots().empty());

    // Similar for samples
    ticker->getPriceSamples("2024-01-21", "2024-05-21", DAY, 10, 80);
    EXPECT_FALSE(ticker->getSamples().empty());
    ticker->clearSamples();
    EXPECT_TRUE(ticker->getSamples().empty());
}

// Test getClosePriceArr
TEST_F(TickerTest, GetClosePriceArr) {
    std::time_t from = std::time(nullptr) - 2000000;
    std::time_t to = std::time(nullptr) - 2000;
    ticker->loadHistoricalSpots(from, to);

    int arrSize;
    double* closePrices = ticker->getClosePriceArr(&arrSize);

    if (ticker->getSpots().empty()) {
        EXPECT_EQ(closePrices, nullptr);
        EXPECT_EQ(arrSize, 0);
    } else {
        EXPECT_NE(closePrices, nullptr);
        EXPECT_EQ(arrSize, ticker->getSpots().size());
        free(closePrices);
    }
}

// Test for empty getHighPriceArr and getLowPriceArr
TEST_F(TickerTest, GetHighAndLowPriceArrEmpty) {
    int arrSizeHigh, arrSizeLow;
    double* highPrices = ticker->getHighPriceArr(&arrSizeHigh);
    double* lowPrices = ticker->getLowPriceArr(&arrSizeLow);

    EXPECT_EQ(highPrices, nullptr);
    EXPECT_EQ(arrSizeHigh, 0);

    EXPECT_EQ(lowPrices, nullptr);
    EXPECT_EQ(arrSizeLow, 0);
}

// Test loading historical spots
TEST_F(TickerTest, LoadHistoricalSpots) {
    // Consider mocking the downloadHistoricalData function to simulate data fetching
    std::string from = "2023-01-01";
    std::string to = "2023-02-01";
    ticker->loadHistoricalSpots(from, to);
    EXPECT_FALSE(ticker->getSpots().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
