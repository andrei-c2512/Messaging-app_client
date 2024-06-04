
#ifndef LOGINPAGE_H
#define LOGINPAGE_H
#include <QWidget>
#include "CustomStyledWidgets/SelectableButton.h"
#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include "CustomWidgets/SignInForm.h"
#include "Page.h"


class LoginPage : public Page
{
    Q_OBJECT
public:
    LoginPage(QWidget* parent , ConnectionHandler& ServerInfoProcessor);
    const CustomButton* registerButton() const;
    const EvaluateButton* loginButton() const;
private slots:
    void testLogin();
private:
    void setupUi();
private:
    QHBoxLayout* pRatioLayout;
    QVBoxLayout*  pMainLayout;
    LoginForm*    pForm;
    QLabel*       pTitle;
    EvaluateButton* pLoginBtn;

    QHBoxLayout*  pRegisterOptionLayout;
    QLabel*       pRegisterLabel;
    CustomButton* pRegisterButton;
};

#endif // LOGINPAGE_H
