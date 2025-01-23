#include "Sample.hpp"
#include <climits>

Sample::Sample(double * closeSample, double * highSample, double * lowSample,
               int sampleLength, int trainSplit = 70) {
    m_dpSampleClose = (double *) malloc(sampleLength*sizeof(double));
    m_dpSampleHigh = (double *) malloc(sampleLength*sizeof(double));
    m_dpSampleLow = (double *) malloc(sampleLength*sizeof(double));

    std::memcpy(m_dpSampleClose, closeSample, sampleLength*sizeof(double));
    std::memcpy(m_dpSampleHigh, highSample, sampleLength*sizeof(double));
    std::memcpy(m_dpSampleLow, lowSample, sampleLength*sizeof(double));

    m_nSampleLength = sampleLength;
    m_nTrainSplit = trainSplit;
}

Sample::~Sample() {
    free(m_dpSampleClose);
    free(m_dpSampleHigh);
    free(m_dpSampleLow);
}

void Sample::show() {
    std::cout << strPart(0);
    for(int i=1; i<m_nSampleLength; i++) {
        std::cout << ", " << strPart(i);
    }
    std::cout << std::endl;
}

std::string Sample::strPart(int index) {
    if(index >= m_nSampleLength) {
        std::cout << "showPart : Index out of range.\n";
        return "";
    }

    return "{C: " + std::to_string(*(m_dpSampleClose+index)) + ", H: " + std::to_string(*(m_dpSampleHigh))
        + ", L: " + std::to_string(*(m_dpSampleLow)) + "}";
}

double Sample::getRSI() {
    int outBegIdx;
    int outNBElement;
    double * rsiOut = (double *) malloc(m_nSampleLength*sizeof(double));

    // 14 is number of intervals to use to calculate RSI
    TA_RSI(0, static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100)), m_dpSampleClose,
                  14, &outBegIdx, &outNBElement, rsiOut);

    return *(rsiOut+outNBElement-1);
}

double * Sample::getRSIRange() {
    double min = 0;
    double max = 100;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}
/**
 * TA_RetCode TA_MACD( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInFastPeriod,  From 2 to 100000
                    int           optInSlowPeriod, /* From 2 to 100000
                    int           optInSignalPeriod, /* From 1 to 100000
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outMACD[],
                    double        outMACDSignal[],
                    double        outMACDHist[] );
 */

double Sample::getMACD() {
    int outBegIdx;
    int outNBElement;

    int optInFastPeriod = 12;
    int optInSlowPeriod = 26;
    int optInSignalPeriod = 9;

    double * outMACD = (double *) malloc(m_nSampleLength*sizeof(double));
    double * outMACDSignal = (double *) malloc(m_nSampleLength*sizeof(double));
    double * outMACDHist = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_MACD(0, static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100)), m_dpSampleClose,
            optInFastPeriod, optInSlowPeriod, optInSignalPeriod, &outBegIdx, &outNBElement, outMACD,
            outMACDSignal, outMACDHist);

    return *(outMACD+outNBElement-1);
}

double * Sample::getMACDRange() {
    double min = -1.0;
    double max = 1.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

double Sample::getClose() {
    return *(m_dpSampleClose+static_cast<int>((m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100))));
}

double * Sample::getCloseRange() {
    double min = DBL_MAX;
    double max = DBL_MIN;
    double * r = (double *) malloc(2*sizeof(double));

    double closePrice;
    for(int i=0; i<m_nSampleLength; i++) {
        closePrice = *(m_dpSampleClose+i);
        min = std::min(closePrice, min);
        max = std::max(closePrice, max);
    }

    *r = min;
    *(r+1) = max;
    return r;
}

/**
TA_RetCode TA_STOCHF( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int           optInFastK_Period, /* From 1 to 100000
                      int           optInFastD_Period, /* From 1 to 100000
                      TA_MAType     optInFastD_MAType,
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outFastK[],
                      double        outFastD[] );
*/

double Sample::getStochFastK() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInFastK_Period = 5;
    int optInFastD_Period = 4;

    TA_MAType optInFastD_MAType = TA_MAType_SMA;

    int outBegIdx;
    int outNBElement;

    double * outFastK = (double *) malloc(m_nSampleLength*sizeof(double));;
    double * outFastD = (double *) malloc(m_nSampleLength*sizeof(double));;

    TA_STOCHF(startIdx, endIdx, m_dpSampleHigh, m_dpSampleLow, m_dpSampleClose, optInFastK_Period,
              optInFastD_Period, optInFastD_MAType, &outBegIdx, &outNBElement, outFastK, outFastD);

    return *(outFastK+outNBElement-1);
}

double * Sample::getStochFastKRange() {
    double min = 0.0;
    double max = 100.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

double Sample::getStochFastD() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInFastK_Period = 5;
    int optInFastD_Period = 4;

    TA_MAType optInFastD_MAType = TA_MAType_SMA;

    int outBegIdx;
    int outNBElement;

    double * outFastK = (double *) malloc(m_nSampleLength*sizeof(double));;
    double * outFastD = (double *) malloc(m_nSampleLength*sizeof(double));;

    TA_STOCHF(startIdx, endIdx, m_dpSampleHigh, m_dpSampleLow, m_dpSampleClose, optInFastK_Period,
              optInFastD_Period, optInFastD_MAType, &outBegIdx, &outNBElement, outFastK, outFastD);

    return *(outFastD+outNBElement-1);
}

double * Sample::getStochFastDRange() {
    double min = 0.0;
    double max = 100.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

/**
 * TA_RetCode TA_WILLR( int    startIdx,
                     int    endIdx,
                     const double inHigh[],
                     const double inLow[],
                     const double inClose[],
                     int           optInTimePeriod, /* From 2 to 100000
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] );
 */

double Sample::getWilliamsR() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod = 14;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_WILLR(startIdx, endIdx, m_dpSampleHigh, m_dpSampleLow, m_dpSampleClose,
             optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    return *(outReal+outNBElement-1);
}

double * Sample::getWilliamsRRange() {
    double min = -100.0;
    double max = 0.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

/**
 * TA_RetCode TA_ULTOSC( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int           optInTimePeriod1, /* From 1 to 100000
                      int           optInTimePeriod2, /* From 1 to 100000
                      int           optInTimePeriod3, /* From 1 to 100000
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] );
 */

double Sample::getUltimateOscillator() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod1 = 7;
    int optInTimePeriod2 = 14;
    int optInTimePeriod3 = 28;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_ULTOSC(startIdx, endIdx, m_dpSampleHigh, m_dpSampleLow, m_dpSampleClose, optInTimePeriod1,
              optInTimePeriod2, optInTimePeriod3, &outBegIdx, &outNBElement, outReal);

    return *(outReal+outNBElement-1);
}

double * Sample::getUltimateOscillatorRange() {
    double min = 0.0;
    double max = 100.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

/**
 * TA_RetCode TA_TSF( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );
 */

double Sample::getTSF() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod = 14;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_TSF(startIdx, endIdx, m_dpSampleClose, optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    return *(outReal+outNBElement-1);
}

double * Sample::getTSFRange() {
    double min = DBL_MAX;
    double max = DBL_MIN;
    double * r = (double *) malloc(2*sizeof(double));

    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod = 14;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_TSF(startIdx, endIdx, m_dpSampleClose, optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    double TSFval;
    for(int i=0; i<outNBElement; i++) {
        TSFval = *(outReal+i);
        min = std::min(TSFval, min);
        max = std::max(TSFval, max);
    }

    *r = min;
    *(r+1) = max;
    return r;
}

/**
 * TA_RetCode TA_DEMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 2 to 100000
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );
 */

double Sample::getDEMA() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    // should be 50 days may change
    int optInTimePeriod = 10;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_DEMA(startIdx, endIdx, m_dpSampleClose, optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    return *(outReal+outNBElement-1);
}

double * Sample::getDEMARange() {
    double min = DBL_MAX;
    double max = DBL_MIN;
    double * r = (double *) malloc(2*sizeof(double));

    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    // should be 50 days may change
    int optInTimePeriod = 10;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_DEMA(startIdx, endIdx, m_dpSampleClose, optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    double DEMAval;
    for(int i=0; i<outNBElement; i++) {
        DEMAval = *(outReal+i);
        min = std::min(DEMAval, min);
        max = std::max(DEMAval, max);
    }

    *r = min;
    *(r+1) = max;
    return r;
}

/**
 * TA_RetCode TA_CCI( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int           optInTimePeriod,
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );
 */

double Sample::getCCI() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod = 5;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_CCI(startIdx, endIdx, m_dpSampleHigh, m_dpSampleLow, m_dpSampleClose,
           optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    return *(outReal+outNBElement-1);
}

double * Sample::getCCIRange() {
    double min = -100.0;
    double max = 100.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

/**
 * TA_RetCode TA_VAR( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod,
                   double        optInNbDev,
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );
 */
/*
double Sample::getVariance() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod;
    double optInNbDev;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));
}*/

/**
 * TA_RetCode TA_TRIX( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod,
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );
 */

double Sample::getTRIX() {
    int startIdx = 0;
    int endIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100));

    int optInTimePeriod = 14;

    int outBegIdx;
    int outNBElement;

    double * outReal = (double *) malloc(m_nSampleLength*sizeof(double));

    TA_TRIX(startIdx, endIdx, m_dpSampleClose, optInTimePeriod, &outBegIdx, &outNBElement, outReal);

    return *(outReal+outNBElement-1);
}

double * Sample::getTRIXRange() {
    double min = -100.0;
    double max = 100.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}


double Sample::getSampleY() {
    int startIdx = static_cast<int>(m_nSampleLength*(static_cast<float>(m_nTrainSplit)/100))+1;
    double startCloseValue = *(m_dpSampleClose+startIdx);
    double endCloseValue = *(m_dpSampleClose+m_nSampleLength-1);

    return ((endCloseValue - startCloseValue) * 100) / startCloseValue;
}

double * Sample::getYRange() {
    double min = -30.0;
    double max = 30.0;
    double * r = (double *) malloc(2*sizeof(double));

    *r = min;
    *(r+1) = max;
    return r;
}

double Sample::minInRange(double * r) {
    double min = *r;
    free(r);
    return min;
}

double Sample::maxInRange(double * r) {
    double max = *(r+1);
    free(r);
    return max;
}

std::string Sample::toCSVLine() {
    return  std::to_string(getRSI()) + "," + std::to_string(getMACD()) + "," + std::to_string(getClose()) + "," + std::to_string(getStochFastK())
         + "," + std::to_string(getStochFastD()) + "," + std::to_string(getWilliamsR()) + "," + std::to_string(getUltimateOscillator())
         + "," + std::to_string(getTSF()) + "," + std::to_string(getCCI()) + "," + std::to_string(getSampleY()) + "\n";

}

std::string Sample::minRangeToCSV() {
    return std::to_string(minInRange(getRSIRange())) + "," + std::to_string(minInRange(getMACDRange())) + "," + std::to_string(minInRange(getCloseRange())) + "," + std::to_string(minInRange(getStochFastKRange()))
         + "," + std::to_string(minInRange(getStochFastDRange())) + "," + std::to_string(minInRange(getWilliamsRRange())) + "," + std::to_string(minInRange(getUltimateOscillatorRange()))
         + "," + std::to_string(minInRange(getTSFRange())) + "," + std::to_string(minInRange(getCCIRange())) + "," + std::to_string(minInRange(getYRange())) + "\n";
}

std::string Sample::maxRangeToCSV() {
    return std::to_string(maxInRange(getRSIRange())) + "," + std::to_string(maxInRange(getMACDRange())) + "," + std::to_string(maxInRange(getCloseRange())) + "," + std::to_string(maxInRange(getStochFastKRange()))
         + "," + std::to_string(maxInRange(getStochFastDRange())) + "," + std::to_string(maxInRange(getWilliamsRRange())) + "," + std::to_string(maxInRange(getUltimateOscillatorRange()))
         + "," + std::to_string(maxInRange(getTSFRange())) + "," + std::to_string(maxInRange(getCCIRange())) + "," + std::to_string(maxInRange(getYRange())) + "\n";
}

const std::string Sample::toCSVHeader() {
    return "RSI,MACD,Close,Stochastic Fast K,Stochastic Fast D,Williams %R,Ultimate Oscilator,TSF,CCI,Y\n";
}
