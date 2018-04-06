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

#include "StrategyEffectiveness.hpp"
#include <cmath>


StrategyEffectiveness::StrategyEffectiveness() {
    grossprofit = 0;
    grossloss = 0;
    money = 100; win = 0; loss = 0;
    vMoney.clear();
    vMoney.reserve(1000);
    vMoney.push_back(money);
    for(int i = 0; i < 24; i++) {
        hwin[i] = 0;
        hloss[i] = 0;
    }
    for(int i = 0; i < 1440; i++) {
        mwin[i] = 0;
        mloss[i] = 0;
    }
};

void StrategyEffectiveness::updata() {
    for(size_t i = 0; i < vRate.size(); i++) {
        vTick[i]--;
        if(vTick[i] <= 0) {
            if(vState[i] == 1) {
                win++;
                if(lossState > maxLoss) {
                    maxLoss = lossState; lossState = 0;
                }
                money += vRate[i] + profit * vRate[i];
                grossprofit += profit * vRate[i];
                vMoney.push_back(money);
            } else {
                loss++;
                lossState++;
                grossloss += vRate[i];
                vMoney.push_back(money);
            }
            vRate.erase(vRate.begin() + i);
            vTick.erase(vTick.begin() + i);
            vState.erase(vState.begin() + i);
        }
    }
}

void StrategyEffectiveness::setDelayWin() {
    vRate.push_back(rate * money);
    vState.push_back(1);
    vTick.push_back(expTime);
    money -= rate * money;
}

void StrategyEffectiveness::setDelayLoss() {
    vRate.push_back(rate * money);
    vState.push_back(0);
    vTick.push_back(expTime);
    money -= rate * money;
}

void StrategyEffectiveness::setWin() {
    win++;
    if(lossState > maxLoss) {
        maxLoss = lossState; lossState = 0;
    }
    money += profit*rate*money;
    grossprofit += profit*rate*money;
    vMoney.push_back(money);
}


void StrategyEffectiveness::setLoss() {
    loss++;
    lossState++;
    money -= rate*money;
    grossloss += rate*money;
    vMoney.push_back(money);
}

void StrategyEffectiveness::setWin(int hour) {
    hwin[hour]++;
    win++;
    if(lossState > maxLoss) {
        maxLoss = lossState; lossState = 0;
    }
    money += profit*rate*money;
    grossprofit += profit*rate*money;
    vMoney.push_back(money);
}

void StrategyEffectiveness::setLoss(int hour) {
    hloss[hour]++;
    loss++;
    lossState++;
    money -= rate*money;
    grossloss += rate*money;
    vMoney.push_back(money);
}

void StrategyEffectiveness::setWin(int hour, int minutes) {
    mwin[hour*60 + minutes]++;
    hwin[hour]++;
    win++;
    if(lossState > maxLoss) {
        maxLoss = lossState; lossState = 0;
    }
    money += profit*rate*money;
    grossprofit += profit*rate*money;
    vMoney.push_back(money);
}

void StrategyEffectiveness::setLoss(int hour, int minutes) {
    mloss[hour*60 + minutes]++;
    hloss[hour]++;
    loss++;
    lossState++;
    money -= rate*money;
    grossloss += rate*money;
    vMoney.push_back(money);
}


double StrategyEffectiveness::getConsistency() {
    if(vMoney.size() == 0) return 1000;
    double sum = 0;
    //double delta = ((double)(vMoney.back() - vMoney.front()) / (double)(vMoney.size()));
    double delta = ((double)(log(vMoney.back()) - log(vMoney.front())) / (double)(vMoney.size()));
    double y0 = log(vMoney.front());
    for(int i = 1; i < (int)vMoney.size() - 1; i++) {
        double y = y0 + delta * (double)i;
        double diff = log(vMoney[i]) - y;
        sum += diff * diff;
    }
    sum /= (double)(vMoney.size() - 2);
    return sum;
}

double StrategyEffectiveness::getProfitStability() {
    if(vMoney.size() == 0) return 0;
    double sum = 0;
    //double delta = ((double)(vMoney.back() - vMoney.front()) / (double)(vMoney.size()));
    double delta = ((double)(log(vMoney.back()) - log(vMoney.front())) / (double)(vMoney.size()));
    double& trendProfit = delta;
    double y0 = log(vMoney.front());
    double totalVolume = 0;
    for(int i = 0; i < (int)vMoney.size(); i++) {
        double logMoney = log(vMoney[i]);
        double y = y0 + delta * (double)i;
        double diff = logMoney - y;
        sum += diff * diff;
        totalVolume += logMoney;
    }
    sum /= (double)(vMoney.size() - 2);
    double lrStandartError = sqrt(sum);
    double profitStability = trendProfit/lrStandartError;
    //profitStability /= totalVolume;
    //profitStability *= vMoney.size();
    profitStability *= 10000;
    return profitStability;
}


double StrategyEffectiveness::getProfitLoss() {
    if(grossloss == 0) return 0;
    return grossprofit/grossloss;
}


double StrategyEffectiveness::getAverageGeometricYield() {
    if(vMoney.size() < 2) return 0;
    double mx = 1.0;
    for(int i = 1; i < (int)vMoney.size(); i++) {
        double ri = vMoney[i - 1] > 0 ? 1.0 + ((double)(vMoney[i] - vMoney[i - 1]) / (double)vMoney[i - 1]) : 0;
        //double ri = vMoney[i - 1] > 0 ? ((double)vMoney[i] / (double)vMoney[i - 1]) : 0;
        mx *= ri;
    }
    if(mx == 0) return 0;
    return pow(mx,(1.0/(double)vMoney.size()));
}

double StrategyEffectiveness::getAverageGeometricYieldMx() {
    if(vMoney.size() < 2) return 0;
    double mx = 1.0;
    for(int i = 1; i < (int)vMoney.size(); i++) {
        double ri = vMoney[i - 1] > 0 ? 1.0 + ((double)(vMoney[i] - vMoney[i - 1]) / (double)vMoney[i - 1]) : 0;
        //double ri = vMoney[i - 1] > 0 ? ((double)vMoney[i] / (double)vMoney[i - 1]) : 0;
        mx *= ri;
    }
    return mx;
    //return pow(mx,(1.0/(double)vMoney.size()));
}


double StrategyEffectiveness::getCoeffSharpe() {
    if(vMoney.size() < 3) return 0;
    double re = getAverageGeometricYield();
    if(re == 0) return 0;
    double sum = 0;
    for(int i = 1; i < (int)vMoney.size(); i++) {
        double ri = vMoney[i - 1] > 0 ? 1.0 + ((double)(vMoney[i] - vMoney[i - 1]) / (double)vMoney[i - 1]) : 0;
        //double ri = vMoney[i - 1] > 0 ? ((double)vMoney[i] / (double)vMoney[i - 1]) : 0;
        double diff = ri - re;
        sum += diff * diff;
    }
    if(sum == 0) return 0;
    double sigma = (1.0 / (double)(vMoney.size() - 1)) * sqrt(sum);
    //return sqrt((double)vMoney.size())*(re/sigma);
    return (re/sigma);
}

double StrategyEffectiveness::getCoeffSortino() {
    if(vMoney.size() < 3) return 0;
    double re = getAverageGeometricYield();
    if(re == 0) return 0;
    double sum = 0;
    for(int i = 1; i < (int)vMoney.size(); i++) {
        double ri = vMoney[i - 1] > 0 ? 1.0 + ((double)(vMoney[i] - vMoney[i - 1]) / (double)vMoney[i - 1]) : 0;
        //double ri = vMoney[i - 1] > 0 ? ((double)vMoney[i] / (double)vMoney[i - 1]) : 0;
        double diff = ri - re;
        sum += diff * diff;
    }
    if(sum == 0) return 0;
    double sigma = (1.0 / (double)(vMoney.size() - 1)) * sqrt(sum);
    //return sqrt((double)vMoney.size())*(re/sigma);
    return (re/sigma);
}


double StrategyEffectiveness::getBalanceMaxCoeffSharpe() {
    if(vMoney.size() < 2) return 0;
    if(win == 0) return 0;
    double re = getAverageGeometricYield();
    if(re == 0) return 0;
    double sum = 0;
    for(int i = 1; i < (int)vMoney.size(); i++) {
        double ri = vMoney[i - 1] > 0 ? 1.0 + ((double)(vMoney[i] - vMoney[i - 1]) / (double)vMoney[i - 1]) : 0;
        //double ri = vMoney[i - 1] > 0 ? ((double)vMoney[i] / (double)vMoney[i - 1]) : 0;
        double diff = ri - re;
        sum += diff * diff;
    }
    if(sum == 0) return 0;
    double sigma = (1.0 / (double)(vMoney.size() - 1)) * sqrt(sum);
    double coeffSharpe = re/sigma;
    double out = coeffSharpe * vMoney.back();
    return out;
}


double StrategyEffectiveness::getEff() {
    return win + loss > 0 ? (double)win /(double)(win + loss) : 0;
}

int StrategyEffectiveness::getGoodHour() {
    double maxEff = 0; int pos = 0;
    for(int i = 0; i < 24; i++) {
        int sum = hloss[i] + hwin[i];
        double eff = sum > 0 ? (double)hwin[i]/(double)sum : 0;
        if(eff > maxEff) {
            maxEff = eff;
            pos = i;
        }
    }
    return pos;
}

double StrategyEffectiveness::getTSSF() {
    if(win == 0) return 0;
    double avg_win = grossprofit / (double) win;
    double avg_loss = loss == 0 ? 0 : grossloss / (double) loss;
    double win_perc =  100 * getEff();
    double  teor = (110.0 - win_perc) / (win_perc - 10.0) + 1.0;
    double  real = avg_loss == 0 ? avg_win : avg_win / avg_loss;
    double  tssf = real / teor;
    return tssf;
}
