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
    @brief Файл содержит класс для определения эффективности стратегий
    @version 1.0
    @date 08.01.2018
*/

#ifndef STRATEGYEFFECTIVENESS_HPP_INCLUDED
#define STRATEGYEFFECTIVENESS_HPP_INCLUDED

#include <vector>

/** @brief Класс для определения эффективности торговых стратегий
    @version 1.0
    @date 03.02.2018
*/
class StrategyEffectiveness {
    public:
    int hwin[24];
    int hloss[24];
    int mwin[1440];
    int mloss[1440];
    int win = 0; // количество выйграшей
    int loss = 0; // количество проигрышей
    int maxLoss = 0;
    int lossState = 0;
    double money = 100; // деньги
    double profit = 0.9; // доходность со ставки
    double rate = 0.01; // ставка
    double grossprofit = 0;
    double grossloss = 0;
    std::vector<double> vMoney;
    StrategyEffectiveness();
    void setWin();
    void setLoss();
    void setWin(int hour);
    void setLoss(int hour);
    void setWin(int hour, int minutes);
    void setLoss(int hour, int minutes);
    double getConsistency();
    double getProfitStability();
    double getProfitLoss();
    double getAverageGeometricYield();
    double getCoeffSharpe();
    double getCoeffSortino();
    double getBalanceMaxCoeffSharpe();
    double getEff();
    double getAverageGeometricYieldMx();
    int getGoodHour();
    // коэффициент безопасности торговой системы
    double getTSSF();
};

#endif // STRATEGYEFFECTIVENESS_HPP_INCLUDED
