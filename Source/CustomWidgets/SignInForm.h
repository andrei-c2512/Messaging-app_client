
#ifndef SIGNINFORM_H
#define SIGNINFORM_H
#include <QWidget>
#include "CustomStyledWidgets/SelectableButton.h"
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include "CustomLineEdit.h"

//for this kind of class YOU have to call setupUi , otherwise it won't work
class SignInForm : public QWidget{
public:
    SignInForm(QWidget* widget = nullptr);
    void setupUi();
    CustomLineEdit& tokenEdit();
    CustomLineEdit& passwordEdit();
    static constexpr unsigned maxCharacters = 50;
    static constexpr unsigned passwordMaxCharacters = 12;
    static constexpr int minimumPasswordLength = 8;
protected:
    //for allocating , setting the default font , text , etc
    virtual void initializeWidgets();
    virtual void assembleLayout();
protected:
    QGridLayout*      pGridLayout;
    QLabel*           pToken;
    CustomLineEdit*   pTokenEdit;

    QLabel*           pPassword;
    CustomLineEdit*   pPasswordEdit;
    SelectableButton* pShow;
};

class LoginForm : public SignInForm{
public:
    LoginForm(QWidget* widget = nullptr);
protected:
    void initializeWidgets() override;
    void assembleLayout() override;
};

class RegisterForm : public LoginForm{
public:
    RegisterForm(QWidget* widget = nullptr);
    CustomLineEdit& emailEdit();
protected:
    void initializeWidgets() override;
    void assembleLayout() override;
protected:
    CustomLineEdit* pEmailEdit;
    QLabel*         pEmail;
};


#endif // SIGNINFORM_H
