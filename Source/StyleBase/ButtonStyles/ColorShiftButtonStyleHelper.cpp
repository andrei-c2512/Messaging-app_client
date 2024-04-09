#include "ColorShiftButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"
ColorShiftButtonStyleHelper::ColorShiftButtonStyleHelper(QPushButton* parent)
    :CustomButtonStyleHelper(parent)
{
    if(parent)
        ColorShiftButtonStyleHelper::settings_setup();
}

void ColorShiftButtonStyleHelper::settings_setup()
{
    assert(pManagingWidget);
    _iconSize = { 24 , 24};
    AnimationTool::createTool(AnimationTool::Type::BackgroundColor , StyleRepository::Base::backgroundColor() , StyleRepository::Base::hoverBackgroundColor() , animDuration);
    AnimationTool::createTool(AnimationTool::Type::BorderColor     , StyleRepository::AddButton::borderColor() , StyleRepository::AddButton::hoverBorderColor() , animDuration);
    AnimationTool::createTool(AnimationTool::Type::TextColor       , StyleRepository::AddButton::borderColor() , StyleRepository::AddButton::hoverBorderColor() , animDuration);
    QPixmap pixmap = QPixmap(":/images/addIcon.png");
    AnimationTool::createIconColorTool(StyleRepository::AddButton::iconColor() , StyleRepository::AddButton::hoverIconColor() , animDuration , pixmap);
    QPushButton* btn = static_cast<QPushButton*>(pManagingWidget);
    btn->setIcon(std::move(pixmap));
    btn->setIconSize(_iconSize);
}

void ColorShiftButtonStyleHelper::drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    IconColorAnimationManager* animManager = static_cast<IconColorAnimationManager*>(animationList[int(Type::IconColor)]);
    ImagePainter& imgPainter = animManager->painter();
    imgPainter.setColor(animManager->animation().currentValue().value<QColor>());

    int centerX = (pManagingWidget->width() - _iconSize.width()) / 2;
    int centerY = (pManagingWidget->height() - _iconSize.height()) / 2;
    painter->drawImage(QRect(QPoint(std::move(centerX) , std::move(centerY)) , _iconSize), imgPainter.image());

}
void ColorShiftButtonStyleHelper::drawButtonShape(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    const int radius = std::min(option->rect.height() , option->rect.width()) / 4;
    painter->setPen(borderPen(option));
    painter->setBrush(backgroundBrush(option));
    painter->drawRoundedRect(option->rect.adjusted(1,1, -1 , -1) , radius ,  radius);
}
QPen   ColorShiftButtonStyleHelper::borderPen(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    QPen pen (animationList[int(AnimationTool::Type::BorderColor)]->animation().currentValue().value<QColor>());
    pen.setWidth(borderWidth);
    return pen;
}
QPen   ColorShiftButtonStyleHelper::textPen(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    QPen pen (animationList[int(AnimationTool::Type::BorderColor)]->animation().currentValue().value<QColor>());
    pen.setWidth(borderWidth);
    return pen;
}
QBrush ColorShiftButtonStyleHelper::backgroundBrush(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    return QBrush(animationList[int(AnimationTool::Type::BackgroundColor)]->animation().currentValue().value<QColor>());
}
void ColorShiftButtonStyleHelper::updateAnimations(const QStyleOptionButton* option) const noexcept
{
    auto state = option->state;
    if(state & QStyle::State_MouseOver)
        setDirectionForActives(QAbstractAnimation::Direction::Forward);
    else
        setDirectionForActives(QAbstractAnimation::Direction::Backward);
}
