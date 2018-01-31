#ifndef STRATEGYEFFECTIVENESS_HPP_INCLUDED
#define STRATEGYEFFECTIVENESS_HPP_INCLUDED

#include <vector>

class StrategyEffectiveness {
    public:
    int hwin[24];
    int hloss[24];
    int win = 0; // количество выйграшей
    int loss = 0; // количество проигрышей
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
    double getConsistency();
    double getProfitStability();
    double getProfitLoss();
    double getAverageGeometricYield();
    double getCoeffSharpe();
    double getBalanceMaxCoeffSharpe();
    double getEff();

    double getAverageGeometricYieldMx();

    int getGoodHour();
    // коэффициент безопасности торговой системы
    double getTSSF();
};

#endif // STRATEGYEFFECTIVENESS_HPP_INCLUDED
