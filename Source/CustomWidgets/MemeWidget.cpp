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
    _paused = false;
    pMovie = new QMovie(":/Images/Images/Gifs/IhaveNoEnemies.gif");
    pMovieLabel = new QLabel(this);
    pMovieLabel->setMovie(pMovie);
    pMovie->start();

    pOutput = new QAudioOutput;
    pOutput->setVolume(0.0f);

    pPlayer = new QMediaPlayer(this);
    pPlayer->setAudioOutput(pOutput);
    pPlayer->setSource(QUrl::fromLocalFile("C:/Users/Const/Documents/Stana/Source/Music/Nujabes-Aruarian Dance.m4a"));
    pPlayer->stop();

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


    pOppacityAnimation = new QPropertyAnimation(this , "opacity" , this);
    pOppacityAnimation->setStartValue(0.0f);
    pOppacityAnimation->setEndValue(1.0f);
    pOppacityAnimation->setDuration(200);


    const int soundAnimDur = 4000; // 5 seconds
    pSoundFadeAnimation = new QPropertyAnimation(this , "volume" , this);
    pSoundFadeAnimation->setEasingCurve(QEasingCurve::Linear);
    pSoundFadeAnimation->setDuration(soundAnimDur);

    connect(pSoundFadeAnimation, &QPropertyAnimation::finished, this, [=]() {
        pPlayer->stop();
        });
    pSoundApproachAnimation = new QPropertyAnimation(this, "volume", this);
    pSoundApproachAnimation->setEasingCurve(QEasingCurve::Linear);
    pSoundApproachAnimation->setDuration(soundAnimDur);
}   


void MemeWidget::fadeMusic()
{
    if (pOutput->volume())
    {
        pSoundFadeAnimation->setStartValue(pOutput->volume());
        pSoundFadeAnimation->setEndValue(0.0f);
        pSoundFadeAnimation->start();
        pSoundApproachAnimation->stop();
    }

}
void MemeWidget::startMusic()
{
    if (pPlayer->isPlaying() == false)
        pPlayer->play();
    pSoundApproachAnimation->setStartValue(pOutput->volume());
    pSoundApproachAnimation->setEndValue(1.0f);
    pSoundApproachAnimation->start();
    pSoundFadeAnimation->stop();
}


bool MemeWidget::event(QEvent* e)
{
    auto type = e->type();
    if(type == QEvent::HoverEnter)
    {
        pOppacityAnimation->setDirection(QAbstractAnimation::Direction::Forward);
        pOppacityAnimation->start();
    }else if(type == QEvent::HoverLeave)
    {
        pOppacityAnimation->setDirection(QAbstractAnimation::Direction::Backward);
        pOppacityAnimation->start();
    }
    return QWidget::event(e);
}

QMediaPlayer& MemeWidget::player() noexcept { return *pPlayer;}
QMovie& MemeWidget::movie() noexcept { return *pMovie;}
QSize MemeWidget::sizeHint() const { return pMovieLabel->sizeHint();}
QSize MemeWidget::minimumSizeHint() const { return pMovieLabel->sizeHint();}
float  MemeWidget::opacity() const { return _opacity;}
float MemeWidget::volume() const { return pOutput->volume(); }

void  MemeWidget::setOpacity(float val) {
    _opacity = std::move(val);
    for(QGraphicsOpacityEffect* e : _effectList)
        e->setOpacity(_opacity);
}
void MemeWidget::setVolume(float vol)
{
    pOutput->setVolume(vol);
}

void MemeWidget::resizeEvent(QResizeEvent* e)
{
    pMovieLabel->setGeometry(QRect(QPoint() , e->size()));
}
