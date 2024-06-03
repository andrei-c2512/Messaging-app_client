#include "LoginPage.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ButtonStyleRepository.h"

LoginPage::LoginPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor)
    :Page(parent , ServerInfoProcessor)
{
    setupUi();
    connect(pLoginBtn , &CustomButton::clicked , this , &LoginPage::testLogin);
    setObjectName("SignInPage");
}
void LoginPage::testLogin()
{

}

void LoginPage::setupUi()
{
    QFont headerFont = StyleRepository::Base::headerFont();
    pForm = new LoginForm;
    pForm->setupUi();

    pTitle = new QLabel;
    pTitle->setText("Login");
    pTitle->setAlignment(Qt::AlignHCenter);
    pTitle->setFont(std::move(headerFont));

    pLoginBtn = new EvaluateButton(nullptr , ButtonStyleRepository::loginButton());
    pLoginBtn->setText("Login");
    pLoginBtn->setFont(StyleRepository::Base::standardFont());
    connect(pLoginBtn , &EvaluateButton::clicked , this , [this](){
        auto& obj = *this;
        if(obj.pForm->tokenEdit().lineEdit().text().isEmpty())
        {
            obj.pForm->tokenEdit().warningLabel().setText("Requiered field");
            return;
        }
        if(obj.pForm->passwordEdit().lineEdit().text().length() < SignInForm::minimumPasswordLength)
        {
            obj.pForm->passwordEdit().warningLabel().setText("Password should be at least " + QString::number(SignInForm::minimumPasswordLength));
            return;
        }

        {
            QString name = pForm->tokenEdit().lineEdit().text();
            QString password = pForm->passwordEdit().lineEdit().text();
            obj.serverInfoProcessor.storage().setName(name);
            obj.serverInfoProcessor.storage().setPassword(password);
            obj.serverInfoProcessor.requestSender().logIntoDatabase(name , password);
        }
    });

    pRegisterLabel = new QLabel;
    pRegisterLabel->setFont(StyleRepository::Base::informativeTextFont());
    pRegisterLabel->setText("Don't have an account? Click ");

    pRegisterButton = new CustomButton(nullptr , ButtonStyleRepository::onlyTextButton());
    pRegisterButton->setFont(StyleRepository::Base::standardFont());
    pRegisterButton->setText("HERE");
    pRegisterOptionLayout = new QHBoxLayout;
    pRegisterOptionLayout->addStretch(1);
    pRegisterOptionLayout->addWidget(pRegisterLabel);
    pRegisterOptionLayout->addWidget(pRegisterButton);


    connect(&serverInfoProcessor.handler(), &ResponseHandler::incorrectPasswordError, this, [this]() {
        this->pForm->passwordEdit().warningLabel().setText("Incorrect password");
    });
    connect(&serverInfoProcessor.handler(), &ResponseHandler::nameNotFoundError, this, [this]() {
        this->pForm->tokenEdit().warningLabel().setText("Name not found");
    });
    pMainLayout = new QVBoxLayout();
    pMainLayout->addStretch(1);
    pMainLayout->addWidget(pTitle);
    pMainLayout->addWidget(pForm);
    pMainLayout->addLayout(pRegisterOptionLayout);
    pMainLayout->addWidget(pLoginBtn);
    pMainLayout->addStretch(1);

    pRatioLayout = new QHBoxLayout(this);
    pRatioLayout->addStretch(1);
    pRatioLayout->addLayout(pMainLayout , 1);
    pRatioLayout->addStretch(1);
}
const CustomButton* LoginPage::registerButton() const{ return pRegisterButton; }
const EvaluateButton* LoginPage::loginButton() const { return pLoginBtn; }
