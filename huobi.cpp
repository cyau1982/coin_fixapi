#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/OrderStatusRequest.h>

#include "huobi.h"
#include "md5/md5.h"

namespace FIX {

static std::string float2str(double d)
{
    char str[32];
    sprintf(str, "%lg", d);
    std::string s(str);
    if (!strchr(str, '.'))
        s = s + ".0";
    return s;
}

int HuobiApp::getCoinType(const char *symbol)
{
    if (!strcasecmp(symbol, "btc"))
        return 1;
    else if (!strcasecmp(symbol, "ltc"))
        return 2;
    else
        return 0;
}

void HuobiApp::submitMarketOrder(const char *symbol, OrderSide side, double amount, const char *userId)
{
    int ts = timestamp();
    FIX44::NewOrderSingle message;
    message.set(Account(_publicKey));
    if (userId)
        message.set(ClOrdID(userId));
    else
        message.set(ClOrdID("HuobiOrder"));
    message.set(Price(0.0));
    message.set(MinQty(amount));
    message.set(OrdType(OrdType_MARKET));
    message.set(Symbol(symbol));
    message.set(Side(side == OrderBuy ? Side_BUY : Side_SELL));
    message.set(TransactTime());
    message.setField(IntField(957, ts));
    message.setField(StringField(958, _publicKey));
    char str[1024];
    sprintf(str, "access_key=%s&amount=%s&coin_type=%d&created=%d&method=%s&secret_key=%s",
            _publicKey.c_str(),
            float2str(amount).c_str(),
            getCoinType(symbol),
            ts,
            side == OrderBuy ? "buy_market" : "sell_market",
            _privateKey.c_str());
    message.setField(StringField(959, md5::MD5String(str, false)));
    Session *session = Session::lookupSession(_sessionID);
    if (session)
        session->send(message);
}

void HuobiApp::submitLimitOrder(const char *symbol, OrderSide side, double price, double amount, const char *userId)
{
    int ts = timestamp();
    FIX44::NewOrderSingle message;
    message.set(Account(_publicKey));
    if (userId)
        message.set(ClOrdID(userId));
    else
        message.set(ClOrdID("HuobiOrder"));
    message.set(Price(price));
    message.set(MinQty(amount));
    message.set(OrdType(OrdType_LIMIT));
    message.set(Symbol(symbol));
    message.set(Side(side == OrderBuy ? Side_BUY : Side_SELL));
    message.set(TransactTime());
    message.setField(IntField(957, ts));
    message.setField(StringField(958, _publicKey));
    char str[1024];
    sprintf(str, "access_key=%s&amount=%s&coin_type=%d&created=%d&method=%s&price=%s&secret_key=%s",
            _publicKey.c_str(),
            float2str(amount).c_str(),
            getCoinType(symbol),
            ts,
            side == OrderBuy ? "buy" : "sell",
            float2str(price).c_str(),
            _privateKey.c_str());
    message.setField(StringField(959, md5::MD5String(str, false)));
    Session *session = Session::lookupSession(_sessionID);
    if (session)
        session->send(message);
}

void HuobiApp::requestOrderStatus(const char *symbol, const char *orderId, OrderSide side)
{
    int ts = timestamp();
    FIX44::OrderStatusRequest message;
    message.set(Account(_publicKey));
    message.set(ClOrdID(orderId));
    message.set(Symbol(symbol));
    message.set(Side(side == OrderBuy ? Side_BUY : Side_SELL));
    message.setField(IntField(957, ts));
    message.setField(StringField(958, _publicKey));
    char str[1024];
    sprintf(str, "access_key=%s&coin_type=%d&created=%d&id=%s&method=order_info&secret_key=%s",
            _publicKey.c_str(),
            getCoinType(symbol),
            ts,
            orderId,
            _privateKey.c_str());
    message.setField(StringField(959, md5::MD5String(str, false)));
    Session *session = Session::lookupSession(_sessionID);
    if (session)
        session->send(message);
}

}   /* namespace FIX */
