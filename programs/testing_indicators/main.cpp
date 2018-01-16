#include <iostream>
#include "TradingIndicators.hpp"
#include "ReadingData.hpp"
#include "Drawing.hpp"
#include "NormalizationData.hpp"
#include "Scaling.hpp"

using namespace std;

int main()
{
    cout << "Test program" << endl;
    CurrencyQuote TestData("..//..//hist//EURUSD.txt");

    Indicators::BollingerBands iBands(20, 2);
    Indicators::EMA iEMA(10);
    Indicators::SMA iSMA(20);
    Indicators::SMM iSMM(10);
    Indicators::NoLagMa iNoLagMa(10);
    Indicators::SearchExtrema iExtrema(100);

    Indicators::BasicExtrema iBasicExtrema(1440*5, 30);

    Indicators::MultiMa<Indicators::EMA> iMultiMa(20, 20, 20);

    Indicators::Window iDataWindow(30);
    Indicators::Window iDataWindow1440(1440);

    Drawing::Window iWindow(120);

    for(int i = 0; i < (int)TestData.close.size(); i++) {
        double& open = TestData.open[i];
        double& high = TestData.high[i];
        double& low = TestData.low[i];
        double& close = TestData.close[i];

        double outSma = iSMA.updata(close);
        double outNoLagMa = iNoLagMa.updata(close);
        double outSmm = iSMM.updata(close);
        iBands.updata(close);
        iExtrema.updata(close);
        iBasicExtrema.updata(close);
        iMultiMa.updata(close);
        iDataWindow.updata(close);
        iDataWindow1440.updata(close);

        iWindow.updata(open, high, low, close);
        #if(0)
        iWindow.setIndicator(iBands.tl);
        iWindow.setIndicator(iBands.bl);
        iWindow.setIndicator(iBands.ml);
        iWindow.setIndicator(outSmm);
        #endif
        for(int n = 0; n < (int)iMultiMa.vOutput.size(); n++) {
            iWindow.setIndicator(iMultiMa.vOutput[n]);
        }

        //std::vector<Drawing::CandlesType> vCandles = iWindow.getCandlesType();
        //Drawing::viewCandleGraph("test", vCandles, 0);

        if(iExtrema.lastExtremums.size() > 10) {
            std::vector<double> vNull;
            std::vector<double> vLastData1440;
            std::vector<double> vLastData1440Dx;
            std::vector<double> vLastData = iDataWindow.data;
            std::vector<double> vLastDataDx;
            std::vector<double> vLastExtremums = iExtrema.lastExtremums;
            std::vector<double> vLastExtremumsDx;

            vLastExtremums.push_back(close);

            ScalingData::calcArrayScaling(iDataWindow1440.data, vLastData1440, 30);
            //Normalization::calcZscore(vLastData1440, vLastData1440Dx);
            //Normalization::calcMaxAmplitude(vLastData1440Dx,vLastData1440Dx);

            Normalization::calcZscore(vLastData, vLastDataDx);
            Normalization::calcMaxAmplitude(vLastDataDx,vLastDataDx);

            Normalization::calcDifference(vLastExtremums, vLastExtremumsDx);
            Normalization::calcMaxAmplitude(vLastExtremumsDx,vLastExtremumsDx);

            Normalization::calcZscore(vLastExtremums, vLastExtremums, 4);
            Normalization::calcMaxAmplitude(vLastExtremums,vLastExtremums);

            //Drawing::drawOscilloscope4xBeam("data 1440", "last data 1440", iDataWindow1440.data, vNull, vNull, vNull, 800, 400, 0);
            //Drawing::drawOscilloscope4xBeam("data 1440 - 30", "last data 1440 - 30", vLastData1440, vNull, vNull, vNull, 800, 400, 0);
            //Drawing::drawOscilloscope4xBeam("data", "last data", vLastDataDx, vNull, vNull, vNull, 800, 400, 0);
            Drawing::drawOscilloscope4xBeam("extrema", "last extrema", vLastExtremums, vNull, vNull, vNull, 800, 400, 0);
            Drawing::drawOscilloscope4xBeam("extrema2", "last extrema2", vLastExtremumsDx, vNull, vNull, vNull, 800, 400, 0);

            //cv::waitKey(100);
        }

        //std::cout << "close: " << TestData.close[i] << " ema 2: " << outEma << " bars: " << i << "//" << TestData.close.size() << "\r";
    }
    return 0;
}
