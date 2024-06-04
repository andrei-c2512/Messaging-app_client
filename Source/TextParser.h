#include <QByteArray>

namespace TextParser{
    int dataPos(const QByteArray& message, const QByteArray& separator, qsizetype start);
    QByteArray extractData(const QByteArray& message, qsizetype start);
    bool extractDataBool(const QByteArray& message, qsizetype start);
};