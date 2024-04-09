
#ifndef SELECTABLEBUTTON_H
#define SELECTABLEBUTTON_H

#include "CustomStyledWidgets/CustomButton.h"
#include "StyleBase/ButtonStyles/SelectableButtonStyleHelper.h"

class SelectableButton : public CustomButton{
    Q_OBJECT;
public:
    SelectableButton(QWidget* parent = nullptr  , CustomButtonStyleHelper* helper = nullptr);
    void setIndex(int index);
    bool isSelected();
    int index();
    void setState(bool state);
signals:
    void selected();
    void unselected();
    void indexOnClick(int index);
protected:
    void mousePressEvent(QMouseEvent* ev);
protected:
    int Index = 0;
};

class TwoStateButton : public CustomButton{
    Q_OBJECT;
public:
    TwoStateButton(QWidget* parent = nullptr  , CustomButtonStyleHelper* helper = nullptr);
    bool isActive();
    void setActive(bool state);
};

#endif // SELECTABLEBUTTON_H
