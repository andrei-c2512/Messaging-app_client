#include "RegisterPage.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ButtonStyleRepository.h"
RegisterPage::RegisterPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor)
    :Page(parent , ServerInfoProcessor)
{
    setupUi();
    setObjectName("SignInPage");
}

void RegisterPage::setupUi()
{
    QFont headerFont = StyleRepository::Base::headerFont();
    pForm = new RegisterForm;
    pForm->setupUi();
    pTitle = new QLabel;
    pTitle->setText("Register");
    pTitle->setAlignment(Qt::AlignCenter);
    pTitle->setFont(std::move(headerFont));

    pRegisterBtn = new EvaluateButton(nullptr , ButtonStyleRepository::loginButton());
    pRegisterBtn->setText("Register");
    pRegisterBtn->setFont(StyleRepository::Base::standardFont());
    connect(pRegisterBtn , &EvaluateButton::clicked , this , [this](){
        auto& obj = *this;
        if(obj.pForm->tokenEdit().lineEdit().text().isEmpty())
        {
            obj.pForm->tokenEdit().warningLabel().setText("Requiered field");
            return;
        }
        if(obj.pForm->emailEdit().lineEdit().text().isEmpty())
        {
            obj.pForm->emailEdit().warningLabel().setText("Requiered field");
            return;
        }
        else if(obj.pForm->emailEdit().lineEdit().text().indexOf('@') == -1)
        {
            obj.pForm->emailEdit().warningLabel().setText("Invalid email");
            return;
        }
        if(obj.pForm->passwordEdit().lineEdit().text().length() < SignInForm::minimumPasswordLength)
        {
            obj.pForm->passwordEdit().warningLabel().setText("Password should be at least " + QString::number(SignInForm::minimumPasswordLength));
            return;
        }

        {
            obj.serverInfoProcessor.setName(pForm->tokenEdit().lineEdit().text());
            obj.serverInfoProcessor.setEmail(pForm->emailEdit().lineEdit().text());
            obj.serverInfoProcessor.setPassword(pForm->passwordEdit().lineEdit().text());
            obj.serverInfoProcessor.registerIntoDatabase();
        }
    });
    pGoBackBtn = new CustomButton(nullptr , ButtonStyleRepository::goBackColor_LeftArrow());

    pWidgetLayout = new QVBoxLayout;
    pWidgetLayout->addStretch(1);
    pWidgetLayout->addWidget(pTitle);
    pWidgetLayout->addWidget(pForm);
    pWidgetLayout->addWidget(pRegisterBtn);
    pWidgetLayout->addStretch(1);


    connect(&serverInfoProcessor , &ServerInfoProcessor::emailUsedError , this , [this](){
        this->pForm->emailEdit().warningLabel().setText("<b>Email</b> already in use");
    });
    connect(&serverInfoProcessor , &ServerInfoProcessor::nameUsedError , this , [this](){
        this->pForm->tokenEdit().warningLabel().setText("<b>Name</b> already in use");
    });
    pLayout = new QHBoxLayout(this);
    pLayout->addStretch(3);
    pLayout->addWidget(pGoBackBtn , 1);
    pLayout->addLayout(pWidgetLayout , 4);
    pLayout->addStretch(4);
}
const CustomButton* RegisterPage::goBackBtn() const{ return pGoBackBtn; }
const EvaluateButton* RegisterPage::registerBtn() const { return pRegisterBtn;}
