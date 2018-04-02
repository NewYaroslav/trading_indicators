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


#ifndef FOREXSIMULATOR_HPP_INCLUDED
#define FOREXSIMULATOR_HPP_INCLUDED

#include <vector>

namespace ForForex {

    double calcProfitsLosses(double contract, double openPrice, double closePrice, int isBuyOrSell);

    class OrderData {
        private:
        public:
        OrderData();
        int type; ///< тип сделки
        int magic; ///< номер сделки
        double volume; ///< размер лота
        double contract; /// размер контракта
        double pledge;
        double openPrice; ///< цена на момент заключени€ контракта
        double closePrice; ///< цена на момент закрыти€ контракта
        double stoploss; ///< цена закрыти€ ордера при достижении уровн€ убыточности
        double takeprofit; ///< цена закрыти€ ордера при достижении уровн€ прибыльности
        int isClose; ///< флаг завершени€ сделки
        int isOpen; ///< флаг открыти€ ордера
        void setData(int _type, int _magic, double _volume, double _contract, double _pledge, double _openPrice, double _stoploss, double _takeprofit, int _isOpen);
    };

    class ForexSimulator {
        private:
        double onePoint; ///< шаг цены
        double startMoney; ///< начальный депозит
        double money;
        std::vector<double> vMoney; ///< график депозита
        double pledgeTot; ///< величина залога
        int leverage; ///< крединое плечо
        std::vector<OrderData> vOrder; ///< вектор всех ордеров
        double spread; ///< спред

        public:
        ForexSimulator();
        enum eCmd {
            OP_BUY = 1, ///< ѕокупка
            OP_SELL = -1, ///< ѕродажа
            OP_BUYLIMIT = 2, ///< ќтложенный ордер BUY LIMIT
            OP_SELLLIMIT = -2, ///< ќтложенный ордер SELL LIMIT
            OP_BUYSTOP = 3, ///< ќтложенный ордер BUY STOP
            OP_SELLSTOP = -3 ///< ќтложенный ордер SELL STOP
        };
        int OrderSend(eCmd cmd, double volume,double price, double priceReal, double stoploss, double takeprofit, int expiration);
        int OrderSend(eCmd cmd, double volume,double price, double bid, double ask, double stoploss, double takeprofit, int expiration);
        int OrderClose(int magic, double bid, double ask);
        int OrderClose(int magic, double price);
        int OrderDelete(int magic);
        //int OrderNewStoploss(int magic, double bid, double ask);
        //int OrderNewStoploss(int magic, double price);
        //int OrderNewTakeprofit(int magic);
        void updata(double open, double high, double low, double close);
        void updata(double bid, double ask);
        double getMoney() {return money;};
        void setSpread(double _spread) {spread = _spread;};
        void setLeverage(int _leverage) {leverage = _leverage;};
        std::vector<double> getVectorMoney() {return vMoney;};
        std::vector<OrderData> getVectorOrder() {return vOrder;};
    };
}
#endif // FOREXSIMULATOR_HPP_INCLUDED
