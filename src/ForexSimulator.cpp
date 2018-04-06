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

#include "ForexSimulator.hpp"
#include <iostream>

namespace ForForex {

    OrderData::OrderData() {};

    void OrderData::setData(int _type, int _magic, double _volume, double _contract, double _pledge, double _openPrice, double _stoploss, double _takeprofit, int _isOpen) {
        type = _type;
        magic = _magic;
        volume = _volume;
        contract = _contract;
        pledge = _pledge;
        openPrice = _openPrice;
        stoploss = _stoploss;
        takeprofit = _takeprofit;
        isOpen = _isOpen;
        isClose = 0;
    }

    ForexSimulator::ForexSimulator() {
        spread = 0;
        vOrder.clear();
        vMoney.clear();
        onePoint = 0.00001;
        startMoney = 100000.0;
        money = startMoney;
        pledgeTot = 0;
        leverage = 1;
        vMoney.push_back(startMoney);
    }

    double ForexSimulator::getLot(double rate) {
        double _money = vMoney.back() * rate;
        const double LOT1 = 100000.0;
        return (_money / (LOT1 / (double)leverage));
    }

    int ForexSimulator::OrderSend(eCmd cmd, double volume, double price, double priceReal, double stoploss = 0, double takeprofit = 0, int expiration = -1) {
        return OrderSend(cmd, volume, price, priceReal, priceReal + spread, stoploss, takeprofit, expiration);

    }

    int ForexSimulator::OrderSend(eCmd cmd, double volume,double price, double bid, double ask, double stoploss = 0, double takeprofit = 0, int expiration = -1) {
        if(ask < bid) {
            std::cout << "Error: ask < bid" << std::endl;
            return 0;
        }
        const double LOT1 = 100000.0;
        double contract = LOT1 * volume; // размер контракта
        double pledge = contract / (double) leverage; // размер залога

        if(money < pledge) {
            std::cout << "Error: Pledge more than available funds: " << vMoney.back() << " / " << pledge << std::endl;
            return 0;
        }

        OrderData iOrder;
        int id = vOrder.size() + 1;

        switch(cmd) {
            case OP_BUY:
                if((ask >= takeprofit || price >= takeprofit) && takeprofit != 0) {
                    std::cout << "Error: Takeprofit must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                if((ask <= stoploss || price <= stoploss) && stoploss != 0) {
                    std::cout << "Error: Stoploss must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                iOrder.setData(cmd,id,volume,contract,pledge,ask,stoploss,takeprofit,1);
                vOrder.push_back(iOrder);
                std::cout << "create buy " << id << std::endl;
            break;
            case OP_SELL:
                if((bid <= takeprofit || price <= takeprofit) && takeprofit != 0) {
                    std::cout << "Error: Takeprofit must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                if((bid >= stoploss || price >= stoploss) && stoploss != 0) {
                    std::cout << "Error: Stoploss must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                iOrder.setData(cmd,id,volume,contract,pledge,bid,stoploss,takeprofit,1);
                vOrder.push_back(iOrder);
                std::cout << "create sell " << id << std::endl;
            break;
            case OP_BUYLIMIT:
                if(price >= takeprofit && takeprofit != 0) {
                    std::cout << "Error: Takeprofit must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price <= stoploss && stoploss != 0) {
                    std::cout << "Error: Stoploss must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price > ask) {
                    std::cout << "Error: ask must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                iOrder.setData(cmd,id,volume,contract,pledge,price,stoploss,takeprofit,0);
                vOrder.push_back(iOrder);
                std::cout << "create buy " << id << std::endl;
            case OP_BUYSTOP:
                if(price >= takeprofit && takeprofit != 0) {
                    std::cout << "Error: Takeprofit must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price <= stoploss && stoploss != 0) {
                    std::cout << "Error: Stoploss must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price < ask) {
                    std::cout << "Error: ask must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                iOrder.setData(cmd,id,volume,contract,pledge,price,stoploss,takeprofit,0);
                vOrder.push_back(iOrder);
                std::cout << "create buy " << id << std::endl;
            break;
            case OP_SELLLIMIT:
                if((bid <= takeprofit || price <= takeprofit) && takeprofit != 0) {
                    std::cout << "Error: Takeprofit must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                if((bid >= stoploss || price >= stoploss) && stoploss != 0) {
                    std::cout << "Error: Stoploss must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price < bid) {
                    std::cout << "Error: bid must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                iOrder.setData(cmd,id,volume,contract,pledge,price,stoploss,takeprofit,0);
                vOrder.push_back(iOrder);
                std::cout << "create sell " << id << std::endl;
            case OP_SELLSTOP:
                if(price <= takeprofit && takeprofit != 0) {
                    std::cout << "Error: Takeprofit must be less than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price >= stoploss && stoploss != 0) {
                    std::cout << "Error: Stoploss must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                if(price > bid) {
                    std::cout << "Error: bid must be larger than opening price of transaction" << std::endl;
                    return 0;
                }
                iOrder.setData(cmd,id,volume,contract,pledge,price,stoploss,takeprofit,0);
                vOrder.push_back(iOrder);
                std::cout << "create sell " << id << std::endl;
            break;
        }
        money -= pledge;
        pledgeTot += pledge;
        return id;
    }

    void ForexSimulator::updata(double open, double high, double low, double close) {
        for(int i = 0; i < (int)vOrder.size(); i++) {
            if(vOrder[i].isOpen == 0 && vOrder[i].isClose == 0) { // проверяем уровни для открытия сделок
                switch(vOrder[i].type) {
                    case OP_BUYLIMIT:
                    if(low + spread >= vOrder[i].openPrice) {
                        vOrder[i].isOpen = 1;
                        std::cout << "buy open " << vOrder[i].magic << std::endl;
                    }
                    case OP_BUYSTOP:
                    if(high + spread >= vOrder[i].openPrice) {
                        vOrder[i].isOpen = 1;
                        std::cout << "buy open " << vOrder[i].magic << std::endl;
                    }
                    break;
                    case OP_SELLLIMIT:
                    if(low  >= vOrder[i].openPrice) {
                        vOrder[i].isOpen = 1;
                        std::cout << "sell open " << vOrder[i].magic << std::endl;
                    }
                    case OP_SELLSTOP:
                    if(high >= vOrder[i].openPrice) {
                        vOrder[i].isOpen = 1;
                        std::cout << "sell open " << vOrder[i].magic << std::endl;
                    }
                    break;
                }
            }
            if(vOrder[i].isOpen == 1 && vOrder[i].isClose == 0) { // если сделка открыта, проверяем уровни, если есть
                if(vOrder[i].stoploss != 0) { // проверяем уровень стоп лосс
                    switch(vOrder[i].type) {
                        case OP_BUY:
                        case OP_BUYLIMIT:
                        case OP_BUYSTOP:
                        if(low <= vOrder[i].stoploss) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].stoploss;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].stoploss,1);
                            vMoney.push_back(money);
                            std::cout << "buy close " << vOrder[i].magic << std::endl;
                        }
                        break;
                        case OP_SELL:
                        case OP_SELLLIMIT:
                        case OP_SELLSTOP:
                        if(high + spread >= vOrder[i].stoploss) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].stoploss;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].stoploss,-1);
                            vMoney.push_back(money);
                            std::cout << "sell close " << vOrder[i].magic << std::endl;
                        }
                        break;
                    }
                } // if
                if(vOrder[i].takeprofit != 0) { // проверяем уровень тейк профита
                    switch(vOrder[i].type) {
                        case OP_BUY:
                        case OP_BUYLIMIT:
                        case OP_BUYSTOP:
                        if(high >= vOrder[i].takeprofit) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].takeprofit;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].takeprofit,1);
                            vMoney.push_back(money);
                            std::cout << "buy close " << vOrder[i].magic << std::endl;
                        }
                        break;
                        case OP_SELL:
                        case OP_SELLLIMIT:
                        case OP_SELLSTOP:
                        if(low + spread <= vOrder[i].takeprofit) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].takeprofit;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].takeprofit,-1);
                            vMoney.push_back(money);
                            std::cout << "sell close " << vOrder[i].magic << std::endl;
                        }
                        break;
                    } // if
                } // if takeprofit
            } // if

        } // for
    }

    void ForexSimulator::updata(double bid, double ask) {
        for(int i = 0; i < (int)vOrder.size(); i++) {
            if(vOrder[i].isOpen == 0 && vOrder[i].isClose == 0) { // проверяем уровни для открытия сделок
                switch(vOrder[i].type) {
                    case OP_BUYLIMIT:
                    case OP_BUYSTOP:
                    if(ask >= vOrder[i].openPrice) {
                        vOrder[i].isOpen = 1;
                    }
                    break;
                    case OP_SELLLIMIT:
                    case OP_SELLSTOP:
                    if(bid >= vOrder[i].openPrice) {
                        vOrder[i].isOpen = 1;
                    }
                    break;
                }
            }
            if(vOrder[i].isOpen == 1 && vOrder[i].isClose == 0) { // если сделка открыта, проверяем уровни, если есть
                if(vOrder[i].stoploss != 0) { // проверяем уровень стоп лосс
                    switch(vOrder[i].type) {
                        case OP_BUY:
                        case OP_BUYLIMIT:
                        case OP_BUYSTOP:
                        if(bid <= vOrder[i].stoploss) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].stoploss;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].stoploss,1);
                            vMoney.push_back(money);
                        }
                        break;
                        case OP_SELL:
                        case OP_SELLLIMIT:
                        case OP_SELLSTOP:
                        if(ask >= vOrder[i].stoploss) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].stoploss;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].stoploss,-1);
                            vMoney.push_back(money);
                        }
                        break;
                    }
                } // if
                if(vOrder[i].takeprofit != 0) { // проверяем уровень тейк профита
                    switch(vOrder[i].type) {
                        case OP_BUY:
                        case OP_BUYLIMIT:
                        case OP_BUYSTOP:
                        if(bid >= vOrder[i].takeprofit) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].takeprofit;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].takeprofit,1);
                            vMoney.push_back(money);
                        }
                        break;
                        case OP_SELL:
                        case OP_SELLLIMIT:
                        case OP_SELLSTOP:
                        if(ask <= vOrder[i].takeprofit) {
                            vOrder[i].isClose = 1;
                            vOrder[i].closePrice = vOrder[i].takeprofit;
                            money += vOrder[i].pledge;
                            pledgeTot -= vOrder[i].pledge;
                            money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].takeprofit,-1);
                            vMoney.push_back(money);
                        }
                        break;
                    } // if
                } // if takeprofit
            } // if

        } // for
    }

    int ForexSimulator::OrderClose(int magic, double price) {
        return OrderClose(magic, price, price + spread);
    }

    int ForexSimulator::OrderClose(int magic, double bid, double ask) {
        for(int i = 0; i < (int)vOrder.size(); i++) {
            if(vOrder[i].magic == magic && vOrder[i].isClose == 0) {
                if(vOrder[i].isOpen == 0) {
                    vOrder[i].isClose = 1;
                    money += vOrder[i].pledge;
                    pledgeTot -= vOrder[i].pledge;
                    return 1;
                }
                switch(vOrder[i].type) {
                    case OP_BUY:
                    case OP_BUYLIMIT:
                    case OP_BUYSTOP:
                        vOrder[i].isClose = 1;
                        if(vOrder[i].stoploss != 0 && bid <= vOrder[i].stoploss) {
                            vOrder[i].closePrice = vOrder[i].stoploss;
                        } else
                        if(vOrder[i].takeprofit != 0 && bid >= vOrder[i].takeprofit) {
                            vOrder[i].closePrice = vOrder[i].takeprofit;
                        } else {
                            vOrder[i].closePrice = bid;
                        }

                        money += vOrder[i].pledge;
                        pledgeTot -= vOrder[i].pledge;
                        money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].closePrice,1);
                        vMoney.push_back(money);
                    break;
                    case OP_SELL:
                    case OP_SELLLIMIT:
                    case OP_SELLSTOP:
                        vOrder[i].isClose = 1;
                        if(vOrder[i].stoploss != 0 && ask >= vOrder[i].stoploss) {
                            vOrder[i].closePrice = vOrder[i].stoploss;
                        } else
                        if(vOrder[i].takeprofit != 0 && ask <= vOrder[i].takeprofit) {
                            vOrder[i].closePrice = vOrder[i].takeprofit;
                        } else {
                            vOrder[i].closePrice = ask;
                        }


                        money += vOrder[i].pledge;
                        pledgeTot -= vOrder[i].pledge;
                        money += calcProfitsLosses(vOrder[i].contract,vOrder[i].openPrice,vOrder[i].closePrice,-1);
                        vMoney.push_back(money);
                    break;
                } // if
                return 1;
            } // if
        } // for
        return 0;
    }

    int ForexSimulator::OrderDelete(int magic) {
        for(int i = 0; i < (int)vOrder.size(); i++) {
            if(vOrder[i].magic == magic && vOrder[i].isOpen == 0 && vOrder[i].isClose == 0) {
                money += vOrder[i].pledge;
                pledgeTot -= vOrder[i].pledge;
                vOrder.erase(vOrder.begin() + i);
                return 1;
            } else
            if(vOrder[i].magic == magic && vOrder[i].isOpen == 0 && vOrder[i].isClose == 1) {
                vOrder.erase(vOrder.begin() + i);
                return 1;
            } else
            if(vOrder[i].magic == magic && vOrder[i].isOpen == 1 && vOrder[i].isClose == 1) {
                vOrder.erase(vOrder.begin() + i);
                return 1;
            }
        }
        return 0;
    }

    #if(0)
    int OrderNewStoploss(int magic, double bid, double ask) {
        for(int i = 0; i < (int)vOrder.size(); i++) {
            if(vOrder[i].magic == magic && vOrder[i].isOpen == 0 && vOrder[i].isClose == 0) {
                money += vOrder[i].pledge;
                pledgeTot -= vOrder[i].pledge;
                vOrder.erase(vOrder.begin() + i);
                return 1;
            } else
            if(vOrder[i].magic == magic && vOrder[i].isOpen == 0 && vOrder[i].isClose == 1) {
                vOrder.erase(vOrder.begin() + i);
                return 1;
            } else
            if(vOrder[i].magic == magic && vOrder[i].isOpen == 1 && vOrder[i].isClose == 1) {
                vOrder.erase(vOrder.begin() + i);
                return 1;
            }
        }
        return 0;
    }

    #endif

    int OrderNewTakeprofit(int magic);

    double calcProfitsLosses(double contract, double openPrice, double closePrice, int isBuyOrSell) {
        if(isBuyOrSell == 1) {
            return ((contract * closePrice) - (contract * openPrice));
        }
        return ((contract * openPrice) - (contract * closePrice));
    }


}
