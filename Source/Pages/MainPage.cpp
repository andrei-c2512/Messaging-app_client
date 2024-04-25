#include "MainPage.h"



MainPage::MainPage(QWidget* parent , ServerInfoProcessor& processor  , UserSelectorWidget& userSelectorWidget)
    :Page(parent , processor)
{
    setupUi(userSelectorWidget);
    setObjectName("MainPage");
}

void MainPage::setupUi(UserSelectorWidget& userSelectorWidget)
{
    QMargins noMargins = QMargins(0 , 0 , 0 , 0);
    pManager = new SubpageManager(nullptr , serverInfoProcessor , userSelectorWidget);
    pManager->setContentsMargins(noMargins);
    pFrame = new OptionSectionFrame(nullptr , serverInfoProcessor , *pManager , userSelectorWidget);
    pFrame->setContentsMargins(noMargins);


    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pLayout->addWidget(pFrame , 1);
    pLayout->addWidget(pManager, 6);

}
