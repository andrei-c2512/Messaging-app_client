#include "EditButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"

EditButtonStyleHelper::EditButtonStyleHelper(SelectableButton* button)
    :SelectableButtonStyleHelper(button)
{
    if(button)
        EditButtonStyleHelper::settings_setup();
}
void EditButtonStyleHelper::settings_setup()
{
    const int duration = 250;
    _iconSize = { 16 , 16};
    startEditPixmap = QPixmap(StyleRepository::EditButton::StartEditState::icon());
    applyEditPixmap = QPixmap(StyleRepository::EditButton::ConfirmState::icon());
    currentPixmap = &startEditPixmap;

    AnimationTool::createTool(Type::BackgroundColor , StyleRepository::Base::backgroundColor() , StyleRepository::Base::hoverBackgroundColor() , duration);
    AnimationTool::createTool(Type::BorderColor , StyleRepository::EditButton::StartEditState::borderColor() ,
                              StyleRepository::EditButton::StartEditState::hoverBorderColor() , duration);
    AnimationTool::createIconColorTool(StyleRepository::EditButton::StartEditState::iconColor() ,
                                       StyleRepository::EditButton::StartEditState::hoverIconColor() , duration , startEditPixmap);

    QPushButton* btn = static_cast<QPushButton*>(pManagingWidget);
    btn->setIcon(startEditPixmap);
    btn->setIconSize(_iconSize);
}
void EditButtonStyleHelper::drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    QImage image = icon();
    QRect rect( QPoint((option->rect.width() - _iconSize.width()) / 2 , (option->rect.height() - _iconSize.height ()) / 2) ,_iconSize );
    painter->drawImage(std::move(rect) , std::move(image));
}
void EditButtonStyleHelper::updateAnimations(const QStyleOptionButton* option)const noexcept
{
    if(changedState)
    {
        QColor borderStart , borderEnd , iconStart , iconEnd;
        if(_selected)
        {
            borderStart = StyleRepository::EditButton::ConfirmState::hoverBorderColor();
            borderEnd = StyleRepository::EditButton::ConfirmState::borderColor();
            iconStart = StyleRepository::EditButton::ConfirmState::hoverIconColor();
            iconEnd = StyleRepository::EditButton::ConfirmState::iconColor();
            currentPixmap = &applyEditPixmap;
        }
        else
        {
            borderStart = StyleRepository::EditButton::StartEditState::borderColor();
            borderEnd = StyleRepository::EditButton::StartEditState::hoverBorderColor();
            iconStart = StyleRepository::EditButton::StartEditState::iconColor();
            iconEnd = StyleRepository::EditButton::StartEditState::hoverIconColor();
            currentPixmap = &startEditPixmap;
        }
        QVariantAnimation& borderAnim = animationList[int(Type::BorderColor)]->animation();
        borderAnim.setStartValue(std::move(borderStart));
        borderAnim.setEndValue(std::move(borderEnd));

        auto& iconAnimManager = *static_cast<IconColorAnimationManager*>(animationList[int(Type::IconColor)]);
        QVariantAnimation& iconAnim = iconAnimManager.animation();
        iconAnim.setStartValue(std::move(iconStart));
        iconAnim.setEndValue(std::move(iconEnd));
        ImagePainter& painter = iconAnimManager.painter();
        painter.setPixmap(*currentPixmap);
    }
    changedState = false;
    auto state = option->state;
    if(state & QStyle::State_MouseOver)
        setDirectionForActives(QAbstractAnimation::Direction::Forward);
    else
        setDirectionForActives(QAbstractAnimation::Direction::Backward);
}

QPen   EditButtonStyleHelper::borderPen(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    QPen pen (animationList[int(AnimationTool::Type::BorderColor)]->animation().currentValue().value<QColor>());
    pen.setWidth(borderWidth);
    return pen;
}
QBrush EditButtonStyleHelper::backgroundBrush(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    return QBrush(animationList[int(AnimationTool::Type::BackgroundColor)]->animation().currentValue().value<QColor>());
}
void EditButtonStyleHelper::setManagingWidget(SelectableButton* button) { SelectableButtonStyleHelper::setManagingWidget(button);}
