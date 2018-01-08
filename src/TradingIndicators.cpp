#include "TradingIndicators.hpp"
#include <numeric>

namespace Indicators {

    SMA::SMA() {
        n = 10;
    }

    SMA::SMA(int period) {
        n = period;
    }

    double SMA::update(double input) {
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        double sum = std::accumulate(data.begin(), data.end(), 0);
        sum /= (double)n;
        return sum;
    }

    EMA::EMA() {
        n = 10;
        a = 2.0/(double)(n + 1);
    }

    EMA::EMA(int period) {
        n = period;
        a = 2.0/(double)(n + 1);
    }

    double EMA::update(double input) {
        if((int)data.size() < n) {
            data.push_back(input);
            double sum = std::accumulate(data.begin(), data.end(), 0);
            sum /= (double)n;
            prevOut = sum;
            return sum;
        }
        prevOut = a * input + (1.0 - a) * prevOut;
        return prevOut;
    }

    WMA::WMA() {
        n = 10;
        a = 1.0/(double)n;
    }

    WMA::WMA(int period) {
        n = period;
        a = 1.0/(double)n;
    }

    double WMA::update(double input) {
        if((int)data.size() < n) {
            data.push_back(input);
            double sum = std::accumulate(data.begin(), data.end(), 0);
            sum /= (double)n;
            prevOut = sum;
            return sum;
        }
        prevOut = a * input + (1.0 - a) * prevOut;
        return prevOut;
    }

    RSI::RSI() {
        n = 5;
        iU = SMA(n);
        iD = SMA(n);
    }

    RSI::RSI(int period) {
        n = period;
        iU = SMA(n);
        iD = SMA(n);
    }

    double RSI::update(double input) {
        if(isStart == false) {
            prevInput = input;
            isStart = true;
            return 50.0;
        }
        double u = 0, d = 0;
        if (prevInput < input) {
            u = input - prevInput;
            d = 0.0;
        } else
        if (prevInput > input) {
            d = prevInput - input;
            u = 0.0;
        }
        u = iU.update(u);
        d = iD.update(d);
        prevInput = input;
        if (d == 0.0) {
            return 100.0;
        }
        double rs = u / d;
        double _rsi = 100.0 - (100.0 / (1.0 + rs));
        return _rsi;
    }

    WRSI::WRSI() {
        n = 5;
        iU = WMA(n);
        iD = WMA(n);
    }

    WRSI::WRSI(int period) {
        n = period;
        iU = WMA(n);
        iD = WMA(n);
    }

    double WRSI::update(double input) {
        if(isStart == false) {
            prevInput = input;
            isStart = true;
            return 50.0;
        }
        double u = 0, d = 0;
        if (prevInput < input) {
            u = input - prevInput;
            d = 0.0;
        } else
        if (prevInput > input) {
            d = prevInput - input;
            u = 0.0;
        }
        u = iU.update(u);
        d = iD.update(d);
        prevInput = input;
        if (d == 0.0) {
            return 100.0;
        }
        double rs = u / d;
        double _rsi = 100.0 - (100.0 / (1.0 + rs));
        return _rsi;
    }

}
