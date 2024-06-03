#include "TextParser.h"

int TextParser::dataPos(const QString& message, const QString& separator, int start) {
    return message.indexOf(separator, start) + separator.length(); 
}
QString TextParser::extractData(const QString& message, int start) {
    return message.mid(start + 1, message.indexOf('"', start + 1) - start - 1);
}
bool TextParser::extractDataBool(const QString& message, int start)
{
    return message[start + 1] == 't';
}