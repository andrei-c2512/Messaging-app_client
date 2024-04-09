
#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H
#include <QPushButton>
#include "StyleBase/ButtonStyles/CustomButtonStyleHelper.h"
#include "StyleBase/AnimationTool.h"

using AnimationToolKit = AnimationTool;

class CustomButton : public QPushButton{
    Q_OBJECT
public:
    CustomButton(QWidget* parent = nullptr);
    CustomButton(QWidget* parent , CustomButtonStyleHelper* styleHelper);
    const CustomButtonStyleHelper* styleHelper() const;
    void setStyleHelper(CustomButtonStyleHelper* styleHelper);
    void setTextVisibility(bool visible);
protected:
    CustomButtonStyleHelper* pStyleHelper;
    QString _text;
};

//evaluates a function on mouse press
class EvaluateButton : public CustomButton{
    Q_OBJECT
public:
    EvaluateButton(QWidget* parent = nullptr);
    EvaluateButton(QWidget* parent , CustomButtonStyleHelper* styleHelper);
    void setEvaluation(std::function<bool()> func);
signals:
    void approved();
    void rejected();
protected:
    void mousePressEvent(QMouseEvent* ev ) override;
protected:
    std::function<bool()> evaluation;
};




#endif // CUSTOMBUTTON_H
