#ifndef FOREXTESTING_HPP_INCLUDED
#define FOREXTESTING_HPP_INCLUDED

#include <vector>

namespace ForexTesting {

    double calcProfitsLosses(double contract, double openPrice, double closePrice, int isBuyOrSell);

    class Testing {
        public:
        int countId = 0;
        double money = 100000.0; ///< депозит в базовой валюте
        double onePoint = 0.00001; ///< шаг цены
        std::vector<double> vMoney;

        typedef struct sOrder {
            int id; ///< номер сделки
            double contract; /// размер контракта
            double lot; ///< размер лота
            double onePointValue; ///< стоимость пункта
            double openPrice; ///< цена на момент заключения контракта
            double closePrice; ///< цена на момент закрытия контракта
            double pledge; ///< залог
            double stopLoss;
            double takeProfit;
            int isBuyOrSell;
            int isClose;
        };

        std::vector<sOrder> vOrder;

        Testing();

        /** @brief установить ордер
            @param[in] price текущая цена
            @param[in] lot объем (лот)
            @param[in] leverage кредитное плечо
            @param[in] isBuyOrSell тип сделки (buy 1, sell -1)
            @param[in] spread размер спреда
            @param[in] stopLoss стоп лосс
            @param[in] takeProfit тейк профит
            @param[in] id номер сделки
        */
        void setOrder(double price, double lot, int leverage, int isBuyOrSell, double spread, double stopLoss, double takeProfit, int& id);

        /** @brief обновить данные по ордерам
            @param[in] priceHigh цена high бара
            @param[in] priceLow цена low бара
            @param[in] price цена закрытия бара
            @param[in] isBuyOrSell тип сделки (вверх 1, вниз -1)
            @param[in] spread размер спреда
        */
        void updata(double priceHigh, double priceLow, double price, double spread);

        /** @brief установить размер депозита
            @param[in] money деньги
        */
        void setMoney(double money);

        /** @brief получить статус сделки
            @param[in] id номер сделки
            @return вернет 1 в случае если сделка была закрыта
        */
        int getStateOrder(int id);

        /** @brief установить стоп лосс
            @param[in] id номер сделки
            @param[in] stopLoss стоп лосс
        */
        void setStopLoss(int id, double stopLoss);

        /** @brief Получить флаг типа сделки
            @param[in] id номер сделки
            @return вернет 1 в случае buy сделки, -1 в случае sell сделки, 0 в случае если сделка не найдена
        */
        int getBuySellFlag(int id);
    };

}

#endif // FOREXTESTING_HPP_INCLUDED
