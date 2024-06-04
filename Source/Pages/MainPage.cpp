#include "MainPage.h"



MainPage::MainPage(QWidget* parent , ConnectionHandler& processor  , UserSelectorWidget& userSelectorWidget , KeywordCombo& keywordCombo)
    :Page(parent , processor)
{
    setupUi(userSelectorWidget , keywordCombo);
    setObjectName("MainPage");
}

void MainPage::setupUi(UserSelectorWidget& userSelectorWidget, KeywordCombo& keywordCombo)
{
    QMargins noMargins = QMargins(0 , 0 , 0 , 0);
    pManager = new SubpageManager(nullptr , serverInfoProcessor , userSelectorWidget, keywordCombo);
    pManager->setContentsMargins(noMargins);
    pFrame = new OptionSectionFrame(nullptr , serverInfoProcessor , *pManager , userSelectorWidget);
    pFrame->setContentsMargins(noMargins);


    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pLayout->addWidget(pFrame , 1);
    pLayout->addWidget(pManager, 6);

}
