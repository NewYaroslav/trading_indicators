#include "Martingale.hpp"

MartingaleAnatolia::MartingaleAnatolia() {
    moneyRate[0] = 30.0;
    moneyRate[1] = 75.0;
    moneyRate[2] = 180.0;
    moneyRate[3] = 540.0;
    moneyRate[4] = 1620.0;
    moneyRate[5] = 6000.0;
    moneyRate[6] = 18000.0;
    moneyRate[7] = 45000.0;
    vMoney.push_back(money);
};

void MartingaleAnatolia::setCoeff(double coeff) {
    MartingaleAnatolia::coeff = coeff;
}

void MartingaleAnatolia::win() {
    if(money > moneyRate[step] && stop == false) {
        money += moneyRate[step] * coeff - moneyRate[step];
        vMoney.push_back(money);
        step = 0;
        nwin++;
        eff = (nwin + nloss) > 0 ? (double)nwin / (double)(nwin + nloss) : 0.0;
    } else {
        stop = true;
    }
}

void MartingaleAnatolia::loss() {
    if(money > moneyRate[step] && stop == false) {
        money -= moneyRate[step];
        vMoney.push_back(money);
        step++;
        nloss++;
        eff = (nwin + nloss) > 0 ? (double)nwin / (double)(nwin + nloss) : 0.0;
        if(step == 6) {
            step = 0;
        }
    } else {
        stop = true;
    }
}
