#include "CustomizableButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"

AnimationPrototype::AnimationPrototype(AnimationTool::Type type0 , QVariant start0 , QVariant end0 , bool directShift0)
    : start(std::move(start0)) , end(std::move(end0)) , type(std::move(type0)) , directShift(directShift0)
{
    try {
        if(type == AnimationTool::Type::IconColor)
            throw std::logic_error("Use derived class for icon color animation");
    } catch (std::logic_error& e) {
        qDebug() << e.what();
    }
}
IconColorAnimationPrototype::IconColorAnimationPrototype(QVariant start0 , QVariant end0 , bool directShift0 , QPixmap pixmap0 , QSize iconSize0)
    :AnimationPrototype(AnimationTool::Type::Count , std::move(start0) , std::move(end0) , directShift0) , pixmap(std::move(pixmap0)) , iconSize(iconSize0)
{
    type = AnimationTool::Type::IconColor;
}
CustomizableButtonStyleHelper::CustomizableButtonStyleHelper(QPushButton* button , std::vector<AnimationPrototype*> prototypeList0)
    :CustomButtonStyleHelper(button) , prototypeList(prototypeList0)
{
    _hasIconAnim = false;
    if(button)
    CustomizableButtonStyleHelper::settings_setup();
    lastDir = (QAbstractAnimation::Direction)3;
}

void CustomizableButtonStyleHelper::settings_setup()
{
    for(AnimationPrototype* prototype : prototypeList)
    {
        int animationDuration;
        animationDuration = (prototype->directShift) ? 0 : 250;
        switch(prototype->type)
        {
        case AnimationTool::Type::IconColor:
            if(IconColorAnimationPrototype* p = static_cast<IconColorAnimationPrototype*>(prototype))
            {
                AnimationTool::createIconColorTool(prototype->start , prototype->end , animationDuration , p->pixmap);
                QPushButton* w = static_cast<QPushButton*>(pManagingWidget);
                w->setIcon( p->pixmap);
                w->setIconSize(p->iconSize);
                _hasIconAnim = true;
            }
            break;
        default:
            AnimationTool::createTool(prototype->type , prototype->start , prototype->end , animationDuration);
            break;
        }
    }
}


static const int spacing = 3;
static const int iconX = 3;

QPoint CustomizableButtonStyleHelper::iconPos(const QStyleOptionButton* option , bool hasText) const noexcept
{
    int x = pManagingWidget->width() - option->iconSize.width();
    if(hasText && _textVisible)
        x = (x - pManagingWidget->fontMetrics().horizontalAdvance(option->text) - spacing) / 2;
    else
        x /=2 ;

    return QPoint( x, (option->rect.height() - option->iconSize.height()) / 2);
}


void CustomizableButtonStyleHelper::drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    painter->setPen(textPen(option));
    painter->setFont(pManagingWidget->font());
    if(!option->icon.isNull())
    {
        if(_hasIconAnim)
            painter->drawImage(QRect(iconPos(option , option->text.length()) , option->iconSize) , icon());
        else
            option->icon.paint(painter , QRect(iconPos(option , option->text.length()) , option->iconSize));

        if (_textVisible)
        {
            QRect rect = option->rect;
            //option->rect.adjusted(iconX + option->iconSize.width() , iconY + option->iconSize.height() , 0 , 0)
            rect.setX(option->iconSize.width() + rect.x() + spacing);
            painter->drawText(rect, Qt::AlignCenter, option->text);
        }
    }
    else if(_textVisible)
        painter->drawText(option->rect , Qt::AlignCenter ,  option->text);
}

void CustomizableButtonStyleHelper::updateAnimations(const QStyleOptionButton* option) const noexcept
{
    auto state = option->state;
    QAbstractAnimation::Direction currentDir = (state & QStyle::State_MouseOver) ?  QAbstractAnimation::Direction::Forward : QAbstractAnimation::Direction::Backward;

    if(lastDir != currentDir)
        setDirectionForActives(currentDir);
    lastDir = std::move(currentDir);
}

QPen   CustomizableButtonStyleHelper::borderPen(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    QPen pen (animationList[int(AnimationTool::Type::BorderColor)]->animation().currentValue().value<QColor>());
    pen.setWidth(borderWidth);
    return pen;
}
QPen   CustomizableButtonStyleHelper::textPen(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    QPen pen (animationList[int(AnimationTool::Type::TextColor)]->animation().currentValue().value<QColor>());
    pen.setWidth(borderWidth);
    return pen;
}
QBrush CustomizableButtonStyleHelper::backgroundBrush(const QStyleOptionButton* option ) const noexcept
{
    Q_UNUSED(option);
    return QBrush(animationList[int(AnimationTool::Type::BackgroundColor)]->animation().currentValue().value<QColor>());
}
