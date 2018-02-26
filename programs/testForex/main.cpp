#include <iostream>
#include "TradingIndicators.hpp"
#include "ReadingData.hpp"
#include "Drawing.hpp"
#include "ForexTesting.hpp"

using namespace std;

int main()
{
    cout << "Test Forex!" << endl;
    double start = 0.0;
    double stop = 1.0;
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

    for(int n = 0; n < (int)vPairName.size(); n++) {
        if(n != 6) continue;
        // загрузим котировки валютных пар
        CurrencyQuote TestData(vPairName[n], CurrencyQuote::DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL);
        int startPos = TestData.close.size() * start; // начало данных
        int stopPos = TestData.close.size() * stop; // конец данных
        std::cout << "Source: " << vPairName[n] << " data size: " << TestData.close.size() << std::endl;

        Indicators::BollingerBands iBB(20, 2);

        Drawing::Window iWindow(80);

        ForexTesting::Testing iForex; // класс для имитации торгов на форексе
        iForex.onePoint = 0.00001; // величина одного пункта
        iForex.setMoney(100000.0); // депозит в базовой валюте
        int leverage = 200; // кредитное плечо
        double spread = 0.00003; // спред
        double lot = 50.0;

        int isBlock = 0; // флаг блокирования открытия новых сделок
        double stopLoss;
        double takeProfit;
        double openPrice;
        int idOrder = 0; // id ордера

        for(int i = startPos + 1; i < stopPos; i++) {
            double& close = TestData.close[i];
            double& open = TestData.open[i];
            double& high = TestData.high[i];
            double& low = TestData.low[i];
            int hour = TestData.hour[i];

            iBB.updata(close);

            iWindow.updata(open, high, low, close);
            iWindow.setIndicator(iBB.tl);
            iWindow.setIndicator(iBB.bl);
            //iWindow.setIndicator(close + spread);

            iForex.updata(high, low, close, spread);

            // открываем сделку buy, если
            if(hour == 0 || hour == 23)
            if(close < iBB.bl && isBlock == 0) {
                stopLoss = close - iForex.onePoint * 6;
                takeProfit = close + iForex.onePoint * 16 + spread;
                openPrice = close;
                const int IS_BUY = 1;
                iForex.setOrder(close, lot, leverage, IS_BUY, spread, stopLoss, takeProfit, idOrder);
                isBlock = 1;
            } else
            if(close > iBB.tl && isBlock == 0) {
                takeProfit = close - iForex.onePoint * 16;
                stopLoss = close + iForex.onePoint * 6 + spread;
                openPrice = close;
                const int IS_SELL = -1;
                iForex.setOrder(close, lot, leverage, IS_SELL, spread, stopLoss, takeProfit, idOrder);
                isBlock = 1;
            }

            if(isBlock) {
                if(iForex.getStateOrder(idOrder)) {
                    isBlock = 0; // если ордер исполнился, снимаем блокировку
                } else {
                    #if(0)
                    if(iForex.getBuySellFlag(idOrder) == 1) {
                        double newStopLoss = close - iForex.onePoint * 30;
                        if(newStopLoss - iForex.onePoint * 20 > stopLoss) {
                            stopLoss = newStopLoss;
                            iForex.setStopLoss(idOrder, stopLoss);
                        }
                    } else
                    if(iForex.getBuySellFlag(idOrder) == -1) {
                        double newStopLoss = close + iForex.onePoint * 30 + spread;
                        if(newStopLoss < stopLoss - iForex.onePoint * 20) {
                            stopLoss = newStopLoss;
                            iForex.setStopLoss(idOrder, stopLoss);
                        }
                    }
                    #endif

                    // рисуем линии открытия сделки, тейк профита и стоп лосса
                    iWindow.setIndicator(openPrice);
                    iWindow.setIndicator(takeProfit);
                    iWindow.setIndicator(stopLoss);
                }
            }

            if(hour == 0 || hour == 23)
            if(1) {
                std::vector<Drawing::CandlesType> data = iWindow.getCandlesType();
                Drawing::viewCandleGraph("Source", data, 0);

                std::vector<double> vNull;
                Drawing::drawOscilloscope4xBeam("money"," money: ", iForex.vMoney, vNull, vNull, vNull, 1200, 400, 0);
            }

        } // for
        std::vector<double> vNull;
        Drawing::drawOscilloscope4xBeam("money"," money: ", iForex.vMoney, vNull, vNull, vNull, 1200, 400, 0);
        while(1) {
            cv::waitKey(10);
        }
    } // for

    return 0;
}
