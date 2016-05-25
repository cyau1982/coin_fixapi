#ifndef __FIXAPI_BASEAPP_H
#define __FIXAPI_BASEAPP_H

#include <quickfix/Application.h>
#include <quickfix/Session.h>
#include <quickfix/fix44/ExecutionReport.h>
#include <quickfix/fix44/Logon.h>

namespace FIX {

typedef enum {
    OrderBuy = 0,
    OrderSell,
} OrderSide;

class BaseApp : public Application {
protected:
    std::string _publicKey;
    std::string _privateKey;
    SessionID _sessionID;

    virtual void onCreate(const SessionID &sessionID) {
        _sessionID = sessionID;
    }

    virtual void onLogon(const SessionID &sessionID) {
    }

    virtual void onLogout(const SessionID &sessionID) {
    }

    virtual void toAdmin(Message &message, const SessionID &sessionID) {
        MsgType msgType;
        message.getHeader().getField(msgType);
        if (msgType == FIX44::Logon::MsgType()) {
            message.setField(Username(_publicKey));
            message.setField(Password(_privateKey));
            message.setField(ResetSeqNumFlag(true));
        }
    }

    virtual void toApp(Message &message, const SessionID &sessionID) throw(DoNotSend) {
    }

    virtual void fromAdmin(const Message &message, const SessionID &sessionID)
        throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon) {
    }

    virtual void fromApp(const Message &message, const SessionID &sessionID)
        throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType) {
        MsgType msgType;
        message.getHeader().getField(msgType);
        if (msgType == FIX44::ExecutionReport::MsgType())
            parseExecutionReport(static_cast<const FIX44::ExecutionReport &>(message));
        else if (msgType.getValue()[0] == 'Z')
            parseUserReport(message);
    }

    virtual void parseExecutionReport(const FIX44::ExecutionReport &message)
        throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType) {
    }

    virtual void parseUserReport(const Message &message)
        throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType) {
    }

public:
    void setKeys(const std::string &publicKey, const std::string &privateKey) {
        _publicKey = publicKey;
        _privateKey = privateKey;
    }
    virtual void submitMarketOrder(const char *symbol, OrderSide side, double amount, const char *userId = NULL) = 0;
    virtual void submitLimitOrder(const char *symbol, OrderSide side, double price, double amount, const char *userId = NULL) = 0;
    virtual void requestOrderStatus(const char *symbol, const char *orderId, OrderSide side) = 0;
};

}   /* namespace FIX */

#endif  /* __FIXAPI_BASEAPP_H */
