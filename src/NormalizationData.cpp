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
}
