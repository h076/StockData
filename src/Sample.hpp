#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "ta-lib/ta_libc.h"
#include "Handler.hpp"

enum signal {STRONG_BUY, BUY, HOLD, SELL, STRONG_SELL};

class Sample
{

    public:
        Sample(double * closeSample, double * highSample, double * lowSample,
               int sampleLength, int trainSplit);

        ~Sample();

        void show();
        std::string strPart(int index);


        class RSIHandler : public Handler {
            public:
                RSIHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getRSI());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getRSI();
        };

        //double getRSI();
        double * getRSIRange();

        class MACDHandler : public Handler {
            public:
                MACDHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getMACD());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getMACD();
        };

        //double getMACD();
        double * getMACDRange();

        class MACDSignalHandler : public Handler {
            public:
                MACDSignalHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getMACDSignal());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getMACDSignal();
        };

        //double getMACDSignal();
        double * getMACDSignalRange();

        class CloseHandler : public Handler {
            public:
                CloseHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getClose());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getClose();
        };

        //double getClose();
        double * getCloseRange();

        class StochFastKHandler : public Handler {
            public:
                StochFastKHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getStochFastK());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getStochFastK();
        };

        //double getStochFastK();
        double * getStochFastKRange();

        class StochFastDHandler : public Handler {
            public:
                StochFastDHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getStochFastD());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getStochFastD();
        };

        //double getStochFastD();
        double * getStochFastDRange();

        class WilliamsRHandler : public Handler {
            public:
                WilliamsRHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getWilliamsR());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getWilliamsR();
        };

        //double getWilliamsR();
        double * getWilliamsRRange();

        class UltimateOscillatorHandler : public Handler {
            public:
                UltimateOscillatorHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getUltimateOscillator());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getUltimateOscillator();
        };

        //double getUltimateOscillator();
        double * getUltimateOscillatorRange();

        class TSFHandler : public Handler {
            public:
                TSFHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getTSF());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getTSF();
        };

        //double getTSF();
        double * getTSFRange();

        class DEMAHandler : public Handler {
            public:
                DEMAHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getDEMA());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getDEMA();
        };

        //double getDEMA();
        double * getDEMARange();

        class CCIHandler : public Handler {
            public:
                CCIHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getCCI());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getCCI();
        };

        //double getCCI();
        double * getCCIRange();

        //double getVariance();

        class TRIXHandler : public Handler {
            public:
                TRIXHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getTRIX());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getTRIX();
        };

        //double getTRIX();
        double * getTRIXRange();

        class AROONHandler : public Handler {
            public:
                AROONHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getAROON());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getAROON();
        };

        //double getAROON();
        double * getAROONRange();

        class AROONUPHandler : public Handler {
            public:
                AROONUPHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getAROONUP());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getAROONUP();
        };

        //double getAROONUP();
        double * getAROONUPRange();

        class AROONDOWNHandler : public Handler {
            public:
                AROONDOWNHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getAROONDOWN());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getAROONDOWN();
        };

        //double getAROONDOWN();
        double * getAROONDOWNRange();

       class SampleYHandler : public Handler {
            public:
                SampleYHandler(Sample * s) : Handler(s) {}
                void handle(std::vector<double>& results) override {
                    results.push_back(getSampleY());
                    if(m_pNext)
                        m_pNext->handle(results);
                }

            private:
                double getSampleY();
        };

        //double getSampleY(); // return percentage increase from start to end of test portion
        double * getYRange();

        void setIndicatorValues();

        double minInRange(double * r);
        double maxInRange(double * r);

        std::string toCSVLine();
        std::string minRangeToCSV();
        std::string maxRangeToCSV();
        const std::string toCSVHeader();

        enum::signal yToSignal(double y);
        void setLabel(double yMin, double yMax);
        std::string getSignalAsString();

        int getNumberOfIndicators() {return m_vdIndicatorValues.size();}
        double getIndicatorValue(int idx);

    private:
        double * m_dpSampleClose;
        double * m_dpSampleHigh;
        double * m_dpSampleLow;
        int m_nSampleLength;
        int m_nTrainSplit;

        std::vector<double> m_vdIndicatorValues;

        enum::signal m_eSignal;


};

#endif // SAMPLE_H_
