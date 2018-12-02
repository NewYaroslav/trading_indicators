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

#ifndef READINGDATA_HPP_INCLUDED
#define READINGDATA_HPP_INCLUDED

#include <vector>
#include <string>

class CurrencyQuote {
    private:

    //int getIntData(std::string buffer, std::size_t offset, std::size_t& found);
    //double getDoubleData(std::string buffer, std::size_t offset, std::size_t& found);
    void tickMinutes(int& year, int& month, int& day, int& hour, int& minutes);
    int getMinutes(int year, int month, int day, int hour, int minutes);
    int getDifference(int year, int month, int day, int hour, int minutes,
                      int year2, int month2, int day2, int hour2, int minutes2);
    public:
    int maxDifference = 30;
    bool isOpen = false;
    /// Варианты чтения данных
    enum eDataType {
        TICKER_DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL = 0, ///< Данные идут в порядке: <TICKER>,<DTYYYYMMDD>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>
        DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL ///< Данные идут в порядке: <DTYYYYMMDD>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>
    } dataType = TICKER_DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL;
    std::string nameCurrencyPair; ///< Имя валютной пары
    std::vector<short> year; ///< Год свечи
    std::vector<char> month; ///< Месяц свечи
    std::vector<char> day; ///< день свечи
    std::vector<char> hour; ///< час свечи
    std::vector<char> minutes; ///< минута свечи
    std::vector<char> seconds; ///< секунда  свечи

    std::vector<double> open; ///< цена открытия свечи
    std::vector<double> high; ///< наивысшая цена свечи
    std::vector<double> low; ///< наинизшая цена свечи
    std::vector<double> close; ///< цена закрытия свечи
    //std::vector<int> vol;

    CurrencyQuote(std::string name);
    CurrencyQuote(std::string name, eDataType type);
    int openFile(std::string name);
};

#endif // READINGDATA_HPP_INCLUDED
