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
    @brief Заголовочный файл с классами различных индикаторов техничекого анализа
    @version 1.0
    @date 08.01.2018
*/

#ifndef TRADINGINDICATORS_HPP_INCLUDED
#define TRADINGINDICATORS_HPP_INCLUDED

#include <vector>
#include <numeric>
#include <algorithm>

namespace Indicators {

    /** @brief Класс простой скользящей средней
        @version 1.0
        @date 08.01.2018
        @code
        Indicators::SMA iSMA(5); // создать индикатор с периодом 5
        // подаем на вход индикатора значение 100,
        // на выходе получаем среднее
        // значение за последние 5 отсчетов.
        double output = iSMA.update(100);
        double output = iSMA.update(100);
        double output = iSMA.update(100);
        @endcode
    */
    class SMA {
        private:
        int n;
        std::vector<double> data;
        public:
        /** @brief Инициализация индикатора со стандартными настройками (период равен 10)
        */
        SMA();
        /** @brief Инициализация индикатора с заданным периодом
            @param[in] period период индикатора
        */
        SMA(int period);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
            @return значение индикатора
        */
        double update(double input);
    };

    /** @brief Класс экспоненциальной скользящей средней
        @version 1.0
        @date 08.01.2018
    */
    class EMA {
        private:
        int n;
        double a;
        double prevOut;
        std::vector<double> data;
        public:
        /** @brief Инициализация индикатора со стандартными настройками (период равен 10)
        */
        EMA();
        /** @brief Инициализация индикатора с заданным периодом
            @param[in] period период индикатора
        */
        EMA(int period);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
            @return значение индикатора
        */
        double update(double input);
    };

    /** @brief Класс взвешенной скользящей средней
        @version 1.0
        @date 08.01.2018
    */
    class WMA {
        private:
        int n;
        double a;
        double prevOut;
        std::vector<double> data;
        public:
        /** @brief Инициализация индикатора со стандартными настройками (период равен 10)
        */
        WMA();
        /** @brief Инициализация индикатора с заданным периодом
            @param[in] period период индикатора
        */
        WMA(int period);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
            @return значение индикатора
        */
        double update(double input);
    };

    /** @brief Простая скользящая медиана
        @version 1.0
        @date 08.01.2018
    */
    class SMM {
        private:
        int n;
        std::vector<double> data;
        public:
        /** @brief Инициализация простой скользящей медианы со стандартными настройками (период равен 10)
        */
        SMM();
        /** @brief Инициализация простой скользящей медианы с заданным периодом
            @param[in] period период индикатора
        */
        SMM(int period);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
            @return значение индикатора
        */
        double update(double input);
    };

    /** @brief Класс индекса относительной силы
        @version 1.0
        @date 08.01.2018
    */
    class RSI {
        private:
        SMA iU;
        SMA iD;
        int n;
        double prevInput;
        public:
        bool isStart = false;
        /** @brief Инициализация индикатора со стандартными настройками (период равен 5)
        */
        RSI();
        /** @brief Инициализация индикатора с заданными периодом
            @param[in] period период индикатора
        */
        RSI(int period);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
            @return значение индикатора
        */
        double update(double input);
    };

    /** @brief Класс индекса относительной силы с использованием WMA вместо SMA
        @version 1.0
        @date 08.01.2018
    */
    class WRSI {
        private:
        WMA iU;
        WMA iD;
        int n;
        double prevInput;
        public:
        bool isStart = false;
        /** @brief Инициализация индикатора со стандартными настройками (период равен 5)
        */
        WRSI();
        /** @brief Инициализация индикатора с заданными периодом
            @param[in] period период индикатора
        */
        WRSI(int period);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
            @return значение индикатора
        */
        double update(double input);
    };

    /** @brief Линии Боллинджера
        @version 1.0
        @date 09.01.2018
    */
    class BollingerBands {
        private:
        int n;
        double d;
        std::vector<double> data;
        public:
        /** @brief Инициализация индикатора со стандартными настройками (период равен 20, множитель 2)
        */
        BollingerBands();
        /** @brief Инициализация индикатора с заданными настройками
            @param[in] period период индикатора
            @param[in] d множитель стандартного отклонения
        */
        BollingerBands(int period, double d);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
        */
        void update(double input);
        double tl = 0; ///< верхняя полоса боллинджера
        double bl = 0; ///< нижняя полоса боллинджера
        double ml = 0; ///< простая скользящая средняя
    };

    /** @brief Расчитать стандартное отклонение
        @param[in] input входные данные индикатора
        @return стандартное отклонение
    */
    template<class dataType>
    double calcStandardDeviation(std::vector<dataType>& input) {
        double mean = std::accumulate(input.begin(), input.end(), 0);
        mean /= (double)input.size();
        double _stdDev = 0;
        for (int i = 0; i < (int)input.size(); i++) {
            double diff = (input[i] - mean);
            _stdDev +=  diff * diff;
        }
        return std::sqrt(_stdDev / (double)(input.size() - 1));
    }

    /** @brief Скользящая средняя NoLagMa
        @version 1.0
        @date 09.01.2018
    */
    class NoLagMa {
        private:
        std::vector<double> nlm_values;
        std::vector<double> nlm_prices;
        std::vector<double> nlm_alphas;

        double Pi = 3.14159265358979323846264338327950288;
        int _length = 0;
        int _len = 1;
        int _weight = 2;

        int LengthMA = 10;
        int bars = 0;

        double _update(double price, int length, int r);
        public:
        /** @brief Инициализация индикатора со стандартными настройками (период равен 3)
        */
        NoLagMa();
        /** @brief Инициализация индикатора с заданными настройками
            @param[in] period период индикатора
            @warning период должен быть больше или равен 3
        */
        NoLagMa(int n);
        /** @brief Обновить входные данные индикатора
            @param[in] in входные данные индикатора
            @return значение индикатора
        */
        double update(double in);
    };

    /** @brief Скользящая средняя FATL
        @version 1.0
        @date 09.01.2018
    */
    class FATL {
        private:
        int FATLPeriod = 39;
        double price[39];
        int tick = 0;
        public:
        int trendState = 0; ///< направление тренда
        double prevOut = 0; ///< предыдущее значение индикатора
        double out = 0; ///< значение индикатора
        /** @brief Инициализация индикатора
        */
        FATL();
        /** @brief Обновить входные данные индикатора
            @param[in] in входные данные индикатора
            @return значение индикатора
        */
        double update(double in);
    };

    /** @brief Скользящая средняя SATL
        @version 1.0
        @date 09.01.2018
    */
    class SATL {
        private:
        int SATLPeriod = 65;
        double price[65];
        int tick = 0;
        public:
        int trendState = 0; ///< направление тренда
        double prevOut = 0; ///< предыдущее значение индикатора
        double out = 0; ///< значение индикатора
        /** @brief Инициализация индикатора
        */
        SATL();
        /** @brief Обновить входные данные индикатора
            @param[in] in входные данные индикатора
            @return значение индикатора
        */
        double update(double in);
    };

    /** @brief Индикатор RBCI
        @version 1.0
        @date 09.01.2018
    */
    class RBCI {
        private:
        BollingerBands iBB;
        int RBCIPeriod=56;
        int BBPeriod=100;          //Период для уровней Боллинджера
        double BandsDeviation = 2; //Девиация
        double price[56];
        int tick = 0;
        public:
        int trendState = 0;
        double prevOut = 0;
        double out = 0;
        RBCI();
        RBCI(int BBPeriod, double BandsDeviation);
        double update(double in);
        double TL2= 0;
        double TL1 = 0;
        double ML = 0;
        double BL1= 0;
        double BL2 = 0;
    };

    /** @brief Поиск экстрерумов. Класс составляет массив последних экстремумов
        @version 1.0
        @date 09.01.2018
    */
    class SearchExtrema {
        private:
        int n;
        double a, b;
        double a2, b2;
        bool isStart = false;
        public:
        /** @brief Инициализация индикатора
        */
        SearchExtrema();
        /** @brief Инициализация индикатора с заданными настройками
            @param[in] nLastExtrema количество последних экстрерумов в массиве
            @warning количество последних экстрерумов в массиве должно быть больше 0
        */
        SearchExtrema(int nLastExtrema);
        /** @brief Обновить входные данные индикатора
            @param[in] input входные данные индикатора
        */
        void update(double input);
        /** @brief Обновить входные данные индикатора
            @param[in] high максимум текущего бара
            @param[in] low минимум текущего бара
        */
        void update(double high, double low);
        std::vector<double> lastExtremums; ///< массив экстрерумов
    };

}

#endif // TRADINGINDICATORS_HPP_INCLUDED
