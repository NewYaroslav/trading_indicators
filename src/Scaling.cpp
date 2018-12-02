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

#include "Scaling.hpp"

#include <string>
#include <algorithm>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>

namespace ScalingData {

    void calcArrayScaling(std::vector<double>& in, std::vector<double>& out, int outputSize) {
        int period = in.size() / outputSize;
        if(period < 1) {out.clear(); return;};

        out.resize(outputSize);

        for(int i = 0; i < outputSize; i++) {
            std::vector<double> part;
            std::copy(in.end() - period - period * i, in.end() - period * i, std::back_inserter(part));
            double maxData = *std::max_element(part.begin(), part.end());
            double minData = *std::min_element(part.begin(), part.end());
            double mean = std::accumulate(part.begin(), part.end(), double(0));
            mean /= (double)part.size();
            double up = std::abs(maxData - mean);
            double down = std::abs(minData - mean);
            #if(0)
            if(up > down) {
                out[outputSize - i - 1] = maxData;
            } else
            if(up < down) {
                out[outputSize - i - 1] = minData;
            } else {
                out[outputSize - i - 1] = mean;
            }
            #endif
            out[outputSize - i - 1] = mean;
        }
    }
}
