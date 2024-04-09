
#ifndef EDITBUTTONSTYLEHELPER_H
#define EDITBUTTONSTYLEHELPER_H
#include "SelectableButtonStyleHelper.h"
#include "CustomStyledWidgets/SelectableButton.h"

class EditButtonStyleHelper : public SelectableButtonStyleHelper{
public:
    EditButtonStyleHelper(SelectableButton* button);

    void drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept override;
    void updateAnimations(const QStyleOptionButton* option)const noexcept override;
    void setManagingWidget(SelectableButton* button);
protected:
    QPen   borderPen(const QStyleOptionButton* option ) const noexcept override;
    QBrush backgroundBrush(const QStyleOptionButton* option ) const noexcept override;
    void settings_setup() override;

    using SelectableButtonStyleHelper::setManagingWidget;
private:
    QPixmap applyEditPixmap;
    QPixmap startEditPixmap;

    //auxiliary , changing it does not the state of the object changed
    mutable const QPixmap* currentPixmap;
};

#endif // EDITBUTTONSTYLEHELPER_H
