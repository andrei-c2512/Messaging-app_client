#pragma once
#include <QString>
#include <QObject>
#include <QDateTime>

class MessageInfo : public QObject {
    Q_OBJECT
public:
    MessageInfo(QObject* parent = nullptr);
    MessageInfo(QObject* parent, QString name, QString text, QString timestamp);
    QString name() const;
    QString text() const;
    QDateTime timestamp() const;
    void setName(QString str);
    void setText(QString text);
    void setAt(int i, QString str);
    void setTimestamp(QString str);
    void setTimestamp(QDateTime str);
    static const QString imageSign;
signals:
    void outOfQueue();
private:
    QString _name;
    QString _text;
    QDateTime _timestamp;
};