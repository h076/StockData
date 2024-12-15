#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "ta-lib/ta_libc.h"

class Sample
{

    public:
        Sample(double * closeSample, double * highSample, double * lowSample,
               int sampleLength, int trainSplit);

        ~Sample();

        void show();
        std::string strPart(int index);

        double getRSI();
        double * getRSIRange();

        double getMACD();
        double * getMACDRange();

        double getClose();
        double * getCloseRange();

        double getStochFastK();
        double * getStochFastKRange();

        double getStochFastD();
        double * getStochFastDRange();

        double getWilliamsR();
        double * getWilliamsRRange();

        double getUltimateOscillator();
        double * getUltimateOscillatorRange();

        double getTSF();
        double * getTSFRange();

        double getDEMA();
        double * getDEMARange();

        double getCCI();
        double * getCCIRange();

        //double getVariance();

        double getTRIX();
        double * getTRIXRange();

        double getSampleY(); // return percentage increase from start to end of test portion
        double * getYRange();

        double minInRange(double * r);
        double maxInRange(double * r);

        std::string toCSVLine();
        std::string rangesToCSVLine();
        const std::string toCSVHeader();

    private:
        double * m_dpSampleClose;
        double * m_dpSampleHigh;
        double * m_dpSampleLow;
        int m_nSampleLength;
        int m_nTrainSplit;
};

#endif // SAMPLE_H_