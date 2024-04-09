#include "SelectableButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"

SelectableButtonStyleHelper::SelectableButtonStyleHelper(QPushButton* button)
    :CustomButtonStyleHelper(button)
{
    pixmapList.resize(2);
    pixmapList[false] = &unselected_pixmap;
    pixmapList[true] = &selected_pixmap;
}
QPen   SelectableButtonStyleHelper::textPen(const QStyleOptionButton* option ) const noexcept
{
    if(_selected)
    {
        QPen pen (StyleRepository::SelectableButton::selectedTextColor());
        pen.setWidth(borderWidth);
        return pen;
    }
    else
        return CustomButtonStyleHelper::textPen(option);
}
QPen   SelectableButtonStyleHelper::borderPen(const QStyleOptionButton* option ) const noexcept
{
    if(_selected)
    {
        QPen pen (StyleRepository::SelectableButton::selectedBorderColor());
        pen.setWidth(borderWidth);
        return pen;
    }
    else
        return CustomButtonStyleHelper::borderPen(option);
}
QBrush SelectableButtonStyleHelper::backgroundBrush(const QStyleOptionButton* option ) const noexcept
{
    if(_selected)
    {
        QBrush brush(StyleRepository::Base::hoverBackgroundColor());
        return brush;
    }
    else
        return CustomButtonStyleHelper::backgroundBrush(option);
}
void SelectableButtonStyleHelper::updateAnimations(const QStyleOptionButton* option) const noexcept
{
    if(changedState)
        currentPixmap = pixmapList[_selected];
}
