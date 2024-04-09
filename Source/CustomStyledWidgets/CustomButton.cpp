#include "CustomButton.h"

CustomButton::CustomButton(QWidget* parent)
    :QPushButton(parent)
{
    pStyleHelper = new CustomButtonStyleHelper(this);
}
CustomButton::CustomButton(QWidget* parent , CustomButtonStyleHelper* styleHelper)
    :QPushButton(parent)
{
    pStyleHelper = styleHelper;
    styleHelper->setParent(this);
    pStyleHelper->setManagingWidget(this);
}
void CustomButton::setTextVisibility(bool visible)
{
    pStyleHelper->setTextVisible(visible);
}

const CustomButtonStyleHelper* CustomButton::styleHelper() const { return pStyleHelper; }
void CustomButton::setStyleHelper(CustomButtonStyleHelper* styleHelper)
{
    if(pStyleHelper)
        delete pStyleHelper;
    pStyleHelper = styleHelper;
    styleHelper->setParent(this);
    pStyleHelper->setManagingWidget(this);
}

EvaluateButton::EvaluateButton(QWidget* parent)
    :CustomButton(parent)
{
    evaluation = [](){return true;};
}
EvaluateButton::EvaluateButton(QWidget* parent , CustomButtonStyleHelper* styleHelper)
    :CustomButton(parent , styleHelper)
{
    evaluation = [](){return true;};
}
void EvaluateButton::setEvaluation(std::function<bool()> func){ evaluation = std::move(func);}

void EvaluateButton::mousePressEvent(QMouseEvent* ev )
{
    if(evaluation())
        emit approved();
    else
        emit rejected();
    CustomButton::mousePressEvent(ev);
}
