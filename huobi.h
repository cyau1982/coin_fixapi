#ifndef __FIXAPI_HUOBI_H
#define __FIXAPI_HUOBI_H

#include "baseapp.h"

namespace FIX {

class HuobiApp : public BaseApp {
private:
    static int timestamp() {
        struct timeval te;
        gettimeofday(&te, NULL);
        int sec = te.tv_sec + te.tv_usec / 1000000;
        return sec;
    }

    int getCoinType(const char *symbol);

public:
    virtual void submitMarketOrder(const char *symbol, OrderSide side, double amount, const char *userId = NULL);
    virtual void submitLimitOrder(const char *symbol, OrderSide side, double price, double amount, const char *userId = NULL);
    virtual void requestOrderStatus(const char *symbol, const char *orderId, OrderSide side);
};

}   /* namespace FIX */

#endif  /* __FIXAPI_HUOBI_H */
