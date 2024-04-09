
#ifndef WINDOWSTYLEHELPER_H
#define WINDOWSTYLEHELPER_H
#include "AnimationTool.h"
#include <QStyleOption>
#include <QPainter>
class WindowStyleHelper{
public:
    WindowStyleHelper() = default;
    void draw(const QStyleOption* styleOpt , QPainter* painter) const;
};

#endif // WINDOWSTYLEHELPER_H
