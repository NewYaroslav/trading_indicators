#include <iostream>
#include "ReadingData.hpp"
#include "TradingIndicators.hpp"
#include "StrategyEffectiveness.hpp"

using namespace std;

int main() {
    cout << "checking winrate" << endl;
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

    double start = 0.0;
    double stop = 1.0;
    int expTime = 4;

    StrategyEffectiveness iEff;
    iEff.money = 100000.0; // начальная сумма
    iEff.profit = 0.9; // доход 90 процентов от ставки
    iEff.rate = 0.01; // ставка 1 процент от депозита

    for(int n = 0; n < (int)vPairName.size(); n++) {
        // загрузим котировки валютных пар
        CurrencyQuote TestData(vPairName[n], CurrencyQuote::DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL);
        int startPos = TestData.close.size() * start; // начало данных
        int stopPos = TestData.close.size() * stop; // конец данных
        std::cout << "Source: " << vPairName[n] << " data size: " << TestData.close.size() << std::endl;

        Indicators::BollingerBands iBB(20, 2.0); // индикатор боллинджер
        Indicators::RSI iRSI(14); // индикатор RSI
        StrategyEffectiveness iEffL;

        for(int i = startPos; i < stopPos - expTime; i++) {
            double& close = TestData.close[i];
            double& closeFuture = TestData.close[i + expTime];
            int hour = TestData.hour[i];

            iBB.updata(close); // обновляем данные боллинджера
            double rsi = iRSI.updata(close); // обновляем данные RSI

            if(close > iBB.tl && rsi > 80) {
                if(closeFuture < close) {
                    iEff.setWin(hour);
                    iEffL.setWin(hour);
                } else
                if(closeFuture > close) {
                    iEff.setLoss(hour);
                    iEffL.setLoss(hour);
                }
            } else
            if(close < iBB.bl && rsi < 20) {
                if(closeFuture > close) {
                    iEff.setWin(hour);
                    iEffL.setWin(hour);
                } else
                if(closeFuture < close) {
                    iEff.setLoss(hour);
                    iEffL.setLoss(hour);
                }
            }
        } // for
        std::cout << "Eff: " << iEffL.getEff() << " num win: " << iEffL.win << " num loss: " << iEffL.loss << std::endl;
        for(int i = 0; i < 24; i++) {
            double eff = (double)iEffL.hwin[i] / (double)(iEffL.hwin[i] + iEffL.hloss[i]);
            std::cout << "hour: " << i << " eff: " << eff << std::endl;
        }
    } // for
    for(int i = 0; i < 24; i++) {
        double eff = (double)iEff.hwin[i] / (double)(iEff.hwin[i] + iEff.hloss[i]);
        std::cout << "hour: " << i << " eff: " << eff << std::endl;
    }
    return 0;
}
