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

#include "ReadingData.hpp"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include <vector>
#include <string>
#include "stdio.h"

#include <fstream>
#include <iostream>

CurrencyQuote::CurrencyQuote(std::string name) {
    openFile(name);
    year.reserve(60);
    month.reserve(60);
    day.reserve(60);
    hour.reserve(60);
    minutes.reserve(60);
    seconds.reserve(60);
    open.reserve(60);
    high.reserve(60);
    low.reserve(60);
    close.reserve(60);
}

CurrencyQuote::CurrencyQuote(std::string name, eDataType type) {
    dataType = type;
    openFile(name);
    year.reserve(60);
    month.reserve(60);
    day.reserve(60);
    hour.reserve(60);
    minutes.reserve(60);
    seconds.reserve(60);
    open.reserve(60);
    high.reserve(60);
    low.reserve(60);
    close.reserve(60);
}

void CurrencyQuote::tickMinutes(int& year, int& month, int& day, int& hour, int& minutes) {
    const int daysMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    minutes++;
    if(minutes >= 60) {
        minutes = 0;
        hour++;
        if(hour >= 24) {
            hour = 0;
            day++;
            if((month != 2 && day > daysMonth[month - 1]) ||
                (year % 4 == 0 && month == 2 && day > 29) ||
                (year % 4 != 0 && day > daysMonth[month - 1])){
                day = 1;
                month++;
                if(month > 12) {
                    month = 1;
                    year++;
                }
            }
        }
    }
}

int CurrencyQuote::getMinutes(int year, int month, int day, int hour, int minutes) {
    //int daysMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const int daysMonth[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

    int allMinutes = 0;
    int nSextus = year/4;
    allMinutes += (nSextus * 366) + ((year - nSextus) * 365);
    allMinutes += month > 1 ? (year % 4 == 0 ? (month > 2 ? daysMonth[month - 1] + day + 1 : daysMonth[month - 1] + day): daysMonth[month - 1] + day) : day;
    allMinutes -= 1; allMinutes *= 1440; allMinutes += hour * 60 + minutes;
    return allMinutes;
}

int CurrencyQuote::getDifference(int year, int month, int day, int hour, int minutes,
                  int year2, int month2, int day2, int hour2, int minutes2) {
    const int daysMonth[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    if(year != year2) {
        int allMinutes = getMinutes(year, month, day, hour, minutes);
        int allMinutes2 = getMinutes(year2, month2, day2, hour2, minutes2);
        return std::abs(allMinutes2 - allMinutes);
    } else
    if(month != month2) {
        int allMinutes = month > 1 ? (year % 4 == 0 ? (month > 2 ? daysMonth[month - 1] + day + 1 : daysMonth[month - 1] + day): daysMonth[month - 1] + day) : day;
        allMinutes -= 1; allMinutes *= 1440; allMinutes += hour * 60 + minutes;

        int allMinutes2 = month2 > 1 ? (year2 % 4 == 0 ? (month2 > 2 ? daysMonth[month2 - 1] + day2 + 1 : daysMonth[month2 - 1] + day2): daysMonth[month2 - 1] + day2) : day2;
        allMinutes2 -= 1; allMinutes2 *= 1440; allMinutes2 += hour2 * 60 + minutes2;
        return std::abs(allMinutes2 - allMinutes);
    } else
    if(day != day2) {
        int allMinutes = day - 1;
        allMinutes *= 1440; allMinutes += hour * 60 + minutes;
        int allMinutes2 = day2 - 1;
        allMinutes2 *= 1440; allMinutes2 += hour2 * 60 + minutes2;
        return std::abs(allMinutes2 - allMinutes);
    } else
    if(hour != hour2) {
        int allMinutes = hour * 60 + minutes;
        int allMinutes2 = hour2 * 60 + minutes2;
        return std::abs(allMinutes2 - allMinutes);
    } else {
        //std::cout << "m: " << minutes2 << ":" << minutes << std::endl;
        return std::abs(minutes2 - minutes);
    }
    return 0;
}

int CurrencyQuote::getIntData(std::string buffer, std::size_t offset, std::size_t& found) {
    found = buffer.find_first_of(";,", offset);
    std::string numberStr = buffer.substr(offset, found - offset);
    found += 1;
    return atoi(numberStr.c_str());
}

double CurrencyQuote::getDoubleData(std::string buffer, std::size_t offset, std::size_t& found) {
    found = buffer.find_first_of(";,", offset);
    std::string numberStr = buffer.substr(offset, found - offset);
    found += 1;
    return atof(numberStr.c_str());
}

int CurrencyQuote::openFile(std::string name) {
    std::ifstream fin(name);
    if (!fin.is_open()) {
        isOpen = false;
        std::cout << "Error! Can not open file!" << std::endl;
        return 0;
    }
    std::cout << "Open the file: " << name << std::endl;

    std::string buffer;
    std::string numberStr;
    std::size_t found;
    std::size_t offset;

    // получаем заголовок файла
    std::getline(fin, buffer);
    nameCurrencyPair = " ";

    switch(dataType) {
        case TICKER_DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL:
        while(!fin.eof()) {
            std::getline(fin, buffer);
            // читаем имя источника
            found = buffer.find_first_of(";,");
            if (found == std::string::npos) {
                break;
            }
            if(nameCurrencyPair == " ") {
                nameCurrencyPair = buffer.substr(0, found);
                std::cout << "name Currency Pair: " << nameCurrencyPair << std::endl;
            }
            // находим дату и время
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            std::string strData = buffer.substr(offset, found - offset);
            numberStr = strData.substr(0, 4);
            short _year = atoi(numberStr.c_str());
            numberStr = strData.substr(4, 2);
            char _month = atoi(numberStr.c_str());
            numberStr = strData.substr(6, 2);
            char _day = atoi(numberStr.c_str());

            //std::cout << (int)_year << "." << (int)_month << "." << (int)_day << std::endl;

            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            std::string strData2 = buffer.substr(offset, found - offset);
            //std::cout << "strData2: " << strData2 << std::endl;

            numberStr = strData2.substr(0, 2);
            char _hour = atoi(numberStr.c_str());
            numberStr = strData2.substr(2, 2);
            char _minutes = atoi(numberStr.c_str());
            numberStr = strData2.substr(4, 2);
            char _seconds = atoi(numberStr.c_str());

            //std::cout << (int)_hour << ":" << (int)_minutes << ":" << (int)_seconds << std::endl;

            if(close.size() > 0) {
                int diffTime = getDifference(year.back(), month.back(), day.back(), hour.back(), minutes.back(),
                                             _year, _month, _day, _hour, _minutes);
                //if(diffTime > 1) std::cout << std::endl << "diffTime: " << diffTime << std::endl;
                if(diffTime > 1 && diffTime < maxDifference) {
                    for(int i = 0; i < diffTime - 1; i++) {
                        int nbYear = year.back(), nbMonth = month.back(), nbDay = day.back(),
                            nbHour = hour.back(), nbMinutes = minutes.back();

                        tickMinutes(nbYear, nbMonth, nbDay, nbHour, nbMinutes);

                        year.push_back((short)nbYear);
                        month.push_back((char)nbMonth);
                        day.push_back((char)nbDay);
                        hour.push_back((char)nbHour);
                        minutes.push_back((char)nbMinutes);
                        seconds.push_back(0);

                        //std::cout << "add a date: " << (int)nbYear << "." << (int)nbMonth << "." << (int)nbDay << std::endl;
                        //std::cout << "add a time: " << (int)nbHour << ":" << (int)nbMinutes << ":" << 0 << std::endl;

                        open.push_back(close.back());
                        high.push_back(close.back());
                        low.push_back(close.back());
                        close.push_back(close.back());
                    }
                }
            }
            if(close.size() % 14400 == 0) {
                std::cout << "number of bars: " << close.size() << "\r";
            }

            // open
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _open = atof(numberStr.c_str());

            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _high = atof(numberStr.c_str());
            // low
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _low = atof(numberStr.c_str());
            // close
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _close = atof(numberStr.c_str());


            year.push_back(_year);
            month.push_back(_month);
            day.push_back(_day);
            hour.push_back(_hour);
            minutes.push_back(_minutes);
            seconds.push_back(_seconds);

            if(_high < _low) {
                std::cout << "error! high < low" << std::endl;
                std::cout << "high: " << _high << " low: " << _low <<  std::endl;
                std::cout << (int)_year << "." << (int)_month << "." << (int)_day << std::endl;
                std::cout << (int)_hour << ":" << (int)_minutes << ":" << (int)_seconds << std::endl;
            }

            open.push_back(_open);
            high.push_back(_high);
            low.push_back(_low);
            close.push_back(_close);
            //if(close.size() > 100) break;
        }
        break;
        case DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL:
        while(!fin.eof()) {
            std::getline(fin, buffer);

            // находим дату и время
            offset = 0;
            found = buffer.find_first_of(";,", offset);
            if (found == std::string::npos) {
                break;
            }
            std::string strData = buffer.substr(offset, found - offset);
            numberStr = strData.substr(0, 4);
            short _year = atoi(numberStr.c_str());
            numberStr = strData.substr(4, 2);
            char _month = atoi(numberStr.c_str());
            numberStr = strData.substr(6, 2);
            char _day = atoi(numberStr.c_str());

            //std::cout << (int)_year << "." << (int)_month << "." << (int)_day << std::endl;

            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            std::string strData2 = buffer.substr(offset, found - offset);
            //std::cout << "strData2: " << strData2 << std::endl;

            numberStr = strData2.substr(0, 2);
            char _hour = atoi(numberStr.c_str());
            numberStr = strData2.substr(3, 2);
            char _minutes = atoi(numberStr.c_str());
            //numberStr = strData2.substr(4, 2);
            char _seconds = 0; //atoi(numberStr.c_str());

            //std::cout << (int)_hour << ":" << (int)_minutes << ":" << (int)_seconds << std::endl;
            // open
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _open = atof(numberStr.c_str());

            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _high = atof(numberStr.c_str());
            // low
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _low = atof(numberStr.c_str());
            // close
            offset = found + 1;
            found = buffer.find_first_of(";,", offset);
            numberStr = buffer.substr(offset, found - offset);
            double _close = atof(numberStr.c_str());


            year.push_back(_year);
            month.push_back(_month);
            day.push_back(_day);
            hour.push_back(_hour);
            minutes.push_back(_minutes);
            seconds.push_back(_seconds);

            if(_high < _low) {
                std::cout << "error! high < low" << std::endl;
                std::cout << "high: " << _high << " low: " << _low <<  std::endl;
                std::cout << (int)_year << "." << (int)_month << "." << (int)_day << std::endl;
                std::cout << (int)_hour << ":" << (int)_minutes << ":" << (int)_seconds << std::endl;
            }

            open.push_back(_open);
            high.push_back(_high);
            low.push_back(_low);
            close.push_back(_close);
            //if(close.size() > 100) break;
        }
        break;
    }
    isOpen = true;
    fin.close(); // закрываем файл
    std::cout << "number of bars: " << close.size() << std::endl;
    std::cout << "Read OK" << std::endl;
    return 1;
}
