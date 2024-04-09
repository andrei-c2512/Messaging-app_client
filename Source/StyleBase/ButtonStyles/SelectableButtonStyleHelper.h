
#ifndef SELECTABLEBUTTONSTYLEHELPER_H
#define SELECTABLEBUTTONSTYLEHELPER_H
#include "CustomButtonStyleHelper.h"
class SelectableButtonStyleHelper : public CustomButtonStyleHelper{
public:
    SelectableButtonStyleHelper(QPushButton* button);
    //this should only be used in mouse press events
    virtual void updateAnimations(const QStyleOptionButton* option) const noexcept override;
protected:
    virtual QPen   textPen(const QStyleOptionButton* option ) const  noexcept override;
    virtual QPen   borderPen(const QStyleOptionButton* option ) const noexcept override;
    virtual QBrush backgroundBrush(const QStyleOptionButton* option ) const  noexcept override;

    QPixmap unselected_pixmap;
    QPixmap selected_pixmap;
    std::vector<QPixmap*> pixmapList;
    mutable const QPixmap* currentPixmap;
};

#endif // SELECTABLEBUTTONSTYLEHELPER_H
