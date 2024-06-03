#include "Tools.h"

int Tools::_lastPos = 0;

QPoint Tools::windowPos(const QWidget* widget)
{
    QPoint pos;
    while (widget->parent() != nullptr)
    {
        pos += widget->pos();
        if (QWidget* w = qobject_cast<QWidget*>(widget->parent()))
            widget = w;
    }
    return pos;
}
//returns the position of the first string in the "searchedList" that has been found in the "str"
int Tools::indexOf(const QString& str, const std::vector<QString>& searchedList, int start)
{
    for (const QString& elem : searchedList)
    {
        int pos = str.indexOf(elem, start);
        if (pos != -1)
            return pos;
    }
    return -1;
}

QByteArray Tools::makeList(char beg, char end, char sep, std::vector<QByteArray> args)
{
    QByteArray result = "";
    result += beg;
    for (QByteArray& arg : args)
        result += arg + sep;

    result.back() = end;
    return result;
}
QByteArray Tools::intArrToByteArr(char beg, char end, int sep, std::vector<int> arr)
{
    QByteArray result = "";
    result += beg;
    for (const int& nr : arr)
        result += QByteArray::number(nr) + sep;

    result.back() = end;
    return result;
}
QByteArray Tools::intArrToByteArr(int sep, std::vector<int> arr)
{
    QByteArray result = "";
    for (const int& nr : arr)
        result += QByteArray::number(nr) + sep;

    result.removeLast();
    return result;
}
QString Tools::extractParameterStr(const QString& message, const QString& separator, int start)
{
    _lastPos = start;
    int index = message.indexOf(separator, _lastPos) + separator.length();
    _lastPos = index;
    return message.mid(index + 1, message.indexOf('"', index + 1) - index - 1);
}
QString Tools::extractParameterStr(const QString& message, const QString& separator)
{
    int index = message.indexOf(separator, _lastPos) + separator.length();
    _lastPos = index;
    return message.mid(index + 1, message.indexOf('"', index + 1) - index - 1);
}

bool Tools::extractParameterBool(const QString& message, const QString& separator, int start)
{
    _lastPos = start;
    int index = message.indexOf(separator, _lastPos) + separator.length();
    _lastPos = index;
    return message[index + 1] == 't';
}
bool Tools::extractParameterBool(const QString& message, const QString& separator)
{
    int index = message.indexOf(separator, _lastPos) + separator.length();
    _lastPos = index;
    return message[index + 1] == 't';
}

std::vector<int> Tools::extractParameterIntList(const QString& message, const QString& separator, int start)
{
    _lastPos = start;
    int index = message.indexOf(separator, _lastPos) + separator.length();
    _lastPos = index;
    return extractIntsFromArr(message, index);
}
std::vector<int> Tools::extractParameterIntList(const QString& message, const QString& separator)
{
    int index = message.indexOf(separator, _lastPos) + separator.length();
    _lastPos = index;
    return extractIntsFromArr(message, index);
}

int Tools::parameterPos(const QString& message, const QString& separator, int start)
{
    _lastPos = start;
    return message.indexOf(separator, _lastPos) + separator.length();
}
int Tools::parameterPos(const QString& message, const QString& separator)
{
    return message.indexOf(separator, _lastPos) + separator.length();
}

int Tools::lastPos() { return _lastPos; }

