#include <iostream>
#include "TradingIndicators.hpp"
#include "ReadingData.hpp"
#include "Drawing.hpp"

using namespace std;

int main()
{
    cout << "Test program" << endl;
    CurrencyQuote TestData("..//..//hist//EURUSD.txt");
    Indicators::EMA iEMA(10);
    Indicators::SearchExtrema iExtrema(30);
    Drawing::Window iWindow(30);
    for(int i = 1440*365*10; i < (int)TestData.close.size(); i++) {
        double& open = TestData.open[i];
        double& high = TestData.high[i];
        double& low = TestData.low[i];
        double& close = TestData.close[i];

        double outEma = iEMA.update(close);
        iExtrema.update(high,low);

        iWindow.update(open, high, low, close);
        iWindow.setIndicator(outEma);

        std::vector<Drawing::CandlesType> vCandles = iWindow.getCandlesType();
        Drawing::viewCandleGraph("test", vCandles, 0);

        std::cout << "close: " << TestData.close[i] << " ema 10: " << outEma << " bars: " << i << "//" << TestData.close.size() << "\r";
    }
    return 0;
}
