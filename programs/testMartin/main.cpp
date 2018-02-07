#include <iostream>
#include "TradingIndicators.hpp"
#include "ReadingData.hpp"
#include "Drawing.hpp"
#include "Martingale.hpp"

using namespace std;

int main() {
    cout << "writing to a data file!" << endl;
    int numTrainingSamples = 400000; // количество данных дя обучения
    int posSamples = 0;

    // вектор файлов данных
    std::vector<std::string> vPairName(25);
    vPairName[0] = "..//..//train2018//EURAUD1.csv"; vPairName[1] = "..//..//train2018//EURJPY1.csv";
    vPairName[2] = "..//..//train2018//EURGBP1.csv"; vPairName[3] = "..//..//train2018//EURCAD1.csv";
    vPairName[4] = "..//..//train2018//EURNZD1.csv"; vPairName[5] = "..//..//train2018//EURCHF1.csv";
    vPairName[6] = "..//..//train2018//EURUSD1.csv";

    vPairName[7] = "..//..//train2018//AUDCAD1.csv"; vPairName[8] = "..//..//train2018//AUDNZD1.csv";
    vPairName[9] = "..//..//train2018//AUDCHF1.csv"; vPairName[10] = "..//..//train2018//AUDJPY1.csv";
    vPairName[11] = "..//..//train2018//AUDUSD1.csv";

    vPairName[12] = "..//..//train2018//CADCHF1.csv"; vPairName[13] = "..//..//train2018//CADJPY1.csv";

    vPairName[14] = "..//..//train2018//GBPAUD1.csv"; vPairName[15] = "..//..//train2018//GBPCHF1.csv";
    vPairName[16] = "..//..//train2018//GBPJPY1.csv"; vPairName[17] = "..//..//train2018//GBPUSD1.csv";

    vPairName[18] = "..//..//train2018//NZDCAD1.csv"; vPairName[19] = "..//..//train2018//NZDJPY1.csv";
    vPairName[20] = "..//..//train2018//NZDUSD1.csv";

    vPairName[21] = "..//..//train2018//USDCAD1.csv"; vPairName[22] = "..//..//train2018//USDCHF1.csv";
    vPairName[23] = "..//..//train2018//USDJPY1.csv";

    vPairName[24] = "..//..//train2018//CHFJPY1.csv";


    const double rawStart = 0.0; // начало данных
    const double rawStop = 1.0; // конец данных


    #if(1)

    for(int n = 0; n < (int)vPairName.size(); n++) {
        CurrencyQuote TestData(vPairName[n], CurrencyQuote::DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL);
        int startPos = TestData.close.size() * rawStart;
        int stopPos = TestData.close.size() * rawStop;
        std::cout << "Source: " << vPairName[n] << " data size: " << TestData.close.size() << std::endl;

        Drawing::Window iWindow(60);
        MartingaleAnatolia iMartingale;
        int nDeals = 0;
        int isStop = 0;
        int nDay = 0;
        int nDelay = 0;

        for(int i = startPos + 1; i < stopPos - 5; i++) {
            double& open = TestData.open[i];
            double& high = TestData.high[i];
            double& low = TestData.low[i];
            double& close = TestData.close[i];
            double& prevClose = TestData.close[i - 1];

            double& closeFutureA = TestData.close[i + 1];
            double& closeFutureB = TestData.close[i + 2];
            double& closeFutureC = TestData.close[i + 3];
            double& closeFutureD = TestData.close[i + 4];
            double& closeFutureE = TestData.close[i + 5];

            int hour = TestData.hour[i];

            iWindow.updata(open, high, low, close);

            int stateMartin = 0; // состояние предсказания для мартингейла
            if(close > prevClose) {
                stateMartin = 1;
            } else
            if(close < prevClose) {
                stateMartin = -1;
            }

            int stateFuture = 0; // состояние цены в будущем
            if(close > closeFutureA) {
                stateFuture = -1;
            } else
            if(close < closeFutureA) {
                stateFuture = 1;
            }

            // имитируем торги
            if(hour == 5 || hour == 6) {
                if(nDelay <= 0) {
                    if(stateMartin == stateFuture && stateMartin == 1) {
                        iMartingale.win(); nDeals++;
                        if(nDeals > 30) isStop = 1;
                    } else
                    if(stateMartin == stateFuture && stateMartin == -1) {
                        iMartingale.win(); nDeals++;
                        if(nDeals > 30) isStop = 1;
                    } else
                    if(stateFuture == 1 && stateMartin == -1) {
                        iMartingale.loss(); nDeals++;
                    } else
                    if(stateFuture == -1 && stateMartin == 1) {
                        iMartingale.loss(); nDeals++;
                    }
                    if(iMartingale.step == 4) {
                        nDelay = 5;
                    }
                } else {
                    if(nDelay > 0) nDelay--;
                }
            }


            if(iMartingale.stop == true) {
                std::cout << "stop!" << std::endl;
                std::vector<double> vNull;
                Drawing::drawOscilloscope4xBeam("money", "money", iMartingale.vMoney, vNull, vNull, vNull, 800, 400, 0);
                std::cout << "eff: " << iMartingale.eff << " nDay: " << nDay << std::endl;
                while(1) {
                    cv::waitKey(10);
                }
            }

            if(hour == 5 || hour == 6) {
                //std::vector<Drawing::CandlesType> vCandles = iWindow.getCandlesType();
                //Drawing::viewCandleGraph("test", vCandles, 0);
                //std::vector<double> vNull;
                //Drawing::drawOscilloscope4xBeam("money", "money", iMartingale.vMoney, vNull, vNull, vNull, 800, 400, 0);
                //std::cout << "eff: " << iMartingale.eff << " nDay: " << nDay << std::endl;
            } else {
                if(nDeals) nDay++;
                isStop = 0;
                nDeals = 0;
            }

            if(i == stopPos - 6) {
                std::vector<double> vNull;
                Drawing::drawOscilloscope4xBeam("money", "money", iMartingale.vMoney, vNull, vNull, vNull, 800, 400, 0);
                std::cout << "eff: " << iMartingale.eff << " nDay: " << nDay << std::endl;
            }

        } // for

        while(1) {
            cv::waitKey(10);
        }
    }

    #endif
    return 0;
}

