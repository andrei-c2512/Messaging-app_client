#include <QString>

namespace TextParser{
    int dataPos(const QString& message, const QString& separator, int start);
    QString extractData(const QString& message, int start);
    bool extractDataBool(const QString& message, int start);
};