#include "CustomStyle.h"
//from the source code
static QWindow* qt_getWindow(const QWidget* widget)
{
    return widget ? widget->window()->windowHandle() : nullptr;
}


CustomStyle::CustomStyle() : QProxyStyle("fusion"){
    adjuster = std::make_unique<PaletteAdjuster>();
    windowStyleHelper = std::make_unique<WindowStyleHelper>();
}
void CustomStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget ) const
{
    switch (element)
    {
    case QStyle::ControlElement::CE_PushButton:
        if (const QStyleOptionButton* btn = qstyleoption_cast<const QStyleOptionButton*>(option)) {
            /* I've thinking about wheater to use dynamic cast or simply having ALL custom buttons have a specific name
             * the latter would be faster */
            if (const CustomButton* button = qobject_cast<const CustomButton*>(widget))
            {
                //super::drawControl(element , &copy , painter , button);
                button->styleHelper()->updateAnimations(btn);
                button->styleHelper()->drawButtonShape(btn, painter);
                button->styleHelper()->drawLabel(btn, painter);
            }
            //from source code
            else  {
                proxy()->drawControl(CE_PushButtonBevel, btn, painter, widget);
                QStyleOptionButton subopt = *btn;
                subopt.rect = subElementRect(SE_PushButtonContents, btn, widget);
                proxy()->drawControl(CE_PushButtonLabel, &subopt, painter, widget);
                if (btn->state & State_HasFocus) {
                    QStyleOptionFocusRect fropt;
                    fropt.QStyleOption::operator=(*btn);
                    fropt.rect = subElementRect(SE_PushButtonFocusRect, btn, widget);
                    proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
                }
            }
        }
        return;
    case QStyle::ControlElement::CE_PushButtonBevel:
        if (const QStyleOptionButton *btnOpt = qstyleoption_cast<const QStyleOptionButton *>(option))
        {
            if(const CustomButton* button = qobject_cast<const CustomButton*>(widget))
            {
                button->styleHelper()->drawButtonShape(btnOpt , painter);
            }
            //from source code
            else {
                QRect br = btnOpt->rect;
                int dbi = proxy()->pixelMetric(PM_ButtonDefaultIndicator, btnOpt, widget);
                if (btnOpt->features & QStyleOptionButton::DefaultButton)
                    proxy()->drawPrimitive(PE_FrameDefaultButton, option, painter, widget);
                if (btnOpt->features & QStyleOptionButton::AutoDefaultButton)
                    br.setCoords(br.left() + dbi, br.top() + dbi, br.right() - dbi, br.bottom() - dbi);
                if (!(btnOpt->features & (QStyleOptionButton::Flat | QStyleOptionButton::CommandLinkButton))
                    || btnOpt->state & (State_Sunken | State_On)
                    || (btnOpt->features & QStyleOptionButton::CommandLinkButton && btnOpt->state & State_MouseOver)) {
                    QStyleOptionButton tmpBtn = *btnOpt;
                    tmpBtn.rect = br;
                    proxy()->drawPrimitive(PE_PanelButtonCommand, &tmpBtn, painter, widget);
                }
                if (btnOpt->features & QStyleOptionButton::HasMenu) {
                    int mbi = proxy()->pixelMetric(PM_MenuButtonIndicator, btnOpt, widget);
                    QRect ir = btnOpt->rect;
                    QStyleOptionButton newBtn = *btnOpt;
                    newBtn.rect = QRect(ir.right() - mbi + 2, ir.height() / 2 - mbi / 2 + 3, mbi - 6, mbi - 6);
                    newBtn.rect = visualRect(btnOpt->direction, br, newBtn.rect);
                    proxy()->drawPrimitive(PE_IndicatorArrowDown, &newBtn, painter, widget);
                }
            }
            break;
        }
        return;
    case QStyle::ControlElement::CE_PushButtonLabel:
        if (const QStyleOptionButton *btnOpt = qstyleoption_cast<const QStyleOptionButton *>(option))
        {
            if(const CustomButton* customButton = qobject_cast<const CustomButton*>(widget))
            {
                customButton->styleHelper()->drawLabel(btnOpt , painter);
            }
            else
            {
                QRect textRect = btnOpt->rect;
                uint tf = Qt::AlignVCenter | Qt::TextShowMnemonic;
                if (!proxy()->styleHint(SH_UnderlineShortcut, btnOpt, widget))
                    tf |= Qt::TextHideMnemonic;
                if (!btnOpt->icon.isNull()) {
                    //Center both icon and text
                    QIcon::Mode mode = btnOpt->state & State_Enabled ? QIcon::Normal : QIcon::Disabled;
                    if (mode == QIcon::Normal && btnOpt->state & State_HasFocus)
                        mode = QIcon::Active;
                    QIcon::State state = QIcon::Off;
                    if (btnOpt->state & State_On)
                        state = QIcon::On;
                    QPixmap pixmap = btnOpt->icon.pixmap(qt_getWindow(widget), btnOpt->iconSize, mode, state);
                    int pixmapWidth = pixmap.width() / pixmap.devicePixelRatio();
                    int pixmapHeight = pixmap.height() / pixmap.devicePixelRatio();
                    int labelWidth = pixmapWidth;
                    int labelHeight = pixmapHeight;
                    int iconSpacing = 4;//### 4 is currently hardcoded in QPushButton::sizeHint()
                    if (!btnOpt->text.isEmpty()) {
                        int textWidth = btnOpt->fontMetrics.boundingRect(option->rect, tf, btnOpt->text).width();
                        labelWidth += (textWidth + iconSpacing);
                    }
                    QRect iconRect = QRect(textRect.x() + (textRect.width() - labelWidth) / 2,
                        textRect.y() + (textRect.height() - labelHeight) / 2,
                        pixmapWidth, pixmapHeight);
                    iconRect = visualRect(btnOpt->direction, textRect, iconRect);
                    if (btnOpt->direction == Qt::RightToLeft) {
                        tf |= Qt::AlignRight;
                        textRect.setRight(iconRect.left() - iconSpacing / 2);
                    }
                    else {
                        tf |= Qt::AlignLeft; //left align, we adjust the text-rect instead
                        textRect.setLeft(iconRect.left() + iconRect.width() + iconSpacing / 2);
                    }
                    if (btnOpt->state & (State_On | State_Sunken))
                        iconRect.translate(proxy()->pixelMetric(PM_ButtonShiftHorizontal, option, widget),
                            proxy()->pixelMetric(PM_ButtonShiftVertical, option, widget));
                    painter->drawPixmap(iconRect, pixmap);
                }
                else {
                    tf |= Qt::AlignHCenter;
                }
                if (btnOpt->state & (State_On | State_Sunken))
                    textRect.translate(proxy()->pixelMetric(PM_ButtonShiftHorizontal, option, widget),
                        proxy()->pixelMetric(PM_ButtonShiftVertical, option, widget));
                if (btnOpt->features & QStyleOptionButton::HasMenu) {
                    int indicatorSize = proxy()->pixelMetric(PM_MenuButtonIndicator, btnOpt, widget);
                    if (btnOpt->direction == Qt::LeftToRight)
                        textRect = textRect.adjusted(0, 0, -indicatorSize, 0);
                    else
                        textRect = textRect.adjusted(indicatorSize, 0, 0, 0);
                }
                proxy()->drawItemText(painter, textRect, tf, btnOpt->palette, (btnOpt->state & State_Enabled),
                    btnOpt->text, QPalette::ButtonText);
            }
        }

        return;
    default:
        super::drawControl(element , option , painter , widget);
     }
}
void CustomStyle::polish(QWidget* w)
{
    if (qobject_cast<QPushButton *>(w)) {
        w->setAttribute(Qt::WA_Hover);
    }
    super::polish(w);
}
void CustomStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *widget) const
{
    switch(pe)
    {
    case QStyle::PrimitiveElement::PE_FrameWindow:
        windowStyleHelper->draw(opt , p);
        return;
    case QStyle::PrimitiveElement::PE_Frame:
        windowStyleHelper->draw(opt , p);
        return;
    case PE_FrameFocusRect:
        // nothing, we don't want focus rects
        break;
    default:
      super::drawPrimitive(pe , opt , p , widget);
    }
}

