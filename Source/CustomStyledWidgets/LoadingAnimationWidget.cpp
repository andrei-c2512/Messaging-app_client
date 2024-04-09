#include "LoadingAnimationWidget.h"


static const float circleDegrees = 2 * Pi;
static const float smallCircleCnt = 12;
static const float add = circleDegrees / smallCircleCnt;


LoadingAnimationWidget::LoadingAnimationWidget(QWidget* parent)
    :QWidget(parent)
{
    _cycleDuration = 600;
    //auto policy = QSizePolicy(QSizePolicy::Preferred , QSizePolicy::Preferred);
    //policy.setHeightForWidth(true);
    //setSizePolicy(policy);
    _circleColor = "gray";
    _currentStart = 0;
    pTimer = new QTimer(this);
    pTimer->setInterval(_cycleDuration / smallCircleCnt);
    pTimer->start();

    connect(pTimer , &QTimer::timeout , this , &LoadingAnimationWidget::updateCircles);
}

void LoadingAnimationWidget::stop() { pTimer->stop();}
void LoadingAnimationWidget::start() { pTimer->start();}

void LoadingAnimationWidget::updateCircles()
{
    _currentStart++;
    _currentStart %= int(smallCircleCnt);
    update();
}
void LoadingAnimationWidget::setCycleDuration(int msec) {
    _cycleDuration = std::move(msec);
    pTimer->setInterval(_cycleDuration / smallCircleCnt);
}
QSize LoadingAnimationWidget::sizeHint() const { return QSize(120 , 120);}
QSize LoadingAnimationWidget::minimumSizeHint() const  { return QSize(120 , 120);}

int LoadingAnimationWidget::cycleDuration() const { return _cycleDuration;}
void LoadingAnimationWidget::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    const int innerCircleRadius = std::min(width() , height()) / 10;
    QPoint center = { width() / 2 , height() / 2};

    int circleCnt = _currentStart;
    for(float degree = 0.0f ; degree < circleDegrees  ; degree += add)
    {
        _circleColor.setAlpha(alphaForN(circleCnt));


        QPen pen(_circleColor);
        QBrush brush(_circleColor);
        painter.setPen(pen);
        painter.setBrush(brush);

        //so basically the small circle radius will be at a distance 4 times it's radius , from the center of drawing
        QPoint circleCenter = center + QPoint(std::cos(degree) * 4.0f * float(innerCircleRadius), std::sin(degree) * 4.0f * float(innerCircleRadius));
        const int radius = (innerCircleRadius / smallCircleCnt) * (smallCircleCnt - circleCnt);
        painter.drawEllipse(circleCenter , radius , radius );
        circleCnt++;
        if(circleCnt == int(smallCircleCnt))
            circleCnt = 0;
    }
}

unsigned char LoadingAnimationWidget::alphaForN(int n)
{
    n %= int(smallCircleCnt);
    n = smallCircleCnt - n;
    return 55 + 200 / 12.0f * n;
}
