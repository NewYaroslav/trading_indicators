#ifndef TRADINGINDICATORS_HPP_INCLUDED
#define TRADINGINDICATORS_HPP_INCLUDED

#include <vector>

namespace Indicators {

    class SMA {
        private:
        int n;
        std::vector<double> data;
        public:
        SMA();
        SMA(int period);
        double update(double input);
    };

    class EMA {
        private:
        int n;
        double a;
        double prevOut;
        std::vector<double> data;
        public:
        EMA();
        EMA(int period);
        double update(double input);
    };

    class WMA {
        private:
        int n;
        double a;
        double prevOut;
        std::vector<double> data;
        public:
        WMA();
        WMA(int period);
        double update(double input);
    };

    class RSI {
        private:
        SMA iU;
        SMA iD;
        int n;
        double prevInput;
        public:
        bool isStart = false;
        RSI();
        RSI(int period);
        double update(double input);
    };

    class WRSI {
        private:
        WMA iU;
        WMA iD;
        int n;
        double prevInput;
        public:
        bool isStart = false;
        WRSI();
        WRSI(int period);
        double update(double input);
    };

}

#endif // TRADINGINDICATORS_HPP_INCLUDED
