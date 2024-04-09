
#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H
#include <QCommonStyle>
#include "StyleBase/PaletteAdjuster.h"
#include "StyleBase/WindowStyleHelper.h"
#include "CustomStyledWidgets/CustomButton.h"
#include <QProxyStyle>

using super = QProxyStyle;

class CustomStyle : public super{
public:
    CustomStyle();
    void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void polish(QWidget* widget) override;
    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *widget) const override;
private:
    std::unique_ptr<PaletteAdjuster> adjuster;
    std::unique_ptr<WindowStyleHelper> windowStyleHelper;
};

#endif // CUSTOMSTYLE_H
