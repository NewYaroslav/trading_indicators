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
    @brief «аголовочный файл с функци€ми дл€ нормализации данных
    @version 1.0
    @date 08.01.2018
*/

#ifndef NORMALIZATIONDATA_HPP_INCLUDED
#define NORMALIZATIONDATA_HPP_INCLUDED

#include <vector>
#include <algorithm>

namespace Normalization {
    /** @brief MinMax нормализаци€ данных
        @param[in] input входные данные дл€ нормализации
        @param[out] output нормализованный вектор
        @param[in] normType тип нормализации (0 - нормализаци€ данных к промежутку от 0 до 1, иначе от -1 до 1)
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

    /** @brief Z-Score нормализаци€ данных
        @param[in] in входные данные дл€ нормализации
        @param[out] out нормализованный вектор
        @param[in] d множитель дл€ стандартного отклонени€
    */
    template<class dataType, class dataType2>
    void calcZscore(std::vector<dataType>& in, std::vector<dataType2>& out, double d = 1.0) {
        double mean = 0;
        for(int k = 0; k < (int)in.size(); k++) {
            mean += in[k];
        }
        mean /= (double)in.size();

        double diff = 0;
        for(int k = 0; k < (int)in.size(); k++) {
            diff += ((in[k] - mean) * (in[k] - mean));
        }
        double stdDev = std::sqrt(diff / (double) (in.size() - 1));

        out.resize(in.size());
        double dix =  d * stdDev;
        for(int k = 0; k < (int)in.size(); k++) {
            out[k] = (in[k] - mean) / dix;
            if(out[k] > 1.0) out[k] = 1.0;
            if(out[k] < -1.0) out[k] = -1.0;
        }
    }

    /** @brief ѕосчитать массив разности элементов
        @param[in] in входные данные дл€ подсчета разницы
        @param[out] out массив с разностью элементов
    */
    template<class dataType, class dataType2>
    void calcDifference(std::vector<dataType>& in, std::vector<dataType2>& out) {
        if(in.size() < 2) {out.clear(); return;};
        out.resize(in.size() - 1);
        for(int i = 1; i < (int)in.size(); i++) {
            out[i - 1] = in[i] - in[i - 1];
        }
    }

    /** @brief ѕосчитать массив разности элементов
        @param[in] in входные данные дл€ подсчета разницы
        @param[out] out массив с разностью элементов
    */
    template<class dataType, class dataType2>
    void calcMaxAmplitude(std::vector<dataType>& input, std::vector<dataType2>& output) {
        dataType maxData = *std::max_element(input.begin(), input.end());
        dataType minData = *std::min_element(input.begin(), input.end());
        double ampl = maxData - minData;
        output.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            output[i] = input[i] > 0 ? (input[i] / maxData) : (input[i] / -minData);
        }
    }

    template<class dataType, class dataType2>
    void calcLog(std::vector<dataType>& input, std::vector<dataType2>& output) {
        output.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            output[i] = std::log(input[i]);
        }
    }



}

#endif // NORMALIZATIONDATA_HPP_INCLUDED
