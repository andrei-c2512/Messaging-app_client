#include "CustomButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"

CustomButtonStyleHelper::CustomButtonStyleHelper(QPushButton* button)
    :AnimationTool(button)
{
    changedState = _selected = false;
    _textVisible = true;
}
void CustomButtonStyleHelper::drawButtonShape(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    //painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(borderPen(option));
    painter->setBrush(backgroundBrush(option));
    painter->drawRoundedRect(option->rect.adjusted(0 , 0 , -1 , -1) , borderRadius , borderRadius);
}
void CustomButtonStyleHelper::drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(textPen(option));
    painter->setFont(pManagingWidget->font());
    if(!option->icon.isNull())
    {
        if(!option->text.isNull() && _textVisible)
        {
            const int spacing = 3;
            //const int iconX = (option->rect.width() - option->fontMetrics.horizontalAdvance(option->text) - option->iconSize.width() - spacing) / 2 ;
            //const int iconY = (option->rect.height() - option->fontMetrics.height() - option->iconSize.height() - spacing) / 2 ;

            option->icon.paint(painter , QRect(QPoint(option->rect.width() - option->fontMetrics.horizontalAdvance(option->text) - option->iconSize.width() - spacing / 2 ,
                                             (option->rect.height() - option->fontMetrics.height() - option->iconSize.height() - spacing) / 2) ,
                                              option->iconSize));
            painter->drawText(QRect(option->rect.topLeft() + QPoint(option->iconSize.width() + spacing , 0),
                                    QSize(option->fontMetrics.horizontalAdvance(option->text) , option->fontMetrics.height())) ,  option->text);
        }
        else
        {
            option->icon.paint(painter , option->rect , Qt::AlignCenter);
        }
    }
    else if(_textVisible)
        painter->drawText(option->rect , Qt::AlignCenter ,  option->text);
}
void CustomButtonStyleHelper::updateAnimations(const QStyleOptionButton* option)const noexcept{}
QPen CustomButtonStyleHelper::textPen(const QStyleOptionButton* option ) const noexcept
{
    return QPen(default_hover_selected_ROUTE(option , StyleRepository::Base::textColor() , StyleRepository::Base::hoverTextColor() , QColor()));
}
QPen CustomButtonStyleHelper::borderPen(const QStyleOptionButton* option ) const noexcept
{
    return QPen(default_hover_selected_ROUTE(option , StyleRepository::Base::borderColor() , StyleRepository::Base::hoverBorderColor() , QColor()));
}
QBrush CustomButtonStyleHelper::backgroundBrush(const QStyleOptionButton* option ) const noexcept
{
    return QBrush(default_hover_selected_ROUTE(option , StyleRepository::Base::backgroundColor() , StyleRepository::Base::hoverBackgroundColor() , QColor()));
}
void CustomButtonStyleHelper::setIconSize(QSize size) noexcept
{
    _iconSize = std::move(size);
    assert(pManagingWidget);
    static_cast<QPushButton*>(pManagingWidget)->setIconSize(_iconSize);
}
QSize CustomButtonStyleHelper::iconSize() const noexcept{ return _iconSize;  }
void CustomButtonStyleHelper::setManagingWidget(QPushButton* button) noexcept{
    pManagingWidget_Button = button ;
    AnimationTool::setManagingWidget(button);
}
QImage CustomButtonStyleHelper::icon() const noexcept
{
    auto& anim = *static_cast<IconColorAnimationManager*>(animationList[int(Type::IconColor)]);
    ImagePainter& painter = anim.painter();
    painter.setColor(anim.animation().currentValue().value<QColor>());
    return painter.image();
}



void CustomButtonStyleHelper::update(QMouseEvent* event) noexcept
{
    Q_UNUSED(event);
    _selected = !_selected;
    pManagingWidget->update();
    changedState = true;
}

bool  CustomButtonStyleHelper::selected() const noexcept { return _selected;}
void  CustomButtonStyleHelper::setSelected(bool val) noexcept{ _selected = std::move(val); pManagingWidget->update(); changedState = true;}
void CustomButtonStyleHelper::setTextVisible(bool visible) noexcept { _textVisible = visible; }