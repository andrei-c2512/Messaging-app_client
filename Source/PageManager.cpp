#include "PageManager.h"

PageManager::PageManager(QWidget* parent , ServerInfoProcessor& serverInfoProcessor0 , UserSelectorWidget& userSelectorWidget)
    :QStackedWidget(parent) , serverInfoProcessor(serverInfoProcessor0)
{
    setupUi(userSelectorWidget);
    setPage(PageType::Loading);
}

void PageManager::setupUi(UserSelectorWidget& userSelectorWidget)
{
    //creating the pages

    pageList = { new LoadingPage(nullptr , serverInfoProcessor) , new LoginPage(nullptr , serverInfoProcessor) , new RegisterPage(nullptr , serverInfoProcessor) ,
                new MainPage(nullptr , serverInfoProcessor , userSelectorWidget)};
    for(int i = 0 ; i < pageList.size() ; i++)
    {
        connectButtons(PageType(i) , pageList[i]);
        addWidget(pageList[i]);
    }
}


void PageManager::connectButtons(PageManager::PageType type , const Page* page)
{
    switch(type)
    {
    case PageType::Loading:
        if(const LoadingPage* p = qobject_cast<const LoadingPage*>(page))
        {
            connect(&serverInfoProcessor.socket(), &QTcpSocket::connected , this , [this](){
                this->setPage(PageType::Login);
                emit this->pageChanged(PageType::Login);
            });
            connect(&serverInfoProcessor.socket() , &QTcpSocket::disconnected , this , [this](){
                this->setPage(PageType::Loading);
                emit this->pageChanged(PageType::Loading);
            });
        }
        return;
    case PageType::Login:
        if(const LoginPage* p = qobject_cast<const LoginPage*>(page))
        {
            connect(p->registerButton() , &CustomButton::clicked , this , [this](){
                this->setPage(PageType::Register);
                emit this->pageChanged(PageType::Register);
            });
            connect(&serverInfoProcessor , &ServerInfoProcessor::signInAccepted , this , [this](){
                this->setPage(PageType::Main);
                emit this->pageChanged(PageType::Main);
            });
        }
        return;
    case PageType::Register:
        if(const RegisterPage* p = qobject_cast<const RegisterPage*>(page))
        {
            connect(p->goBackBtn() , &CustomButton::clicked , this , [this](){
                this->setPage(PageType::Login);
                emit this->pageChanged(PageType::Login);
            });
            connect(&serverInfoProcessor , &ServerInfoProcessor::signInAccepted , this , [this](){
                this->setPage(PageType::Main);
                emit this->pageChanged(PageType::Main);
            });
        }
        return;
    case PageType::Main:
        if(const MainPage* p = qobject_cast<const MainPage*>(page))
        {

        }
        return;
        return;
    default:
        return;
    }
}


void PageManager::setPage(PageManager::PageType page)
{
    QStackedWidget::setCurrentIndex(int(page));
}
