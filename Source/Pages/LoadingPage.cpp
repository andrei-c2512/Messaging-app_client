#include "LoadingPage.h"
#include "StyleBase/StyleRepository.h"
LoadingPage::LoadingPage(QWidget* parent , ConnectionHandler& pProcessor)
    :Page(parent , pProcessor)
{
    setupUi();
    pLabelUpdateTimer = new QTimer(this);
    pLabelUpdateTimer->setInterval(1000);
    pLabelUpdateTimer->start();
    _currentDotCount = 0;
    connect(pLabelUpdateTimer , &QTimer::timeout , this , [this](){
        this->_currentDotCount++;
        if(this->_currentDotCount == 4)
            this->_currentDotCount = 1;

        QString str = "Trying to connect to server";
        for(int i = 0 ; i < this->_currentDotCount ; i++)
            str.append('.');
        this->pLoadingMessage->setText(std::move(str));
    });
}

void LoadingPage::setupUi()
{
    pLayout = new QVBoxLayout(this);
    pLoadingAnim = new LoadingAnimationWidget;
    pLoadingAnim->setMaximumHeight(120);
    pLoadingMessage = new QLabel;
    pLoadingMessage->setFont(StyleRepository::Base::standardFont());
    pLoadingMessage->setText("Trying to connect to server...");
    pLoadingMessage->setAlignment(Qt::AlignCenter);

    pLayout->addStretch(1);
    pLayout->addWidget(pLoadingAnim , 2);
    pLayout->addWidget(pLoadingMessage);
    pLayout->addStretch(1);
}
