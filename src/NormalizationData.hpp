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
#include "TradingIndicators.hpp"

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
        if(ampl != 0) {
            for(int i = 0; i < (int)input.size(); i++) {
                output[i] = normType == 0 ? (double)(input[i] - minData) / ampl : 2.0 * ((double)(input[i] - minData) / ampl) - 1.0;
            }
        } else {
            for(int i = 0; i < (int)input.size(); i++) {
                output[i] = 0.0;
            }
        }
    }

    /** @brief Z-Score нормализация данных
        @param[in] in входные данные для нормализации
        @param[out] out нормализованный вектор
        @param[in] d множитель для стандартного отклонения
    */
    template<class dataType, class dataType2>
    void calcZscore(std::vector<dataType>& in, std::vector<dataType2>& out, double d = 1.0) {
        double mean = std::accumulate(in.begin(), in.end(), double(0));
        mean /= (double)in.size();

        double diff = 0;
        for(int k = 0; k < (int)in.size(); k++) {
            diff += ((in[k] - mean) * (in[k] - mean));
        }

        double stdDev = diff > 0 ? std::sqrt(diff / (double) (in.size() - 1)) : 0.0;

        out.resize(in.size());
        double dix =  d * stdDev;
        for(int k = 0; k < (int)in.size(); k++) {
            out[k] = dix != 0 ? (in[k] - mean) / dix : 0.0;
            if(out[k] > 1.0) out[k] = 1.0;
            if(out[k] < -1.0) out[k] = -1.0;
        }
    }

    /** @brief Посчитать массив разности элементов
        @param[in] in входные данные для подсчета разницы
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

    /** @brief Максимизировать амплитуду
        @param[in] input входные данные
        @param[out] output получившееся данные
    */
    template<class dataType, class dataType2>
    void calcMaxAmplitude(std::vector<dataType>& input, std::vector<dataType2>& output) {
        dataType maxData = *std::max_element(input.begin(), input.end());
        dataType minData = *std::min_element(input.begin(), input.end());
        output.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            output[i] = input[i] > 0 ? (input[i] / maxData) : (input[i] < 0 ? (input[i] / -minData) : 0.0);
        }
    }


    /** @brief Максимизировать амплитуду
        @param[in] input входные данные
        @param[out] output получившееся данные
        @param[in] normData вектор по которому нормируем данные
    */
    template<class dataType, class dataType2, class dataType3>
    void calcMaxAmplitude(std::vector<dataType>& input, std::vector<dataType2>& output, std::vector<dataType3>& normData) {
        dataType maxData = *std::max_element(normData.begin(), normData.end());
        dataType minData = *std::min_element(normData.begin(), normData.end());
        output.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            output[i] = input[i] > 0 ? (input[i] / maxData) : (input[i] < 0 ? (input[i] / -minData) : 0.0);
        }
    }

    /** @brief Логарифм от данных
        @param[in] input входные данные
        @param[out] output получившееся данные
    */
    template<class dataType, class dataType2>
    void calcLog(std::vector<dataType>& input, std::vector<dataType2>& output) {
        output.resize(input.size());
        for(int i = 0; i < (int)input.size(); i++) {
            output[i] = std::log(input[i]);
        }
    }

    /** @brief Стандартное отклонение
        @param[in] input входные данные
        @param[out] mean среднее значение данных
        @return стандартное отклонение
    */
    template<class dataType>
    dataType calcStdDev(std::vector<dataType>& in, dataType mean) {
        double diff = 0;
        for(int k = 0; k < (int)in.size(); k++) {
            diff += ((in[k] - mean) * (in[k] - mean));
        }
        dataType stdDev = std::sqrt(diff / (double) (in.size() - 1));
        return stdDev;
    }

    /** @brief Среднее значение
        @param[in] input входные данные
        @return среднее отклонение
    */
    template<class dataType>
    dataType calcMean(std::vector<dataType>& input) {
        double sum = std::accumulate(input.begin(), input.end(), double(0));
        sum /= (double)input.size();
        return sum;
    }

    /** @brief Проверка данных на диапазон от -1 до 1
        @param[in] input входные данные
    */
    template<class dataType>
    void checkingData(std::vector<dataType>& input) {
        for(int k = 0; k < (int)input.size(); k++) {
            if(input[k] > 1) input[k] = 1.0;
            else if(input[k] < -1) input[k] = -1.0;
        }
    }

    /** @brief Получить число десятичных знаков после запятой
        @param[in] input входные данные
        @param[in] isFactor флаг. При установке данного флага функция возвращает множитель
        @return количество знаков после запятой или множитель
    */
    template<class dataType>
    int getNumberDecimals(std::vector<dataType>& input, bool isFactor = true) {
        int factor, num = 1;
        for(int n = 10; n < 1000000; n *= 10, num++) {
            bool isFound = true;
            factor = n;
            for(int i = 0; i < (int)input.size(); i++) {
                int temp = input[i] * n;
                dataType dTemp = temp / (dataType) n;
                if(dTemp != input[i]) {
                    isFound = false;
                    break;
                }

            }
            if(isFound == true) {
                break;
            }
        }
        if(isFactor == true) return factor;
        else return num;
    }

    /** @brief Класс для нормализации сигнала в пределах полос Боллинджера
        @version 1.0
        @date 30.01.2018
    */
    class BollingerBandsNorm {
        public:
        bool isInit = false;
        Indicators::BollingerBands iBands;
        BollingerBandsNorm();
        BollingerBandsNorm(int n, double d);
        double updata(double input);
    };

}

#endif // NORMALIZATIONDATA_HPP_INCLUDED
