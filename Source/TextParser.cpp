#include "TextParser.h"

int TextParser::dataPos(const QByteArray& message, const QByteArray& separator, qsizetype start) {
    return message.indexOf(separator, start) + separator.length(); 
}
QByteArray TextParser::extractData(const QByteArray& message, qsizetype start) {
    return message.mid(start + 1, message.indexOf('"', start + 1) - start - 1);
}
bool TextParser::extractDataBool(const QByteArray& message, qsizetype start)
{
    return message[start + 1] == 't';
}