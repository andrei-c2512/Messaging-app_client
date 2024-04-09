#include "SignInForm.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ButtonStyleRepository.h"

SignInForm::SignInForm(QWidget* widget)
    :QWidget(widget)
{}

void SignInForm::initializeWidgets(){}
void SignInForm::assembleLayout(){}
void SignInForm::setupUi(){
    initializeWidgets();
    assembleLayout();
}
CustomLineEdit&  SignInForm::tokenEdit() { return *pTokenEdit;}
CustomLineEdit&  SignInForm::passwordEdit() { return * pPasswordEdit;}

LoginForm::LoginForm(QWidget* widget)
    :SignInForm(widget)
{

}

void LoginForm::initializeWidgets()
{
    QFont standardFont = StyleRepository::Base::standardFont();
    pToken = new QLabel;
    pToken->setFont(standardFont);
    pToken->setText("Username:");
    pToken->setAlignment(Qt::AlignTop);

    pTokenEdit = new CustomLineEdit;
    pTokenEdit->lineEdit().setFont(standardFont);
    pTokenEdit->lineEdit().setPlaceholderText("Username");
    pTokenEdit->lineEdit();
    pTokenEdit->setWarningUse(true);
    pTokenEdit->setCharacterLimit(maxCharacters);

    pPassword = new QLabel;
    pPassword->setFont(standardFont);
    pPassword->setText("Password:");
    pPassword->setAlignment(Qt::AlignTop);

    pPasswordEdit = new CustomLineEdit;
    pPasswordEdit->setWarningUse(true);
    pPasswordEdit->setCharacterLimit(passwordMaxCharacters);

    pPasswordEdit->lineEdit().setFont(standardFont);
    pPasswordEdit->lineEdit().setPlaceholderText("Password...");
    pShow = new SelectableButton;
    pShow->setStyleHelper(ButtonStyleRepository::showPasswordButton());

    connect(pShow , &SelectableButton::selected , this , [this](){
        this->pPasswordEdit->lineEdit().setEchoMode(QLineEdit::Password);
    });
    connect(pShow , &SelectableButton::unselected , this , [this](){
        this->pPasswordEdit->lineEdit().setEchoMode(QLineEdit::Normal);
    });
    pShow->setState(true);
    pGridLayout = new QGridLayout(this);


}
void LoginForm::assembleLayout()
{
    pGridLayout->addWidget(pToken , 0 , 0);
    pGridLayout->addWidget(pTokenEdit , 0 , 1);

    pGridLayout->addWidget(pPassword , 1 , 0);
    pGridLayout->addWidget(pPasswordEdit , 1 , 1);
    pGridLayout->addWidget(pShow , 1 , 2);
}

RegisterForm::RegisterForm(QWidget* widget)
    :LoginForm(widget)
{}
void RegisterForm::initializeWidgets()
{
    LoginForm::initializeWidgets();
    QFont standardFont = StyleRepository::Base::standardFont();
    pEmailEdit = new CustomLineEdit;
    pEmailEdit->lineEdit().setFont(standardFont);
    pEmailEdit->lineEdit().setPlaceholderText("Email...");
    pEmailEdit->setWarningUse(true);
    pEmailEdit->setCharacterLimit(maxCharacters);

    pEmail = new QLabel;
    pEmail->setText("Email");
    pEmail->setFont(standardFont);
    pEmail->setAlignment(Qt::AlignTop);
}
void RegisterForm::assembleLayout()
{
    pGridLayout->addWidget(pToken , 0 , 0);
    pGridLayout->addWidget(pTokenEdit , 0 , 1);

    pGridLayout->addWidget(pEmail , 1 , 0);
    pGridLayout->addWidget(pEmailEdit , 1 , 1);

    pGridLayout->addWidget(pPassword , 2 , 0);
    pGridLayout->addWidget(pPasswordEdit , 2 , 1);
    pGridLayout->addWidget(pShow , 2 , 2);
}

CustomLineEdit& RegisterForm::emailEdit() { return *pEmailEdit;}
