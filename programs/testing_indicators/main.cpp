#include <iostream>
#include "TradingIndicators.hpp"
#include "ReadingData.hpp"

using namespace std;

int main()
{
    cout << "Test program" << endl;
    CurrencyQuote TestData("..//..//hist//EURUSD.txt");
    Indicators::EMA iEMA(10);
    for(int i = 0; i < (int)TestData.close.size(); i++) {
        double outEma = iEMA.update(TestData.close[i]);
        std::cout << "close: " << TestData.close[i] << " ema 10: " << outEma << " bars: " << i << "//" << TestData.close.size() << "\r";
    }
    return 0;
}
