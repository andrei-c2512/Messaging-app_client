#include "MainPage.h"



MainPage::MainPage(QWidget* parent , ServerInfoProcessor& processor)
    :Page(parent , processor)
{
    setupUi();
    setObjectName("MainPage");
}

void MainPage::setupUi()
{
    QMargins noMargins = QMargins(0 , 0 , 0 , 0);
    pManager = new SubpageManager(nullptr , serverInfoProcessor);
    pManager->setContentsMargins(noMargins);
    pFrame = new OptionSectionFrame(nullptr , serverInfoProcessor , *pManager);
    pFrame->setContentsMargins(noMargins);


    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pLayout->addWidget(pFrame , 1);
    pLayout->addWidget(pManager, 6);

}
