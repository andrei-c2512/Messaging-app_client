#include "MessageInfo.h"
#include "Tools.h"

const QString MessageInfo::imageSign = "url::";

//for MessageInfo
MessageInfo::MessageInfo(QObject* parent) : QObject(parent) {}
MessageInfo::MessageInfo(QObject* parent, QString name0, QString text0, QString timestamp0)
    :QObject(parent), _name(std::move(name0)), _text(std::move(text0))
{
    setTimestamp(std::move(timestamp0));
}

QString MessageInfo::name() const { return _name; }
QString MessageInfo::text() const { return _text; }
QDateTime MessageInfo::timestamp() const { return _timestamp; }
void MessageInfo::setAt(int i, QString str) {
    switch (i)
    {
    case 0:
        _name = std::move(str);
        return;
    case 1:
        _text = std::move(str);
        return;
    case 2:
        setTimestamp(std::move(str));
        return;
    default:
        return;
    }
}
void MessageInfo::setTimestamp(QString str)
{
    _timestamp = Tools::stringToDateTime(std::move(str));
}
void MessageInfo::setTimestamp(QDateTime t) { _timestamp = std::move(t); }
void MessageInfo::setName(QString str) { _name = std::move(str); }
void MessageInfo::setText(QString text) { _text = std::move(text); }
