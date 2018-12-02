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

#include "NormalizationData.hpp"

namespace Normalization {
    BollingerBandsNorm::BollingerBandsNorm() {isInit = false;};

    BollingerBandsNorm::BollingerBandsNorm(int n, double d) {
        iBands = Indicators::BollingerBands(n, d);
        isInit = true;
    }

    double BollingerBandsNorm::updata(double input) {
        if(isInit == true) {
            iBands.updata(input);
            if(input > iBands.tl) return 1.0;
            else if(input < iBands.bl) return -1.0;
            if(input > iBands.ml) {
                return (input - iBands.ml) / (iBands.tl - iBands.ml);
            } else
            if(input < iBands.ml) {
                return -((iBands.ml - input) / (iBands.ml - iBands.bl));
            }
        }
        return 0.0;
    }

    std::vector<double> smoothOutData(std::vector<double>& input, int period) {
        Indicators::SMA iSMA(period);
        std::vector<double> output(input.size());
        for(size_t i = 0; i < input.size(); i++) {
            output[i] = iSMA.updata(input[i]);
        }
        size_t outSize = input.size() - period - 1;
        for(size_t i = 0; i < outSize; i++) {
            output[i] = output[i + period - 1];
        }
        output.resize(outSize);
        return output;
    }

    std::vector<double> smoothDataLoop(std::vector<double>& input, int period) {
        Indicators::SMA iSMA(period);
        std::vector<double> data;
        data.insert(data.end(), input.begin(), input.end());
        data.insert(data.end(), input.begin(), input.end());
        data.insert(data.end(), input.begin(), input.end());
        std::vector<double> _output(data.size());
        std::vector<double> output(input.size());
        for(size_t i = 0; i < data.size(); i++) {
            _output[i] = iSMA.updata(data[i]);
        }
        size_t stop = input.size() * 2;
        for(size_t i = input.size(); i < stop; i++) {
            output[i - input.size()] = _output[i + period - 1];
        }

        return output;
    }

    int getMax(std::vector<double>& input, int pos) {
        int num = 0;
        for(size_t i = 1; i < input.size() - 1; i++) {
            if(input[i] > input[i - 1] && input[i] > input[i + 1]) {
                if(pos == num) {
                    return i;
                } else {
                    num++;
                }
            }
        }
        return -1;
    }

    std::vector<double> DirectTransform(std::vector<double> SourceList) {
        if(SourceList.size() == 1)
            return SourceList;

        std::vector<double> RetVal;
        std::vector<double> TmpArr;

        for (int j = 0; j < (int)SourceList.size() - 1; j+=2) {
            RetVal.push_back((SourceList[j] - SourceList[j + 1]) / 2.0);
            TmpArr.push_back((SourceList[j] + SourceList[j + 1]) / 2.0);
        }

        //RetVal.AddRange(DirectTransform(TmpArr));
        std::vector<double> b = DirectTransform(TmpArr);
        RetVal.insert(RetVal.end(), b.begin(), b.end());

        return RetVal;
    }

    std::vector<double> InverseTransform(std::vector<double> SourceList) {
        if (SourceList.size() == 1)
            return SourceList;

        std::vector<double> RetVal;
        std::vector<double> TmpPart;

        for (int i = (int)SourceList.size() / 2; i < (int)SourceList.size(); i++)
            TmpPart.push_back(SourceList[i]);

        std::vector<double> SecondPart = InverseTransform(TmpPart);

        for (int i = 0; i < (int)SourceList.size() / 2; i++) {
            RetVal.push_back(SecondPart[i] + SourceList[i]);
            RetVal.push_back(SecondPart[i] - SourceList[i]);
        }

        return RetVal;
    }


    // AVal - массив анализируемых данных, Nvl - длина массива должна быть кратна степени 2.
    // FTvl - массив полученных значений, Nft - длина массива должна быть равна Nvl.

    const double TwoPi = 6.283185307179586;

    void FFTAnalysis(std::vector<double>& AVal, std::vector<double>& FTvl) {
      int Nvl = (int)AVal.size();
      int Nft = (int)FTvl.size();
      int i, j, n, m, Mmax, Istp;
      double Tmpr, Tmpi, Wtmp, Theta;
      double Wpr, Wpi, Wr, Wi;


      n = Nvl * 2; //Tmvl = new double[n];
      std::vector<double> Tmvl(n);

      for (i = 0; i < n; i+=2) {
       Tmvl[i] = 0;
       Tmvl[i+1] = AVal[i/2];
      }

      i = 1; j = 1;
      while (i < n) {
        if (j > i) {
          Tmpr = Tmvl[i]; Tmvl[i] = Tmvl[j]; Tmvl[j] = Tmpr;
          Tmpr = Tmvl[i+1]; Tmvl[i+1] = Tmvl[j+1]; Tmvl[j+1] = Tmpr;
        }
        i = i + 2; m = Nvl;
        while ((m >= 2) && (j > m)) {
          j = j - m; m = m >> 1;
        }
        j = j + m;
      }

      Mmax = 2;
      while (n > Mmax) {
        Theta = -TwoPi / Mmax; Wpi = sin(Theta);
        Wtmp = sin(Theta / 2); Wpr = Wtmp * Wtmp * 2;
        Istp = Mmax * 2; Wr = 1; Wi = 0; m = 1;

        while (m < Mmax) {
          i = m; m = m + 2; Tmpr = Wr; Tmpi = Wi;
          Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
          Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;

          while (i < n) {
            j = i + Mmax;
            Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
            Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];

            Tmvl[j] = Tmvl[i] - Tmpr; Tmvl[j-1] = Tmvl[i-1] - Tmpi;
            Tmvl[i] = Tmvl[i] + Tmpr; Tmvl[i-1] = Tmvl[i-1] + Tmpi;
            i = i + Istp;
          }
        }

        Mmax = Istp;
      }

      for (i = 0; i < Nft; i++) {
        j = i * 2; FTvl[i] = 2*sqrt(pow(Tmvl[j],2) + pow(Tmvl[j+1],2))/Nvl;
      }

      //delete []Tmvl;
    }

}
