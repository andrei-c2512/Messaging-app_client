
#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H
#include <QWidget>
#include "CustomWidgets/SignInForm.h"
#include "Page.h"

class RegisterPage : public Page{
    Q_OBJECT
public:
    RegisterPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor);
    const CustomButton* goBackBtn() const;
    const EvaluateButton* registerBtn() const;
private:
    void setupUi();
private:
    QHBoxLayout* pLayout;
    QVBoxLayout*  pWidgetLayout;
    RegisterForm*    pForm;
    QLabel*       pTitle;

    EvaluateButton* pRegisterBtn;
    CustomButton* pGoBackBtn;
};

#endif // REGISTERPAGE_H
