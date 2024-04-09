
#ifndef COLORSHIFTBUTTONSTYLEHELPER_H
#define COLORSHIFTBUTTONSTYLEHELPER_H
#include "CustomButtonStyleHelper.h"
#include <QImage>
class ColorShiftButtonStyleHelper : public CustomButtonStyleHelper{
public:
    ColorShiftButtonStyleHelper(QPushButton* parent = nullptr);
    void drawButtonShape(const QStyleOptionButton* option , QPainter* painter) const noexcept override;
    void drawLabel(const QStyleOptionButton* option , QPainter* painter) const  noexcept override;
    void updateAnimations(const QStyleOptionButton* option) const  noexcept override;
protected:
    QPen   borderPen(const QStyleOptionButton* option ) const noexcept override;
    QBrush backgroundBrush(const QStyleOptionButton* option ) const noexcept override;
    QPen   textPen(const QStyleOptionButton* option ) const noexcept override;
protected:
    void settings_setup() override;
    static constexpr int animDuration = 250;
};

#endif // COLORSHIFTBUTTONSTYLEHELPER_H
