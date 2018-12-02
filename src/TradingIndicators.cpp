/*
* trading_indicators - Indicators for technical analysis.
*
* Copyright (c) 2018 Elektro Yar. Email: git.electroyar@gmail.com
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
#include "NormalizationData.hpp"
#include <iostream>

namespace Indicators {

    SMA::SMA() {
        n = 10;
    }

    SMA::SMA(int period) {
        n = period;
    }

    double SMA::updata(double input) {
        #if(0)
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        double sum = std::accumulate(data.begin(), data.end(), double(0));
        sum /= (double)data.size();
        return sum;
        #else
        // рекурентная формула
        if((int)data.size() < n) {
            data.push_back(input);
            double sum = std::accumulate(data.begin(), data.end(), double(0));
            sum /= (double)data.size();
            if((int)data.size() == n) {
                prevData = sum;
            }
            return sum;
        } else {
            prevData -= (data[0] - input) / (double)n;
            data.push_back(input);
            data.erase(data.begin());
            return prevData;
        }
        #endif
    }

    double SMA::test(double input) {
        #if(0)
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        double sum = std::accumulate(data.begin(), data.end(), double(0));
        sum /= (double)data.size();
        return sum;
        #else
        std::vector<double> _data = data;
        // рекурентная формула
        if((int)_data.size() < n) {
            _data.push_back(input);
            double sum = std::accumulate(_data.begin(), _data.end(), double(0));
            sum /= (double)_data.size();

            return sum;
        } else {
            double _prevData = prevData - ((_data[0] - input) / (double)n);
            return _prevData;
        }
        #endif
    }

    EMA::EMA() {
        n = 10;
        a = 2.0/(double)(n + 1);
        isInit = false;
    }

    EMA::EMA(int period) {
        n = period;
        a = 2.0/(double)(n + 1);
        isInit = false;
    }

    double EMA::updata(double input) {
        if((int)data.size() < n) {
            data.push_back(input);
            double sum = std::accumulate(data.begin(), data.end(), double(0));
            sum /= (double)n;
            prevOut = sum;
            if((int)data.size() == n) isInit = true;
            return sum;
        }
        prevOut = a * input + (1.0 - a) * prevOut;
        return prevOut;
    }

    MMA::MMA() {
        n = 10;
        a = 1.0/(double)n;
    }

    MMA::MMA(int period) {
        n = period;
        a = 1.0/(double)n;
    }

    double MMA::updata(double input) {
        if((int)data.size() < n) {
            data.push_back(input);
            double sum = std::accumulate(data.begin(), data.end(), double(0));
            sum /= (double)n;
            prevOut = sum;
            return sum;
        }
        prevOut = a * input + (1.0 - a) * prevOut;
        return prevOut;
    }

    double MMA::test(double input) {
        std::vector<double> _data = data;
        if((int)_data.size() < n) {
            _data.push_back(input);
            double sum = std::accumulate(_data.begin(), _data.end(), double(0));
            sum /= (double)n;
            //prevOut = sum;
            return sum;
        }
        return a * input + (1.0 - a) * prevOut;
    }

    SMM::SMM() {
        n = 10;
    }

    SMM::SMM(int period) {
        n = period;
    }

    double SMM::updata(double input) {
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

    double RSI::updata(double input) {
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
        u = iU.updata(u);
        d = iD.updata(d);
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
        iU = MMA(n);
        iD = MMA(n);
    }

    WRSI::WRSI(int period) {
        n = period;
        iU = MMA(n);
        iD = MMA(n);
    }

    double WRSI::updata(double input) {
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
        u = iU.updata(u);
        d = iD.updata(d);
        prevInput = input;
        if (d == 0.0) {
            return 100.0;
        }
        double rs = u / d;
        double _rsi = 100.0 - (100.0 / (1.0 + rs));
        return _rsi;
    }

    double WRSI::test(double input) {
        if(isStart == false) {
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
        u = iU.test(u);
        d = iD.test(d);
        //prevInput = input;
        if (d == 0.0) {
            return 100.0;
        }
        double rs = u / d;
        double _rsi = 100.0 - (100.0 / (1.0 + rs));
        return _rsi;
    }

    SRSI::SRSI() {
        n = 5;
        iU = SMM(n);
        iD = SMM(n);
    }

    SRSI::SRSI(int period) {
        n = period;
        iU = SMM(n);
        iD = SMM(n);
    }

    double SRSI::updata(double input) {
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
        u = iU.updata(u);
        d = iD.updata(d);
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

    void BollingerBands::updata(double input) {
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        ml = std::accumulate(data.begin(), data.end(), double(0));
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

    void BollingerBands::test(double input) {
        std::vector<double> _data = data;
        _data.push_back(input);
        if((int)_data.size() > n) {
            _data.erase(_data.begin());
        }
        ml = std::accumulate(_data.begin(), _data.end(), double(0));
        ml /= (double)n;
        double sum = 0;
        for (int i = 0; i < (int)_data.size(); i++) {
            double diff = (data[i] - ml);
            sum +=  diff * diff;
        }
        double stdDev = std::sqrt(sum / (double)(_data.size() - 1));
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

    double NoLagMa::_updata(double price, int length, int r) {
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

    double NoLagMa::updata(double in) {
        bars++;
        return _updata(in, LengthMA, bars - 1);
    }

    FATL::FATL() {
        tick = 0;
        FATLPeriod = 39;
    }

    double FATL::updata(double in) {
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

    double SATL::updata(double in) {
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


    double RBCI::updata(double in) {
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
        iBB.updata(out);
        TL2 = iBB.tl;
        ML = iBB.ml;
        BL2 = iBB.bl;
        double BB = (TL2 - ML) / 2.0;
        TL1 = ML + BB;
        BL1 = ML - BB;

        return out;
    }

    SearchExtrema::SearchExtrema() {
        n = 10;
    }

    SearchExtrema::SearchExtrema(int nLastExtrema) {
        n = nLastExtrema;
    }

    SearchExtrema::SearchExtrema(int nLastExtrema, double extDeviation) {
        n = nLastExtrema;
        SearchExtrema::extDeviation = extDeviation;
    }

    void SearchExtrema::updata(double input) {
        if(isStart == true) {
            if(a <= b && b > input) {
                if(lastExtremums.size() > 0) {
                    if(b > lastExtremums.back() && isMinMax == 1) {
                        lastExtremums[lastExtremums.size() - 1] = b;
                    } else if(isMinMax == -1){
                        lastExtremums.push_back(b);
                        if((int)lastExtremums.size() > n) {
                            lastExtremums.erase(lastExtremums.begin());
                        }
                    }
                } else {
                    lastExtremums.push_back(b);
                    if((int)lastExtremums.size() > n) {
                        lastExtremums.erase(lastExtremums.begin());
                    }
                }
                isMinMax = 1;
            } else
            if(a >= b && b < input) {
                if(lastExtremums.size() > 0) {
                    if(b < lastExtremums.back() && isMinMax == -1) {
                        lastExtremums[lastExtremums.size() - 1] = b;
                    } else if(isMinMax == 1){
                        lastExtremums.push_back(b);
                        if((int)lastExtremums.size() > n) {
                            lastExtremums.erase(lastExtremums.begin());
                        }
                    }
                } else {
                    lastExtremums.push_back(b);
                    if((int)lastExtremums.size() > n) {
                        lastExtremums.erase(lastExtremums.begin());
                    }
                }
                isMinMax = -1;
            }
        } else {
            b = input; a = b;
            isStart = true;
            return;
        }
        a = b;
        b = input;
    }

    void SearchExtrema::updata(double high, double low) {
        if(isStart == true) {
            if((a <= b && b > high) || (a < b && b >= high)) {
                if(lastExtremums.size() > 0) {
                    double pDiff = std::min(b, lastExtremums.back())/std::max(b, lastExtremums.back());
                    if(pDiff > extDeviation) {
                        lastExtremums.push_back(b);
                        if((int)lastExtremums.size() > n) {
                            lastExtremums.erase(lastExtremums.begin());
                        }
                    }
                } else {
                    lastExtremums.push_back(b);
                    if((int)lastExtremums.size() > n) {
                        lastExtremums.erase(lastExtremums.begin());
                    }
                }
            } else
            if((a2 >= b2 && b2 < low) || (a2 > b2 && b2 <= low)) {
                if(lastExtremums.size() > 0) {
                    double pDiff = std::min(b2, lastExtremums.back())/std::max(b2, lastExtremums.back());
                    if(pDiff > extDeviation) {
                        lastExtremums.push_back(b2);
                        if((int)lastExtremums.size() > n) {
                            lastExtremums.erase(lastExtremums.begin());
                        }
                    }
                } else {
                    lastExtremums.push_back(b2);
                    if((int)lastExtremums.size() > n) {
                        lastExtremums.erase(lastExtremums.begin());
                    }
                }
            }
        } else {
            b = high; a = b;
            b2 = low; a2 = b2;
            isStart = true;
            return;
        }
        a = b;
        b = high;
        a2 = b2;
        b2 = low;
    }

#if(0)
    ZigZag::ZigZag() {};

    ZigZag::ZigZag(int extDepth, double extDeviation, int extBackstep, int nLastExtrema) {
        ZigZag::extDepth = extDepth;
        ZigZag::extDeviation = extDeviation;
        ZigZag::extBackstep = extBackstep;
        ZigZag::nLastExtrema = nLastExtrema;
    }

    void ZigZag::updata(double input) {
        vDepth.push_back(input);
        if((int)vDepth.size() > extDepth) {
            vDepth.erase(vDepth.begin());
            if(isMinimumSearch == 1) {
                lastMin = *std::min_element(vDepth.begin(), vDepth.end());
                isMinimumSearch = 0;
            } else {
                double minData = *std::min_element(vDepth.begin(), vDepth.end());
                double maxData = *std::max_element(vDepth.begin(), vDepth.end());
                double dMin = std::min(minData, lastMin) / std::max(minData, lastMin);
                double dMax = std::min(maxData, lastMax) / std::max(maxData, lastMax);
                if(minData < lastMin && dMin > extDeviation) {
                    lastMin = minData;
                } else
                if(maxData < lastMin && dMin > extDeviation) {
                    lastMin = minData;
                }
            }
        } else {
            lastMin = *std::min_element(vDepth.begin(), vDepth.end());
            lastMax = *std::max_element(vDepth.begin(), vDepth.end());
        }
    }

    void ZigZag::updata(double high, double low) {

    }

#endif

    Window::Window() {
        n = 10;
    }

    Window::Window(int period) {
        n = period;
    }

    std::vector<double> Window::updata(double input) {
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        return data;
    }

    SearchMinMax::SearchMinMax() {
        iWindow = Window(10);
    }

    SearchMinMax::SearchMinMax(int period) {
        iWindow = Window(period);
    }

    void SearchMinMax::updata(double input) {
        iWindow.updata(input);
        if(isStart == true) {
            minPrevData = minData;
            maxPrevData = maxData;
        }
        minData = *std::min_element(iWindow.data.begin(), iWindow.data.end());
        maxData = *std::max_element(iWindow.data.begin(), iWindow.data.end());
        if(isStart == false) {
            minPrevData = minData;
            maxPrevData = maxData;
            isStart = true;
        }
        if(minData != minPrevData) {
            isNewMin = true;
        } else {
            isNewMin = false;
        }
        if(maxData != maxPrevData) {
            isNewMax = true;
        } else {
            isNewMax = false;
        }
    }

    BasicExtrema::BasicExtrema() {};

    BasicExtrema::BasicExtrema(int nWinow, int nExtrema) {
        BasicExtrema::nWinow = nWinow;
        BasicExtrema::nExtrema = nExtrema;
    }

    void BasicExtrema::getExtrema(std::vector<double>& in, std::vector<double>& out, int dist = 1) {
        if(in.size() < 3) {
            return;
        }
        // находим экстрерумы
        std::vector<double> _out;
        _out.reserve(in.size()/3);
        int isPeak = 0;
        switch(dist) {
            case 1:
                for(int i = 1; i < (int)in.size() - 1; i++) {
                    double& a =  in[i - 1];
                    double& b =  in[i];
                    double& c =  in[i + 1];
                    if(b > a && isPeak <= 0) {
                        isPeak = 1;
                    }
                    if(b > c && isPeak == 1) {
                        isPeak = 2;
                    }
                    if(b < a && isPeak >= 0) {
                        isPeak = -1;
                    }
                    if(b < c && isPeak == -1) {
                        isPeak = -2;
                    }
                    if(isPeak == 2 || isPeak == -2) {
                        _out.push_back(b);
                        isPeak = 0;
                    }
                }
            break;
            case 2:
                for(int i = 2; i < (int)in.size() - 2; i++) {
                    double& a =  in[i - 2];
                    double& b =  in[i - 1];
                    double& c =  in[i];
                    double& d =  in[i + 1];
                    double& e =  in[i + 2];

                    if(c > a && c > b && c > d && c > e) {
                        _out.push_back(b);
                    }
                    if(c < a && c < b && c < d && c < e) {
                        _out.push_back(b);
                    }
                }
            break;
        }
        out = _out;
    }

    void BasicExtrema::updata(double input) {
        // собираем данные
        data.push_back(input);
        if((int)data.size() > nWinow) {
            data.erase(data.begin());
        } else {
            return;
        }
        // находим экстрерумы
        std::vector<double> vExtrema;
        std::vector<double> vExtrema2;
        std::vector<double> vExtrema3;
        std::vector<double> vExtrema4;
        //vExtrema.reserve(nWinow/3);
        getExtrema(data, vExtrema);
        getExtrema(vExtrema, vExtrema2, 2);
        getExtrema(vExtrema2, vExtrema3, 2);
        getExtrema(vExtrema3, vExtrema4, 2);

        vExtrema.push_back(input);
        vExtrema2.push_back(input);
        vExtrema3.push_back(input);
        vExtrema4.push_back(input);

        //std::vector<double> vNull;
        //Drawing::drawOscilloscope4xBeam("BasicExtrema", "BasicExtrema", vExtrema, vNull, vNull, vNull, 800, 400, 0);
        //Drawing::drawOscilloscope4xBeam("BasicExtrema - 2", "BasicExtrema - 2", vExtrema2, vNull, vNull, vNull, 800, 400, 0);
        //Drawing::drawOscilloscope4xBeam("BasicExtrema - 3", "BasicExtrema - 3", vExtrema3, vNull, vNull, vNull, 800, 400, 0);
        //Drawing::drawOscilloscope4xBeam("BasicExtrema - 4", "BasicExtrema - 4", vExtrema4, vNull, vNull, vNull, 800, 400, 0);
        //Drawing::drawOscilloscope4xBeam("BasicExtrema2", "BasicExtrema2", data, vNull, vNull, vNull, 800, 400, 0);
    }

    ExtremaDetector::ExtremaDetector() {isInit = false; state = 0;};

    void ExtremaDetector::updata(double input) {
        if(isInit == false) {
            prevInput = input;
            isInit = true;
            return;
        } else {
            // конечный автомат
            switch(state) {
                case 0:
                    // начальное состояние
                    if(input > prevInput) state = 1;
                    else if(input < prevInput) state = 2;
                    isUpdataExtremaUp = false;
                    isUpdataExtremaDown = false;
                break;
                case 1:
                    if(input < prevInput) {
                        isUpdataExtremaUp = true;
                        dataMax = prevInput;
                        state = 2;
                    } else {
                        isUpdataExtremaUp = false;
                        isUpdataExtremaDown = false;
                    }
                break;
                case 2:
                    if(input > prevInput) {
                        isUpdataExtremaDown = true;
                        dataMin = prevInput;
                        state = 1;
                    } else {
                        isUpdataExtremaUp = false;
                        isUpdataExtremaDown = false;
                    }
                break;
            }

            prevInput = input;
        }
    }


    LastExtrema::LastExtrema() {
        isInit = false;
    }

    LastExtrema::LastExtrema(int numExtrema, int period) {
        isInit = false;
        LastExtrema::numExtrema = numExtrema;
        iWindow = Window(period);
        iSmaMin = SMA(period);
        iSmaMax = SMA(period);
        SearchMinMax iSearchMinMax(period);
    }

    void LastExtrema::updata(double input) {
        if(isInit == false) {
            isInit = true;
            iSearchMinMax.updata(input);
            smaMin = iSmaMin.updata(iSearchMinMax.minData);
            smaMax = iSmaMax.updata(iSearchMinMax.maxData);
            prevSmaMin = smaMin;
            prevSmaMax = smaMax;
            return;
        } else {
            double a = prevSmaMin;
            double d = prevSmaMax;
            prevSmaMin = smaMin;
            prevSmaMax = smaMax;
            double& b = prevSmaMin;
            double& e = prevSmaMax;
            iSearchMinMax.updata(input);
            smaMin = iSmaMin.updata(iSearchMinMax.minData);
            smaMax = iSmaMax.updata(iSearchMinMax.maxData);
            double& c = smaMin;
            double& f = smaMax;

            state = 0;
            switch(stateMin) {
                case 0:
                    if(b < a && b < c) state = 2;
                    else if(b < a) stateMin = 1;
                break;
                case 1:
                    if(b < c) {stateMin = 0; state = 2;}
                break;
            }
            switch(stateMax) {
                case 0:
                    if(e > d && e > f) state = 1;
                    else if(e > d) stateMax = 1;
                break;
                case 1:
                    if(e > f) {stateMax = 0; state = 1;}
                break;
            }

            // конечный автомат
            isUpdataExtremaUp = false;
            isUpdataExtremaDown = false;

            switch(state) {
                case 1:
                    isUpdataExtremaUp = true;
                    vExtremaUp.push_back(prevSmaMax);
                    vExtrema.push_back(prevSmaMax);
                break;
                case 2:
                    isUpdataExtremaDown = true;
                    vExtremaDown.push_back(prevSmaMin);
                    vExtrema.push_back(prevSmaMin);
                break;
            }
            if((int)vExtremaUp.size() > numExtrema) {
                vExtremaUp.erase(vExtremaUp.begin());
                initStep |= 0x01;
            }
            if((int)vExtremaDown.size() > numExtrema) {
                vExtremaDown.erase(vExtremaDown.begin());
                initStep |= 0x02;
            }
            if((int)vExtrema.size() > numExtrema) {
                vExtrema.erase(vExtrema.begin());
                initStep |= 0x04;
            }
        }
    }

    LowPassFilter::LowPassFilter() {};

    LowPassFilter::LowPassFilter(double tranTime, double period, double errorSignal) {
        double N = tranTime / period;
        double Ntay = std::log(1.0/errorSignal);
        alfa = std::exp(-Ntay/N);
        beta = 1.0 - alfa;
        LowPassFilter::tranTime = tranTime;
    }

    LowPassFilter::LowPassFilter(double n, double errorSignal) {
        double Ntay = std::log(1.0/errorSignal);
        alfa = std::exp(-Ntay/n);
        beta = 1.0 - alfa;
        LowPassFilter::tranTime = n;
    }

    LowPassFilter::LowPassFilter(double n) {
        double Ntay = std::log(1.0/0.01);
        alfa = std::exp(-Ntay/n);
        beta = 1.0 - alfa;
        LowPassFilter::tranTime = n;
    }

    double LowPassFilter::updata(double in) {
        if (isStart == 0) {
            prevY = in;
            isStart = 1;
        }
        double y = alfa*prevY + beta*in;
        prevY = y;
        return y;
    }

    PsychologicalLevel::PsychologicalLevel() {
        iWindow = Window(30);
        nLevel = 5;
    };

    PsychologicalLevel::PsychologicalLevel(int nLevel) {
        iWindow = Window(30);
        PsychologicalLevel::nLevel = nLevel;
    }

    PsychologicalLevel::PsychologicalLevel(int nLevel, int nWindow) {
        iWindow = Window(nWindow);
        PsychologicalLevel::nLevel = nLevel;
    }

    void PsychologicalLevel::updata(double input) {
        if(isFactor == false) iWindow.updata(input);
        if((int)iWindow.data.size() == iWindow.getPeriod()) {
            if(isFactor == false) {
                factor = Normalization::getNumberDecimals(iWindow.data, true);
                diffMin = (double)nLevel/(double)factor;
                isFactor = true;
            }
            //std::cout << "factor: " << factor << std::endl;
            //std::cout << "input: " << input << std::endl;
            double nearestLevel = ((double)((int)((double)factor * input) / 100) * 100.0) / (double)factor;

            double level100Up = input > nearestLevel ? nearestLevel + 100.0/(double)factor : nearestLevel;
            double level100Down = input < nearestLevel ? nearestLevel - 100.0/(double)factor : nearestLevel;
            double level80 = nearestLevel - 20.0/(double)factor;
            double level20 = nearestLevel + 20.0/(double)factor;
            double level50 = input > nearestLevel ? nearestLevel + 50.0/(double)factor : nearestLevel - 50.0/(double)factor;

            double diff100Up, diff100Down, diff50, diff80, diff20;

            diff100Up = std::abs(input - level100Up);
            diff100Down = std::abs(input - level100Down);
            diff50 = std::abs(input - level50);
            diff80 = std::abs(input - level80);
            diff20 = std::abs(input - level20);

            if(diff100Up < diff100Down && diff100Up < diff50 && diff100Up < diff80 && diff100Up < diff20) {
                if(diffMin >= diff100Up) {
                    isLevel100 = true;
                    isLevel80 = false;
                    isLevel50 = false;
                    isLevel20 = false;
                }
            } else
            if(diff100Down < diff100Up && diff100Down < diff50 && diff100Down < diff80 && diff100Down < diff20) {
                if(diffMin >= diff100Down) {
                    isLevel100 = true;
                    isLevel80 = false;
                    isLevel50 = false;
                    isLevel20 = false;
                }
            } else
            if(diff80 < diff100Up && diff80 < diff50 && diff80 < diff100Down && diff80 < diff20) {
                if(diffMin >= diff80) {
                    isLevel100 = false;
                    isLevel80 = true;
                    isLevel50 = false;
                    isLevel20 = false;
                }
            } else
            if(diff50 < diff100Up && diff50 < diff80 && diff50 < diff100Down && diff50 < diff20) {
                if(diffMin >= diff50) {
                    isLevel100 = false;
                    isLevel80 = false;
                    isLevel50 = true;
                    isLevel20 = false;
                }
            } else
            if(diff20 < diff100Up && diff20 < diff80 && diff20 < diff100Down && diff20 < diff50) {
                if(diffMin >= diff20) {
                    isLevel100 = false;
                    isLevel80 = false;
                    isLevel50 = false;
                    isLevel20 = true;
                }
            } else {
                isLevel100 = false;
                isLevel80 = false;
                isLevel50 = false;
                isLevel20 = false;
            }
            //std::cout << "nearest level: " << nearestLevel << std::endl;
        }
    }

    void getMinMaxBands(std::vector<double>& input, std::vector<double>& vMin, std::vector<double>& vMax, int period, int offset) {
        Indicators::SMA iSmaMin(period);
        Indicators::SMA iSmaMax(period);
        Indicators::SearchMinMax iSearchMinMax(period);
        vMin.resize(input.size());
        vMax.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            iSearchMinMax.updata(input[i]);
            vMin[i] = iSmaMin.updata(iSearchMinMax.minData);
            vMax[i] = iSmaMax.updata(iSearchMinMax.maxData);
        }
        for(int i = 0; i < (int)input.size() - offset; i++) {
            vMin[i] = vMin[i + offset - 1];
            vMax[i] = vMax[i + offset - 1];
        }
    }

    ShannonEntropy::ShannonEntropy() {};

    ShannonEntropy::ShannonEntropy(int period) {
        iWindow = Window(period + 1);
        maxEntropy = log((double)period);
    }

    double ShannonEntropy::updata(double input) {
        iWindow.updata(input);
        if((int)iWindow.data.size() == iWindow.getPeriod()) {
            const int binsCount = 3;
            double hist[binsCount];
            for (int i = 0; i < binsCount; i++) {hist[i] = 0.0;};
            std::vector<double> vInput;
            Normalization::calcDifference(iWindow.data, vInput);
            for (int i = 0; i < (int)vInput.size(); i++) {
                int index = vInput[i] < 0.0 ? 0 : vInput[i] > 0.0 ? 2 : 1;
                hist[index] = hist[index] + 1.0;
            }
            for (int i = 0; i < binsCount; i++) {
                hist[i] = hist[i] / (double)vInput.size();
            }
            double h = 0.0;
            for (int i = 0; i < binsCount; i++) {
                if (hist[i] > std::numeric_limits<double>::min()) {
                    h += hist[i] * log2(hist[i]);
                }
            }
            h = -h;
            return h/maxEntropy;
        }
        return 0.0;
    }

    PriceMovementStatistics::PriceMovementStatistics() {};

    PriceMovementStatistics::PriceMovementStatistics(int period, int nHist) {
        iWindow = Window(period);
        PriceMovementStatistics::nHist = nHist;
        PriceMovementStatistics::period = period;
        hist.reserve(nHist);
    }

    void PriceMovementStatistics::updata(double in) {
        iWindow.updata(in);
        if((int)iWindow.data.size() == iWindow.getPeriod()) {
            hist.clear();
            hist.resize(nHist);
            double minData = *std::min_element(iWindow.data.begin(), iWindow.data.end());
            double maxData = *std::max_element(iWindow.data.begin(), iWindow.data.end());
            if(minData == maxData) {
                pos = 0.5;
                for(int i = 0; i < nHist; i++) {
                    hist[i] = i != nHist/2 ? 0.0 : 1.0;
                }
                up = 0; down = 0; neutral = 1;
                return;
            }
            double gain = (double) (nHist - 1) / (maxData - minData);
            for(int i = 0; i < (int)iWindow.data.size(); i++) {
                int index = std::floor(gain * (iWindow.data[i] - minData));
                hist[index] = hist[index] + 1.0;
            }
            Normalization::calcMinMax(hist, hist, 0);
            double maxHist = 0;
            int setPos = 0;
            double sum = 0;
            for(int i = 0; i < nHist; i++) {
                if(hist[i] > maxHist) {
                    maxHist = hist[i];
                    setPos = i;
                }
                sum += hist[i];
            }
            pos = (double)setPos / (double)(nHist - 1);
            // для прогноза вверх вниз
            up = 0; down = 0; neutral = 0;
            for(int i = 0; i < std::floor(nHist/2); i++) {
                down += hist[i];
            }
            for(int i = std::ceil(nHist/2); i < nHist; i++) {
                up += hist[i];
            }
            for(int i = std::floor(nHist/3); i < std::ceil(2*nHist/3); i++) {
                neutral += hist[i];
            }
            up /= sum;
            down /= sum;
            neutral /= sum;
        } else {
            hist.clear();
            hist.resize(nHist);
            pos = 0.5;
            for(int i = 0; i < nHist; i++) {
                hist[i] = i != nHist/2 ? 0.0 : 1.0;
            }
            up = 0; down = 0; neutral = 1;
        }
    }

    TrendIndicator::TrendIndicator() {};

    TrendIndicator::TrendIndicator(int period) {
        iWindow = Window(period);
    };

    double TrendIndicator::updata(double input) {
        iWindow.updata(input);
        if(iWindow.getPeriod() == (int)iWindow.data.size()) {
            Normalization::calcDifference(iWindow.data, diff);
            double mean = Normalization::calcMean(diff);
            double stdDev = Normalization::calcStdDev(diff, mean);
            if(mean == 0) return 0;
            double percent = stdDev / mean;
            if(percent > 1.0 || percent < -1.0) percent = 0.0;
            percent = percent > 0.0 ? 1.0 - percent : percent < 0.0 ? -(percent + 1.0) : 0.0;
            return percent;
        }
        return 0;
    }

    StandardDeviation::StandardDeviation() {
        n = 20;
    };

    StandardDeviation::StandardDeviation(int period) {
        n = period;
    }

    double StandardDeviation::updata(double input) {
        data.push_back(input);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        double ml = std::accumulate(data.begin(), data.end(), double(0));
        ml /= (double)n;
        double sum = 0;
        for (int i = 0; i < (int)data.size(); i++) {
            double diff = (data[i] - ml);
            sum +=  diff * diff;
        }
        double stdDev = std::sqrt(sum / (double)(data.size() - 1));
        return stdDev;
    }

    NewTimeFrame::NewTimeFrame() {};

    NewTimeFrame::NewTimeFrame(ePeriod period) {
        iPeriod = period;
    }

    int NewTimeFrame::updata(double in, int minutes) {
        if(minutes % iPeriod == 0) {
            out = in;
            return 1;
        }
        return 0;
    }

    ATR::ATR() {n = 0; isInit = false;}

    ATR::ATR(int period) {
        isInit = false;
        n = period;
        iEmaAtr = EMA(period);
    }

    double ATR::updata(double high, double low, double close) {
        if(isInit == false) {
            isInit = true;
            prevClose = close;
            TRj = 0;
            return 0;
        }
        TRj = std::max(std::abs(high - low),std::max(std::abs(low - prevClose),std::abs(high - prevClose)));
        prevClose = close;
        TRj = iEmaAtr.updata(TRj);
        return TRj;
    }

    ADX::ADX() {isInit = false; n = 0;};

    ADX::ADX(int period) {
        iEmaP = EMA(period);
        iEmaM = EMA(period);
        iEmaAdx = EMA(period);
        n = period;
        isInit = false;
    }

    double ADX::updata(double high, double low, double close) {
        if(isInit == false) {
            isInit = true;
            prevHigh = high;
            prevLow = low;
            prevClose = close;
            adxj = 0;
            return 0;
        }
        double pDMj;
        double mDMj;
        if(high > prevHigh) {
            pDMj = high - prevHigh;
        } else {
            pDMj = 0;
        }
        if(low < prevLow) {
            mDMj = prevLow - low;
        } else {
            mDMj = 0;
        }
        if(pDMj > mDMj) mDMj = 0;
        else if(mDMj > pDMj) pDMj = 0;
        else if(mDMj == pDMj) {pDMj = 0; mDMj = 0;};

        double TRj = std::max(std::abs(high - low),std::max(std::abs(low - prevClose),std::abs(high - prevClose)));
        double pSDI = 0;
        double mSDI = 0;
        if(TRj != 0) {
            pSDI = pDMj / TRj;
            mSDI = mDMj / TRj;
        }
        double pDIj = iEmaP.updata(pSDI);
        double mDIj = iEmaM.updata(mSDI);
        double sumDIj = pDIj + mDIj;
        double DXj = sumDIj != 0 ? (std::abs(pDIj - mDIj) / (pDIj + mDIj)) * 100.0 : 0;
        adxj = iEmaAdx.updata(DXj);
        prevHigh = high;
        prevLow = low;
        prevClose = close;
        return adxj;
    }

    VMA::VMA() {
        n = 10;
    }

    VMA::VMA(int period) {
        n = period;
    }

    double VMA::updata(double in, double vol) {
        data.push_back(in);
        w.push_back(vol);
        if((int)data.size() > n) {
            data.erase(data.begin());
            w.erase(w.begin());
        }
        double sum = 0;
        for(int i = 0; i < (int)data.size(); i++) {
            sum += data[i] * w[i];
        }
        double sum2 = std::accumulate(w.begin(), w.end(), double(0));
        sum /= sum2;
        return sum;
    }

    DMA::DMA() {
        n = 10;
        a = 2.0/(double)(n + 1);
        iEMA = EMA(n);
        isInit = false;
    }

    DMA::DMA(int period) {
        n = period;
        a = 2.0/(double)(n + 1);
        iEMA = EMA(n);
        isInit = false;
    }

    double DMA::updata(double in) {
        double ema = iEMA.updata(in);
        if(isInit == false) {
            if(iEMA.isInit == true) {
                prevOut = ema;
                isInit = true;
                return ema;
            } else {
                return ema;
            }
        } else {
            prevOut = a * ema + (1.0 - a) * prevOut;
            return prevOut;
        }
        return ema;
    }

    TMA::TMA() {
        n = 10;
        a = 2.0/(double)(n + 1);
        iDMA = DMA(n);
        isInit = false;
    }

    TMA::TMA(int period) {
        n = period;
        a = 2.0/(double)(n + 1);
        iDMA = DMA(n);
        isInit = false;
    }

    double TMA::updata(double in) {
        double dma = iDMA.updata(in);
        if(isInit == false) {
            if(iDMA.isInit == true) {
                prevOut = dma;
                isInit = true;
                return dma;
            } else {
                return dma;
            }
        } else {
            prevOut = a * dma + (1.0 - a) * prevOut;
            return prevOut;
        }
        return dma;
    }

    WMA::WMA() {n = 10;};

    WMA::WMA(int period) {n = period;};

    double WMA::updata(double in) {
        data.push_back(in);
        if((int)data.size() > n) {
            data.erase(data.begin());
        }
        double sum = 0;
        for(int i = (int)data.size(); i > 0; i--) {
            sum += data[i - 1] * i;
        }
        sum = (sum * 2.0) / ((double)data.size() * ((double)data.size() + 1.0));
        return sum;
    }

    CA::CA() {n = 0; prevData = 0;};

    double CA::updata(double in) {
        n++;
        prevData = prevData + (in - prevData) / n;
        return prevData;
    }

    AMA::AMA() {n = 10; f = 2; s = 30;
        isX2 = true;
        iSD = StandardDeviation(n);
        _K = 0.1;
        filter = 0;
        state = 0;
    };

    AMA::AMA(int n, int f, int s, int d, double K) {
        AMA::n = n;
        AMA::f = f;
        AMA::s = s;
        iSD = StandardDeviation(d);
        _K = K;
        isX2 = true;
        filter = 0;
        state = 0;
    }

    double AMA::updata(double in) {
        data.push_back(in);
        if((int)data.size() > n) {
            data.erase(data.begin());
            //
            double _dir = std::abs(data[n - 1] - data[0]);
            double _vol = 0;
            for(int i = n - 1; i > 0; i--) {
                _vol += std::abs(data[i] - data[i - 1]);
            }
            double er = _dir/_vol;
            double _fastest = 2.0/(double)(f + 1);
            double _slowest = 2.0/(double)(s + 1);
            double _smooth = er * (_fastest - _slowest) + _slowest;
            double _c = isX2 == true ? _smooth * _smooth : _smooth;

            double _prevAMA = _c * in + (1.0 - _c) * prevAMA;

            double di = _prevAMA - prevAMA;
            prevAMA = _prevAMA;
            filter = _K * iSD.updata(di);
            return prevAMA;
        } else {
            filter = 0;
            state = 0;
            return in;
        }
    }

    SMMA::SMMA() {
        n = 10;
    }

    SMMA::SMMA(int n) {
        SMMA::n = n;
    }

    double SMMA::updata(double in) {
        if((int)data.size() < n) {
            data.push_back(in);
            double sma = std::accumulate(data.begin(), data.end(), double(0));
            sma /= (double)(data.size());
            if((int)data.size() == n) {
                double smma1 = sma;
                smma2 = (smma1 * (double)(n - 1) + in) / (double)n;
                //isInit = true;
                return smma2;
            }
            return sma;
        } else {
            double prevsum = smma2 * (double)n;
            smma2 = (prevsum -  smma2 + in) / (double)n;
            return smma2;
        }
        return 0;
    }

    FRAMA::FRAMA() {
        n = 10;
        isUseHighLow = true;
        isFlexFrama = false;
    };

    FRAMA::FRAMA(int n) {
        FRAMA::n = n;
        isUseHighLow = true;
        isFlexFrama = false;
    }

    FRAMA::FRAMA(int n, bool isUseHighLow) {
        FRAMA::n = n;
        FRAMA::isUseHighLow = isUseHighLow;
        isFlexFrama = false;
    }

    double FRAMA::updata(double in, double high, double low) {
        vdata.push_back(in);
        vhigh.push_back(high);
        vlow.push_back(low);
        if((int)vdata.size() > n) {
            vdata.erase(vdata.begin());
            vhigh.erase(vhigh.begin());
            vlow.erase(vlow.begin());
        } else {
            return in;
        }
        double N1, N2, N3;
        if(isUseHighLow) {
            double maxData1 = *std::max_element(vhigh.begin(), vhigh.begin() + n/2);
            double maxData2 = *std::max_element(vhigh.begin() + n/2, vhigh.end());
            double maxData3 = *std::max_element(vhigh.begin(), vhigh.end());
            double minData1 = *std::min_element(vlow.begin(), vlow.begin() + n/2);
            double minData2 = *std::min_element(vlow.begin() + n/2, vlow.end());
            double minData3 = *std::min_element(vlow.begin(), vlow.end());
            N1 = (maxData1 - minData1) / (double)(n/2);
            N2 = (maxData2 - minData2) / (double)(n/2);
            N3 = (maxData3 - minData3) / (double)n;
        } else {
            double maxData1 = *std::max_element(vdata.begin(), vdata.begin() + n/2);
            double maxData2 = *std::max_element(vdata.begin() + n/2, vdata.end());
            double maxData3 = *std::max_element(vdata.begin(), vdata.end());
            double minData1 = *std::min_element(vdata.begin(), vdata.begin() + n/2);
            double minData2 = *std::min_element(vdata.begin() + n/2, vdata.end());
            double minData3 = *std::min_element(vdata.begin(), vdata.end());
            N1 = (maxData1 - minData1) / (double)(n/2);
            N2 = (maxData2 - minData2) / (double)(n/2);
            N3 = (maxData3 - minData3) / (double)n;
        }

        double D = (log(N1 + N2) - log(N3)) / log(2.0);
        if(isFlexFrama == true) {
            coeff = -(3.89 * exp(D) - 9.57);
        }
        double A = exp(-coeff*(D - 1.0));
        prevFRAMA = A * in + (1.0 - A) * prevFRAMA;
        return prevFRAMA;
    }

    HMA::HMA() {
        n = 10;
        iWMAn = WMA(n);
        iWMAn2 = WMA(n/2);
        iWMAsqrtn = WMA(sqrt(n));
    };

    HMA::HMA(int n) {
        HMA::n = n;
        iWMAn = WMA(n);
        iWMAn2 = WMA(n/2);
        iWMAsqrtn = WMA(sqrt(n));
    };

    double HMA::updata(double in) {
        double wmaN = iWMAn.updata(in);
        double wmaN2 = iWMAn2.updata(in);
        return  iWMAsqrtn.updata(2.0 * wmaN2 - wmaN);
    }

    VIDYA::VIDYA() {
        n = 10;
        isStart = false;
        F = 2.0 / (double)(n + 1.0);
    };

    VIDYA::VIDYA(int n) {
        VIDYA::n = n;
        isStart = false;
        F = 2.0 / (double)(n + 1.0);
    }

    double VIDYA::updata(double in) {
        if(isStart == false) {
            prevInput = in;
            prevVIDYA = in;
            isStart = true;
            return in;
        }
        double u = 0, d = 0;
        if (prevInput < in) {
            u = in - prevInput;
            d = 0.0;
        } else
        if (prevInput > in) {
            d = prevInput - in;
            u = 0.0;
        }
        u = iSumUp.updata(u);
        d = iSumDn.updata(d);
        double CMO = (u - d) / (u + d);
        prevVIDYA = in * F * abs(CMO + prevVIDYA * (1.0 - (F * abs(CMO))));
        prevInput = in;
        return prevVIDYA;
    }

    TrendFlatIndicator::TrendFlatIndicator() {};

    TrendFlatIndicator::TrendFlatIndicator(int _n, int nFilter, double dX) {
        iLastExtrema = LastExtrema(2,_n);
        iEMA = EMA(nFilter);
        TrendFlatIndicator::dX = dX;
    }

    double TrendFlatIndicator::updata(double in) {
        iLastExtrema.updata(in);
        if(iLastExtrema.vExtremaUp.size() == 2 && iLastExtrema.vExtremaDown.size() == 2) {
            double& maxData = iLastExtrema.vExtremaUp[1];
            double& minData = iLastExtrema.vExtremaDown[1];
            if(in > maxData && in > minData) {
                trendData += dX;
            } else
            if(in < maxData && in < minData) {
                trendData -= dX;
            } else
            if(maxData > iLastExtrema.vExtremaUp[0] && in > maxData) {
                if(minData > iLastExtrema.vExtremaDown[0]) {
                    trendData += dX;
                } else {
                    trendData = 0.0;
                    if(in > maxData) {
                        trendData += dX;
                    }
                }
            } else
            if(maxData < iLastExtrema.vExtremaUp[0]) {
                if(minData < iLastExtrema.vExtremaDown[0] && in < minData) {
                    trendData -= dX;
                } else {
                    trendData = 0.0;
                    if(in < minData) {
                        trendData -= dX;
                    }
                }
            } else {
                trendData = 0.0;
                if(in > maxData) {
                    trendData = dX;
                } else
                if(in < minData) {
                    trendData -= dX;
                }
            }
        } else {
            trendData = 0.0;
        }
        if(trendData > 1.0) trendData = 1.0;
        else if(trendData < -1.0) trendData = -1.0;
        double out = iEMA.updata(trendData);
        return out;
    }

}
