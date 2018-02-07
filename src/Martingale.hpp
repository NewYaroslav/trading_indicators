#ifndef MARTINGALE_HPP_INCLUDED
#define MARTINGALE_HPP_INCLUDED

#include <vector>

class MartingaleAnatolia {
    public:
    double money = 100000.0;
    double moneyRate[8];
    double coeff = 1.9;
    int step = 0;
    bool stop = false;
    MartingaleAnatolia();
    void setCoeff(double coeff);
    void win();
    void loss();
    std::vector<double> vMoney;
    double eff = 0;
    int nwin = 0;
    int nloss = 0;
};

#endif // MARTINGALE_HPP_INCLUDED
