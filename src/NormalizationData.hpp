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

/** @file
    @brief Заголовочный файл с функциями для нормализации данных
    @version 1.0
    @date 08.01.2018
*/

#ifndef NORMALIZATIONDATA_HPP_INCLUDED
#define NORMALIZATIONDATA_HPP_INCLUDED

#include <vector>
#include <algorithm>

namespace Normalization {
    /** @brief MinMax нормализация данных
        @param[in] input входные данные для нормализации
        @param[out] output нормализованный вектор
        @param[in] normType тип нормализации (0 - нормализация данных к промежутку от 0 до 1, иначе от -1 до 1)
    */
    template<class dataType, class dataType2>
    void calcMinMax(std::vector<dataType>& input, std::vector<dataType2>& output, int normType) {
        dataType maxData = *std::max_element(input.begin(), input.end());
        dataType minData = *std::min_element(input.begin(), input.end());
        double ampl = maxData - minData;
        output.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            output[i] = normType == 0 ? (double)(input[i] - minData) / ampl : 2.0 * ((double)(input[i] - minData) / ampl) - 1.0;
        }
    }

    /** @brief Z-Score нормализация данных
        @param[in] in входные данные для нормализации
        @param[out] out нормализованный вектор
        @param[in] d множитель для стандартного отклонения
    */
    template<class dataType, class dataType2>
    void calcZscore(std::vector<dataType>& in, std::vector<dataType2>& out, double d = 1.0) {
        double mean = 0;
        for(int k = 0; k < in.size(); k++) {
            mean += in[k];
        }
        mean /= (double)in.size();

        double diff = 0;
        for(int k = 0; k < in.size(); k++) {
            diff += ((in[k] - mean) * (in[k] - mean));
        }
        double stdDev = std::sqrt(diff / (double) (in.size() - 1));

        out.resize(in.size());
        double dix =  d * stdDev;
        for(int k = 0; k < in.size(); k++) {
            out[k] = (in[k] - mean) / dix;
            if(out[k] > 1.0) out[k] = 1.0;
            if(out[k] < -1.0) out[k] = -1.0;
        }
    }
}

#endif // NORMALIZATIONDATA_HPP_INCLUDED
