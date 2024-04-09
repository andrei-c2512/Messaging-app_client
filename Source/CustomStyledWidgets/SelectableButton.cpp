#include "SelectableButton.h"

SelectableButton::SelectableButton(QWidget* parent , CustomButtonStyleHelper* helper)
    :CustomButton(parent)
{
    if(helper)
    setStyleHelper(helper);
}


void SelectableButton::mousePressEvent(QMouseEvent* ev)
{
    pStyleHelper->update(ev);
    if(pStyleHelper->selected())
        emit selected();
    else
        emit unselected();
    CustomButton::mousePressEvent(ev);
}

void SelectableButton::setIndex(int index)
{
    Index = std::move(index);
}
bool SelectableButton::isSelected(){ return pStyleHelper->selected();}
int SelectableButton::index(){ return Index;}
void SelectableButton::setState(bool state) {
    pStyleHelper->setSelected(state);
    if(state)
        emit selected();
    else
        emit unselected();
}


TwoStateButton::TwoStateButton(QWidget* parent  , CustomButtonStyleHelper* helper)
    :CustomButton(parent)
{
    if(helper)
        setStyleHelper(helper);

}
bool TwoStateButton::isActive() { return pStyleHelper->selected();}
void TwoStateButton::setActive(bool state) { pStyleHelper->setSelected(state);}


