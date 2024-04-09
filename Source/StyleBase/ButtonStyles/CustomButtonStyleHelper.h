
#ifndef CUSTOMBUTTONSTYLEHELPER_H
#define CUSTOMBUTTONSTYLEHELPER_H
#include "StyleBase/AnimationTool.h"
#include <QStyleOptionButton>
#include <QBrush>
#include <QPalette>
#include <QPainter>
#include <QPushButton>

/* on default , it's a simple black and white button with no color shift , ( you can change the black and white in the Base section of StyleRepository.h)
 * You have the option to add direct shift on hover , or cool color shift for border , text , background and icon */
class CustomButtonStyleHelper : public AnimationTool{
public:
    CustomButtonStyleHelper(QPushButton* button = nullptr);
    virtual ~CustomButtonStyleHelper() = default;
    virtual void drawButtonShape(const QStyleOptionButton* option , QPainter* painter) const noexcept;
    virtual void drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept;
    virtual void updateAnimations(const QStyleOptionButton* option) const noexcept;
    void setManagingWidget(QPushButton* button) noexcept;
    void setIconSize(QSize size) noexcept;
    QSize iconSize() const noexcept;

    void update(QMouseEvent* event) noexcept;
    void setSelected(bool selected) noexcept;
    bool selected() const noexcept;
    void setTextVisible(bool _visible) noexcept;
protected:
    virtual QPen   textPen(const QStyleOptionButton* option ) const noexcept;
    virtual QPen   borderPen(const QStyleOptionButton* option ) const noexcept;
    virtual QBrush backgroundBrush(const QStyleOptionButton* option ) const noexcept;
    using AnimationTool::setManagingWidget;
    virtual QImage icon() const noexcept;
protected:
    template<typename DrawingMaterial>
    /* DrawingMaterial should only be QGradient or QColor */
    inline static DrawingMaterial default_hover_selected_ROUTE(const QStyleOptionButton* option , DrawingMaterial defaultMaterial , DrawingMaterial hoverMaterial , DrawingMaterial selectedMaterial)
    {
        auto state = option->state;
        DrawingMaterial color;
        if(state & QStyle::State_Selected)
            color = selectedMaterial;
        else if((state & QStyle::State_MouseOver))
            color = hoverMaterial;
        else
            color = defaultMaterial;

        return color;
    }
    static constexpr int borderRadius = 5;
    static constexpr int borderWidth = 2;
    static constexpr int textWidth = 2;
    QSize _iconSize = { 16 , 16};
    mutable bool changedState;
    bool _selected;
    bool _textVisible;
    QPushButton* pManagingWidget_Button;

};

#endif // CUSTOMBUTTONSTYLEHELPER_H
