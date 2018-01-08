/*
* trading_indicators - Indicators for technical analysis.
*
* Copyright (c) 2018 Yaroslav Barabanov. Email: elektroyar@yandex.ru
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "TradingIndicators.hpp"

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

    SMM::SMM() {
        n = 10;
    }

    SMM::SMM(int period) {
        n = period;
    }

    double SMM::update(double input) {
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        std::vector<double> _data = data;
        std::sort(_data.rbegin(), _data.rend());
        double out = _data[_data.size()/2];
        return out;
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

    BollingerBands::BollingerBands() {
        n = 20;
        d = 2.0;
    }

    BollingerBands::BollingerBands(int period, double d) {
        BollingerBands::d = d;
        n = period;
    }

    void BollingerBands::update(double input) {
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        ml = std::accumulate(data.begin(), data.end(), 0);
        ml /= (double)n;
        double sum = 0;
        for (int i = 0; i < (int)data.size(); i++) {
            double diff = (data[i] - ml);
            sum +=  diff * diff;
        }
        double stdDev = std::sqrt(sum / (double)(data.size() - 1));
        stdDev *= d;
        tl = ml + stdDev;
        bl = ml - stdDev;
    }

    NoLagMa::NoLagMa() {
        nlm_values.resize(3);
    }

    NoLagMa::NoLagMa(int n) {
        LengthMA = n;
        nlm_values.resize(3);
    }

    double NoLagMa::_update(double price, int length, int r) {
        if((int)nlm_prices.size() != bars) {
            nlm_prices.resize(bars);
        }
        nlm_prices[r] = price;
        if (nlm_values[_length] != length) {
            double Cycle = 4.0;
            double Coeff = 3.0*Pi;
            int    Phase = length-1;

            nlm_values[_length] = length;
            nlm_values[_len] = length*4 + Phase;
            nlm_values[_weight] = 0;

            if (nlm_alphas.size() < nlm_values[_len]) {
                nlm_alphas.resize((int)nlm_values[_len]);
            }
            for (int k = 0; k< nlm_values[_len]; k++) {
                double t;
                if (k <= Phase-1) {
                    t = 1.0 * k/(Phase-1);
                } else {
                    t = 1.0 + (k - Phase + 1)*(2.0 * Cycle - 1.0)/(Cycle * length - 1.0);
                }
                double beta = cos(Pi*t);
                double g = 1.0/(Coeff*t+1);
                if (t <= 0.5 ) {g = 1;}

                nlm_alphas[k] = g * beta;
                nlm_values[_weight] += nlm_alphas[k];
            }
        }

        if (nlm_values[_weight]>0) {
            double sum = 0;
            for (int k=0; k < nlm_values[_len] && (r-k)>=0; k++) sum += nlm_alphas[k]*nlm_prices[r-k];
            return(sum / nlm_values[_weight]);
        }
        else return 0;
    }

    double NoLagMa::update(double in) {
        bars++;
        return _update(in, LengthMA, bars - 1);
    }

    FATL::FATL() {
        tick = 0;
        FATLPeriod = 39;
    }

    double FATL::update(double in) {
        tick++;
        if(tick < FATLPeriod) {
            for(int i = FATLPeriod - 1; i > 0; i--) {
                price[i] = price[i - 1];
            }
            price[0] = in;

            //std::cout << "no init: " << tick << " : " << SATLPeriod << std::endl;
            return in;
        }

        for(int i = FATLPeriod - 1; i > 0; i--) {
            price[i] = price[i - 1];
        }
        price[0] = in;

        prevOut = out;
        out = 0.4360409450 * price[0]
               + 0.3658689069 * price[1]
               + 0.2460452079 * price[2]
               + 0.1104506886 * price[3]
               - 0.0054034585 * price[4]
               - 0.0760367731 * price[5]
               - 0.0933058722 * price[6]
               - 0.0670110374 * price[7]
               - 0.0190795053 * price[8]
               + 0.0259609206 * price[9]
               + 0.0502044896 * price[10]
               + 0.0477818607 * price[11]
               + 0.0249252327 * price[12]
               - 0.0047706151 * price[13]
               - 0.0272432537 * price[14]
               - 0.0338917071 * price[15]
               - 0.0244141482 * price[16]
               - 0.0055774838 * price[17]
               + 0.0128149838 * price[18]
               + 0.0226522218 * price[19]
               + 0.0208778257 * price[20]
               + 0.0100299086 * price[21]
               - 0.0036771622 * price[22]
               - 0.0136744850 * price[23]
               - 0.0160483392 * price[24]
               - 0.0108597376 * price[25]
               - 0.0016060704 * price[26]
               + 0.0069480557 * price[27]
               + 0.0110573605 * price[28]
               + 0.0095711419 * price[29]
               + 0.0040444064 * price[30]
               - 0.0023824623 * price[31]
               - 0.0067093714 * price[32]
               - 0.0072003400 * price[33]
               - 0.0047717710 * price[34]
               + 0.0005541115 * price[35]
               + 0.0007860160 * price[36]
               + 0.0130129076 * price[37]
               + 0.0040364019 * price[38];
        if(out > prevOut) {trendState = 1;}
        else if(out < prevOut) {trendState = -1;} else {trendState = 0;}
        return out;
    }

    SATL::SATL() {
        tick = 0;
        SATLPeriod = 65;
    }

    double SATL::update(double in) {
        tick++;
        if(tick < SATLPeriod) {
            for(int i = SATLPeriod - 1; i > 0; i--) {
                price[i] = price[i - 1];
            }
            price[0] = in;

            //std::cout << "no init: " << tick << " : " << SATLPeriod << std::endl;
            return in;
        }

        for(int i = SATLPeriod - 1; i > 0; i--) {
            price[i] = price[i - 1];
        }
        price[0] = in;

        prevOut = out;
        out = 0.0982862174*price[0]
           +0.0975682269 * price[1]
           +0.0961401078 * price[2]
           +0.0940230544 * price[3]
           +0.0912437090 * price[4]
           +0.0878391006 * price[5]
           +0.0838544303 * price[6]
           +0.0793406350 * price[7]
           +0.0743569346 * price[8]
           +0.0689666682 * price[9]
           +0.0632381578 * price[10]
           +0.0572428925 * price[11]
           +0.0510534242 * price[12]
           +0.0447468229 * price[13]
           +0.0383959950 * price[14]
           +0.0320735368 * price[15]
           +0.0258537721 * price[16]
           +0.0198005183 * price[17]
           +0.0139807863 * price[18]
           +0.0084512448 * price[19]
           +0.0032639979 * price[20]
           -0.0015350359 * price[21]
           -0.0059060082 * price[22]
           -0.0098190256 * price[23]
           -0.0132507215 * price[24]
           -0.0161875265 * price[25]
           -0.0186164872 * price[26]
           -0.0205446727 * price[27]
           -0.0219739146 * price[28]
           -0.0229204861 * price[29]
           -0.0234080863 * price[30]
           -0.0234566315 * price[31]
           -0.0231017777 * price[32]
           -0.0223796900 * price[33]
           -0.0213300463 * price[34]
           -0.0199924534 * price[35]
           -0.0184126992 * price[36]
           -0.0166377699 * price[37]
           -0.0147139428 * price[38]
           -0.0126796776 * price[39]
           -0.0105938331 * price[40]
           -0.0084736770 * price[41]
           -0.0063841850 * price[42]
           -0.0043466731 * price[43]
           -0.0023956944 * price[44]
           -0.0005535180 * price[45]
           +0.0011421469 * price[46]
           +0.0026845693 * price[47]
           +0.0040471369 * price[48]
           +0.0052380201 * price[49]
           +0.0062194591 * price[50]
           +0.0070340085 * price[51]
           +0.0076266453 * price[52]
           +0.0080376628 * price[53]
           +0.0083037666 * price[54]
           +0.0083694798 * price[55]
           +0.0082901022 * price[56]
           +0.0080741359 * price[57]
           +0.0077543820 * price[58]
           +0.0073260526 * price[59]
           +0.0068163569 * price[60]
           +0.0062325477 * price[61]
           +0.0056078229 * price[62]
           +0.0049516078 * price[63]
           +0.0161380976 * price[64];
        if(out > prevOut) {trendState = 1;}
        else if(out < prevOut) {trendState = -1;} else {trendState = 0;}
        return out;
    }

    RBCI::RBCI() {
        RBCIPeriod=56;
        BBPeriod=100;
        iBB = BollingerBands(BBPeriod, BandsDeviation);
        RBCIPeriod = 56;
        tick = 0;
    }

    RBCI::RBCI(int BBPeriod, double BandsDeviation) {
        iBB = BollingerBands(BBPeriod, BandsDeviation);
        RBCIPeriod = 56;
        tick = 0;
    }


    double RBCI::update(double in) {
        tick++;
        if(tick < RBCIPeriod) {
            for(int i = RBCIPeriod - 1; i > 0; i--) {
                price[i] = price[i - 1];
            }
            price[0] = in;

            //std::cout << "no init: " << tick << " : " << SATLPeriod << std::endl;
            return in;
        }

        for(int i = RBCIPeriod - 1; i > 0; i--) {
            price[i] = price[i - 1];
        }
        price[0] = in;

        prevOut = out;
        out = -35.524181940 * price[0]
               -29.333989650 * price[1]
               -18.427744960 * price[2]
               -5.3418475670 * price[3]
               +7.0231636950 * price[4]
               +16.176281560 * price[5]
               +20.656621040 * price[6]
               +20.326611580 * price[7]
               +16.270239060 * price[8]
               +10.352401270 * price[9]
               +4.5964239920 * price[10]
               +0.5817527531 * price[11]
               -0.9559211961 * price[12]
               -0.2191111431 * price[13]
               +1.8617342810 * price[14]
               +4.0433304300 * price[15]
               +5.2342243280 * price[16]
               +4.8510862920 * price[17]
               +2.9604408870 * price[18]
               +0.1815496232 * price[19]
               -2.5919387010 * price[20]
               -4.5358834460 * price[21]
               -5.1808556950 * price[22]
               -4.5422535300 * price[23]
               -3.0671459820 * price[24]
               -1.4310126580 * price[25]
               -0.2740437883 * price[26]
               +0.0260722294 * price[27]
               -0.5359717954 * price[28]
               -1.6274916400 * price[29]
               -2.7322958560 * price[30]
               -3.3589596820 * price[31]
               -3.2216514550 * price[32]
               -2.3326257940 * price[33]
               -0.9760510577 * price[34]
               +0.4132650195 * price[35]
               +1.4202166770 * price[36]
               +1.7969987350 * price[37]
               +1.5412722800 * price[38]
               +0.8771442423 * price[39]
               +0.1561848839 * price[40]
               -0.2797065802 * price[41]
               -0.2245901578 * price[42]
               +0.3278853523 * price[43]
               +1.1887841480 * price[44]
               +2.0577410750 * price[45]
               +2.6270409820 * price[46]
               +2.6973742340 * price[47]
               +2.2289941280 * price[48]
               +1.3536792430 * price[49]
               +0.3089253193 * price[50]
               -0.6386689841 * price[51]
               -1.2766707670 * price[52]
               -1.5136918450 * price[53]
               -1.3775160780 * price[54]
               -1.6156173970 * price[55];
        if(out > prevOut) {trendState = 1;}
        else if(out < prevOut) {trendState = -1;} else {trendState = 0;}
        iBB.update(out);
        TL2 = iBB.tl;
        ML = iBB.ml;
        BL2 = iBB.bl;
        double BB = (TL2 - ML) / 2.0;
        TL1 = ML + BB;
        BL1 = ML - BB;

        return out;
    }

}
