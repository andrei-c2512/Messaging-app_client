#include "MemeWidget.h"
#include "StyleBase/ButtonStyleRepository.h"
MemeWidget::MemeWidget(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_Hover);
    setSizePolicy(QSizePolicy(QSizePolicy::Policy::Minimum ,QSizePolicy::Policy::Minimum ));
    setupUi();
}

void MemeWidget::setupUi()
{
    pMovie = new QMovie(":/Special/IhaveNoEnemies.gif");
    pMovieLabel = new QLabel(this);
    pMovieLabel->setMovie(pMovie);
    pMovie->start();

    pOutput = new QAudioOutput;
    pOutput->setVolume(1.0f);

    pPlayer = new QMediaPlayer(this);
    pPlayer->setAudioOutput(pOutput);
    pPlayer->setSource(QUrl::fromLocalFile("C:/Users/Const/Documents/DiscordApp/Code/Nujabes - Aruarian Dance.mp4"));

    _effectList = { new QGraphicsOpacityEffect(this) , new QGraphicsOpacityEffect(this) , new QGraphicsOpacityEffect(this)};
    pSoundButton = new SelectableButton(nullptr , ButtonStyleRepository::muteButton());

    connect(pSoundButton , &SelectableButton::clicked , this , [=](){
        int currentVal = std::ceil(pOutput->volume());
        if(currentVal)
        {
            pVolumeControl->setValue(0);
        }
        else
        {
            pVolumeControl->setValue(100);
        }
    });

    pPauseButton = new SelectableButton(nullptr , ButtonStyleRepository::pauseButton());
    connect(pPauseButton , &SelectableButton::clicked , this ,[=](){
        qDebug() << "clicked";
        _paused = !_paused;
        if(_paused)
        {
            pPlayer->pause();
            pMovie->setPaused(true);
        }
        else
        {
            pPlayer->play();
            pMovie->setPaused(false);
        }
    });

    //for some reason the slider does not visually update. How frustrating
    pVolumeControl = new QSlider;
    pVolumeControl->setOrientation(Qt::Vertical);
    pVolumeControl->setMinimum(0);
    pVolumeControl->setMaximum(3);
    pVolumeControl->setValue(70);
    pVolumeControl->setVisible(false);
    connect(pVolumeControl , &QSlider::valueChanged , this , [=](int value){
        pOutput->setVolume(float(value) / float(pVolumeControl->maximum()));
    });

    //setting opacity for all widgets
    {
        pSoundButton->setGraphicsEffect  (_effectList[0]);
        pPauseButton->setGraphicsEffect  (_effectList[1]);
        pVolumeControl->setGraphicsEffect(_effectList[2]);
    }

    pMovieLabel->setGeometry(rect());
    pWidgetLayout = new QVBoxLayout;
    pWidgetLayout->addStretch(1);
    pWidgetLayout->addWidget(pVolumeControl , Qt::AlignHCenter);
    pWidgetLayout->addWidget(pSoundButton);

    pPlacementLayout = new QHBoxLayout(this);
    pPlacementLayout->addStretch(2);
    pPlacementLayout->addWidget(pPauseButton , 1, Qt::AlignBottom);
    pPlacementLayout->addStretch(1);
    pPlacementLayout->addLayout(pWidgetLayout , 1 );


    pAnimation = new QPropertyAnimation(this , "opacity" , this);
    pAnimation->setStartValue(0.0f);
    pAnimation->setEndValue(1.0f);
    pAnimation->setDuration(200);


}

bool MemeWidget::event(QEvent* e)
{
    auto type = e->type();
    if(type == QEvent::HoverEnter)
    {
        pAnimation->setDirection(QAbstractAnimation::Direction::Forward);
        pAnimation->start();
    }else if(type == QEvent::HoverLeave)
    {
        pAnimation->setDirection(QAbstractAnimation::Direction::Backward);
        pAnimation->start();
    }
    return QWidget::event(e);
}

QMediaPlayer& MemeWidget::player() noexcept { return *pPlayer;}
QMovie& MemeWidget::movie() noexcept { return *pMovie;}
QSize MemeWidget::sizeHint() const { return pMovieLabel->sizeHint();}
QSize MemeWidget::minimumSizeHint() const { return pMovieLabel->sizeHint();}
float  MemeWidget::opacity() const { return _opacity;}
void  MemeWidget::setOpacity(float val) {
    _opacity = std::move(val);
    for(QGraphicsOpacityEffect* e : _effectList)
        e->setOpacity(_opacity);
}


void MemeWidget::resizeEvent(QResizeEvent* e)
{
    pMovieLabel->setGeometry(QRect(QPoint() , e->size()));
}
