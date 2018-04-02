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
    @brief Заголовочный файл с классами для построения графиков и сбора информации для графиков
    @version 1.0
    @date 11.01.2018
*/

#ifndef DRAWING_HPP_INCLUDED
#define DRAWING_HPP_INCLUDED

#include <vector>
#include <string>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

namespace Drawing {

    /** @brief Класс для хранения данных японских свечей
        @version 1.0
        @date 11.01.2018
    */
    class CandlesType {
        public:
        CandlesType();
        /** @brief Установить данные японской свечи
            @param[in] _open данные отрытия свечи
            @param[in] _close данные закрытия свечи
            @param[in] _high данные максимума свечи
            @param[in] _low данные минимума свечи
        */
        CandlesType(double _open, double _close, double _high, double _low);
        /** @brief Установить данные индикатора
            @warning Данные индикатора должны рисоваться поверх графика с японскими свечами!
            @param[in] data данные индикатора
        */
        void setIndicator(double data);
        void setArrowUp();
        void setArrowDown();
        void setArrow(char arrow);
        void setCircle(int state);

        // функции для обновления данных
        void updataIndicator(double data, int pos);
        void updataArrow(char arrow);

        double open;
        double close;
        double high;
        double low;
        char isArrow = 0;
        char isCirc = 0;
        std::vector<double> indicator;
    };

    /** @brief Класс для сбора данных японских свечей
        @version 1.0
        @date 11.01.2018
    */
    class Window {
        private:
        std::vector<CandlesType> data;
        int N = 0;
        public:
        Window();
        Window(int n);
        void updata(double open, double high, double low, double close);
        void updataLast(double open, double high, double low, double close);
        void setArrow(char arrow);
        void setCircle(int state, int pos);
        void setIndicator(double dataInd);
        void updateIndicator(double dataInd, int pos);
        void getCandlesType(std::vector<CandlesType>& out);
        std::vector<CandlesType> getCandlesType();
    };

    /** @brief Получить график свечей
        @param[in] out изображение
        @param[in] width ширина изображения
        @param[in] height высота изображения
        @param[in] in массив с данными свечей
        @param[in] backgroundColor цвет фона
        @param[in] backgroundColor2 цвет фона
        @param[in] lineColor цвет линий индикаторов
    */
    void getCandleGraph(cv::Mat& out,
                        int width, int height,
                        std::vector<CandlesType>& in,
                        cv::Scalar backgroundColor, cv::Scalar backgroundColor2, std::vector<cv::Scalar>& lineColor);

    /** @brief Показать график свечей
        @param[in] name имя окна изображения
        @param[in] in массив с данными свечей
        @param[in] flag флаг включения задержки кадров (для пропуска кадра надо нажать английскую P)
    */
    void viewCandleGraph(std::string name, std::vector<CandlesType>& in, int flag);

    /** @brief Сохранить график свечей
        @param[in] name имя окна изображения
        @param[in] in массив с данными свечей
    */
    void saveCandleGraph(std::string name, std::vector<CandlesType>& in);

    /** @brief Показать график
        @param[in] name имя окна изображения
        @param[in] text текс на изображении
        @param[in] in1 массив с данными
        @param[in] in2 массив с данными
        @param[in] in3 массив с данными
        @param[in] in4 массив с данными
        @param[in] width ширина изображения
        @param[in] height высота изображения
        @param[in] mask флаг, который если равен 0x10 включет сохранение картинки в папке img с именем name.jpg
    */
    char drawOscilloscope4xBeam(
    std::string name, std::string text,
    std::vector<double> in1, std::vector<double> in2, std::vector<double> in3, std::vector<double> in4,
    int width, int height, int mask);


    void drawCandleGraph(cv::Mat& output, int x, int y, int width, int height, std::vector<CandlesType>& in, cv::Scalar backgroundColor, std::vector<cv::Scalar>& lineColor, int isAutoCircl);
    void viewCandleGraph2(std::string name, Window& iWind, int width, int height);
}

#endif // DRAWING_HPP_INCLUDED
