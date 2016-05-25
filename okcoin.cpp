#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/OrderMassStatusRequest.h>

#include "okcoin.h"

namespace FIX {

void OkcoinApp::submitMarketOrder(const char *symbol, OrderSide side, double amount, const char *userId)
{
    int ts = timestamp();
    FIX44::NewOrderSingle message;
    message.set(Account(_publicKey + "," + _privateKey));
    if (userId)
        message.set(ClOrdID(userId));
    else
        message.set(ClOrdID("OkcoinOrder"));
    message.set(Price(amount));
    message.set(OrderQty(amount));
    message.set(OrdType(OrdType_MARKET));
    message.set(Symbol(symbol));
    message.set(Side(side == OrderBuy ? Side_BUY : Side_SELL));
    message.set(TransactTime());
    Session *session = Session::lookupSession(_sessionID);
    if (session)
        session->send(message);
}

void OkcoinApp::submitLimitOrder(const char *symbol, OrderSide side, double price, double amount, const char *userId)
{
    int ts = timestamp();
    FIX44::NewOrderSingle message;
    message.set(Account(_publicKey + "," + _privateKey));
    if (userId)
        message.set(ClOrdID(userId));
    else
        message.set(ClOrdID("OkcoinOrder"));
    message.set(Price(price));
    message.set(OrderQty(amount));
    message.set(OrdType(OrdType_LIMIT));
    message.set(Symbol(symbol));
    message.set(Side(side == OrderBuy ? Side_BUY : Side_SELL));
    message.set(TransactTime());
    Session *session = Session::lookupSession(_sessionID);
    if (session)
        session->send(message);
}

void OkcoinApp::requestOrderStatus(const char *symbol, const char *orderId, OrderSide side)
{
    int ts = timestamp();
    FIX44::OrderMassStatusRequest message;
    message.set(Account(_publicKey + "," + _privateKey));
    message.set(MassStatusReqID(orderId));
    message.set(MassStatusReqType(MassStatusReqType_STATUS_FOR_ORDERS_FOR_A_SECURITY));
    Session *session = Session::lookupSession(_sessionID);
    if (session)
        session->send(message);
}

}   /* namespace FIX */
