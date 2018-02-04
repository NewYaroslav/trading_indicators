#include "ForexTesting.hpp"
#include <iostream>

namespace ForexTesting {

    double calcProfitsLosses(double contract, double openPrice, double closePrice, int isBuyOrSell) {
        if(isBuyOrSell == 1) {
            return ((contract * closePrice) - (contract * openPrice));
        }
        return ((contract * openPrice) - (contract * closePrice));
    }

    Testing::Testing() {};

    void Testing::setOrder(double price, double lot, int leverage, int isBuyOrSell, double spread, double stopLoss, double takeProfit, int& id) {
        const double LOT1 = 100000.0;
        sOrder order;
        order.lot = lot;
        if(isBuyOrSell == 1) {
            order.openPrice = price + spread;
        } else {
            order.openPrice = price;
        }
        order.contract = LOT1 * lot;
        double& contract = order.contract;
        order.pledge = contract / (double) leverage; // расчет залога в базовой валюте
        std::cout << "order.pledge: " << order.pledge << std::endl;
        // далее можно перевести залог в основную валюту по текущему курсу
        //...

        // рассчет стоимости одного пункта
        // OnePointValue = (Contract * (Price + OnePoint)) - (Contract * Price)
        order.onePointValue = (contract * (price + onePoint)) - contract * price;
        //std::cout << "onePointValue: " << order.onePointValue << std::endl;
        order.isBuyOrSell = isBuyOrSell;
        order.stopLoss = stopLoss;
        order.takeProfit = takeProfit;
        order.isClose = 0;
        countId++;
        order.id = countId;
        id = countId;
        //std::cout << "money: " << money << std::endl;
        //money -= ((spread/onePoint) * order.onePointValue);
        //std::cout << "money - spread: " << money << std::endl;
        //money -= order.contract;
        vOrder.push_back(order);
    }

    void Testing::updata(double priceHigh, double priceLow, double price, double spread) {
        for(int n = 0; n < (int)vOrder.size(); n++) {
            if(vOrder[n].isClose) continue;
            if(vOrder[n].isBuyOrSell == 1) {
                if(priceLow <= vOrder[n].stopLoss) {
                    // сделка закрылась в минус
                    std::cout << "buy loss: " << std::endl;
                    //money += vOrder[n].contract;
                    vOrder[n].closePrice = vOrder[n].stopLoss;
                    money += calcProfitsLosses(vOrder[n].contract, vOrder[n].openPrice,  vOrder[n].closePrice, vOrder[n].isBuyOrSell);
                    std::cout << "loss money: " << money << std::endl;
                    vOrder[n].isClose = 1;
                    vMoney.push_back(money);
                } else
                if(priceHigh >= vOrder[n].takeProfit) {
                    // сделка закрылась в плюс
                    std::cout << "buy win: " << std::endl;
                    //money += vOrder[n].contract;
                    vOrder[n].closePrice = vOrder[n].takeProfit;
                    money += calcProfitsLosses(vOrder[n].contract, vOrder[n].openPrice,  vOrder[n].closePrice, vOrder[n].isBuyOrSell);
                    std::cout << "win money: " << money << std::endl;
                    vOrder[n].isClose = 1;
                    vMoney.push_back(money);
                }
            } else {
                if(priceHigh + spread >= vOrder[n].stopLoss) {
                    // сделка закрылась в минус
                    std::cout << "sell loss: " << std::endl;
                    vOrder[n].closePrice = vOrder[n].stopLoss;
                    money += calcProfitsLosses(vOrder[n].contract, vOrder[n].openPrice,  vOrder[n].closePrice, vOrder[n].isBuyOrSell);
                    vOrder[n].isClose = 1;
                    vMoney.push_back(money);
                } else
                if(priceLow + spread <= vOrder[n].takeProfit) {
                    // сделка закрылась в плюс
                    std::cout << "sell win: " << std::endl;
                    vOrder[n].closePrice = vOrder[n].takeProfit;
                    money += calcProfitsLosses(vOrder[n].contract, vOrder[n].openPrice,  vOrder[n].closePrice, vOrder[n].isBuyOrSell);
                    vOrder[n].isClose = 1;
                    vMoney.push_back(money);
                }
            }
        }
    }

    void Testing::setMoney(double money) {
        Testing::money = money;
        vMoney.clear();
        vMoney.push_back(money);
    }

    int Testing::getStateOrder(int id) {
        for(int n = 0; n < (int)vOrder.size(); n++) {
            if(vOrder[n].id == id) return vOrder[n].isClose;
        }
        return -1;
    }

    void Testing::setStopLoss(int id, double stopLoss) {
        for(int n = 0; n < (int)vOrder.size(); n++) {
            if(vOrder[n].id == id) vOrder[n].stopLoss = stopLoss;
        }
    }

    int Testing::getBuySellFlag(int id) {
        for(int n = 0; n < (int)vOrder.size(); n++) {
            if(vOrder[n].id == id) return vOrder[n].isBuyOrSell;
        }
        return 0;
    }
}
