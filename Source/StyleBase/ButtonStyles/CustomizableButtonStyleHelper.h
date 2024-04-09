
#ifndef CUSTOMIZABLEBUTTONSTYLEHELPER_H
#define CUSTOMIZABLEBUTTONSTYLEHELPER_H
#include "CustomButtonStyleHelper.h"

class AnimationPrototype {
public:
    AnimationPrototype(AnimationTool::Type type , QVariant start , QVariant end , bool directShift);

    QVariant start;
    QVariant end;
    AnimationTool::Type type;
    bool directShift;
};

class IconColorAnimationPrototype : public AnimationPrototype{
public:
    IconColorAnimationPrototype(QVariant start , QVariant end , bool directShift , QPixmap pixmap0 , QSize iconSize);
    QPixmap pixmap;
    QSize   iconSize;
};

class CustomizableButtonStyleHelper : public CustomButtonStyleHelper{
public:
    CustomizableButtonStyleHelper(QPushButton* button , std::vector<AnimationPrototype*> prototypeList0);
    virtual void drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept override ;
    virtual void updateAnimations(const QStyleOptionButton* option)const noexcept override;
protected:
    virtual QPen   textPen(const QStyleOptionButton* option ) const noexcept override;
    virtual QPen   borderPen(const QStyleOptionButton* option ) const noexcept override;
    virtual QBrush backgroundBrush(const QStyleOptionButton* option ) const  noexcept override;
    virtual void settings_setup() override;
private:
    QPoint iconPos(const QStyleOptionButton* option , bool hasText) const noexcept;
protected:
    std::vector<AnimationPrototype*>prototypeList;
    bool _hasIconAnim;
    mutable QAbstractAnimation::Direction lastDir;
};

#endif // CUSTOMIZABLEBUTTONSTYLEHELPER_H
