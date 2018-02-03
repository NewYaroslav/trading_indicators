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
    @brief «аголовочный файл с функци€ми дл€ обучени€
    @version 1.0
    @date 08.01.2018
*/

#ifndef TRAINING_HPP_INCLUDED
#define TRAINING_HPP_INCLUDED

#include <vector>
#include <string>

namespace TrainingFunctions {

    /** @brief ѕолучить массивы данных
        @param[in] path вектор путей до файлов с данными
        @param[in] data вектор данных
        @param[in] start процент начала данных (от 0 до 1)
        @param[in] stop процент конца данных (от 0 до 1)
    */
    void getData(std::vector<std::string>& path, std::vector<std::vector<double>>& data, double start, double stop);

    void getExtremaDetectorTrainData(std::string name, std::vector<std::vector<double>>& data, int numTrainingSamples, int nWindow);

    /** @brief ѕолучить массивы минимумов и максимумов
        @param[in] close входные данные
        @param[out] dataMin вектор минимумов
        @param[out] dataMax вектор масимумов
    */
    void getMaxMinData(std::vector<double>& close, std::vector<double>& dataMin, std::vector<double>& dataMax);

    /** @brief ѕолучить данные дл€ обучени€ с одного массива
        @param[in] name им€ файла дл€ сохранени€
        @param[in] numTrainingSamples количество данных дл€ обучени€
        @param[in] nWindow размер окна
        @param[in] data данные дл€ обработки
    */
    void getWindowTrainData(std::string name, int numTrainingSamples, int nWindow, std::vector<double>& data);

    /** @brief ѕолучить данные нормированные внутри полос боллинджера
        @param[in] name им€ файла дл€ сохранени€
        @param[in] numTrainingSamples количество данных дл€ обучени€
        @param[in] nMa период скольз€щей средней дл€ фильтрации данных
        @param[in] nBb период боллинджера
        @param[in] d множитель стандартного отколенни€ полос боллинджера
        @param[in] nRsi период rsi
        @param[in] nWindow размер окна
        @param[in] data данные дл€ обработки
    */
    void getNormBbRsiTrain(std::string name, int numTrainingSamples, int nMa, int nBb, double d, int nRsi, int nWindow, std::vector<double>& data);


    void getGoodTimeEmaRsiTrainData(std::string name, int numTrainingSamples, std::vector<double>& data);

    void getBbTrainData(std::string name, int numTrainingSamples, int nWindow, double d, std::vector<double>& data);

}

#endif // TRAINING_HPP_INCLUDED
