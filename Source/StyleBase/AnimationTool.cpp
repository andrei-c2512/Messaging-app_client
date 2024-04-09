#include "AnimationTool.h"

AnimationManager::AnimationManager(QObject* parent)
    :QObject(parent)
{}
QVariantAnimation& AnimationManager::animation() { return *pAnimation;}
bool AnimationManager::isEnabled() const{ return _enabled;}
const QVariantAnimation* AnimationManager::panimation() { return pAnimation;}
void AnimationManager::setEnabled(bool enabled)
{
    _enabled = std::move(enabled);
    if(_enabled)
    {
        pAnimation = new QVariantAnimation(this);
        emit animationEnabled();
    }
    else
        if(pAnimation) pAnimation->deleteLater();
}
IconColorAnimationManager::IconColorAnimationManager(QObject* parent)
    :AnimationManager(parent) , pPainter(new ImagePainter(this , QPixmap()))
{}
ImagePainter& IconColorAnimationManager::painter() { return *pPainter;}


void AnimationTool::scheduleUpdate()
{
    pManagingWidget->update();
}
AnimationTool::AnimationTool( QWidget* widget)
    :StyleHelper(widget)
{
    animationCounter = 0u;
    animationList.resize(int(Type::Count));
    for(auto& manager : animationList)
    {
        manager = new AnimationManager(this);
        connect(manager , SIGNAL(animationEnabled()) , this , SLOT(connectAnimation()));
    }
    auto* iconColorAnim = animationList[int(Type::IconColor)];
    iconColorAnim->deleteLater();
    iconColorAnim = new IconColorAnimationManager(this);
    animationList[int(Type::IconColor)] = iconColorAnim;
    connect(iconColorAnim , SIGNAL(animationEnabled()) , this , SLOT(connectAnimation()));

    updateScheduled = false;
    updateTimer = new QTimer;
    updateTimer->setInterval(1000/60);
    connect(updateTimer , SIGNAL(timeout()) , this , SLOT(scheduleUpdate()));
}
AnimationManager& AnimationTool::tool(Type type) { return *animationList[int(type)];}
const AnimationManager&  AnimationTool::tool(Type type) const { return *animationList[int(type)];}
void AnimationTool::cntUpdater(QAbstractAnimation::State state , QAbstractAnimation::State oldState)
{
    Q_UNUSED(oldState);
    //Running state = 2
    if(!state)
    {
        animationCounter--;
        if(!animationCounter)
            updateTimer->stop();
    }
    else
    {
        if(!animationCounter)
            updateTimer->start();
        animationCounter++;
    }
    //qDebug() << animationCounter;
}
void AnimationTool::connectAnimation()
{
    QObject* sender = QObject::sender();
    if(AnimationManager* pManager = qobject_cast<AnimationManager*>(sender))
    {
        activeAnimList.emplace_back(pManager);
        connect(pManager->panimation() , SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)) ,
                this , SLOT(cntUpdater(QAbstractAnimation::State,QAbstractAnimation::State)));
    }
}
void AnimationTool::createTool(Type type , QVariant startVal , QVariant endVal , int duration)
{
    assert(type != Type::IconColor && "Icon Color tool utilises a different tool maker");

    AnimationManager& manager = *animationList[int(type)];
    manager.setEnabled(true);
    QVariantAnimation& backgroundAnim = manager.animation();
    backgroundAnim.setStartValue(std::move(startVal));
    backgroundAnim.setEndValue(std::move(endVal));
    backgroundAnim.setDuration(std::move(duration));
}

void AnimationTool::createIconColorTool(QVariant startVal , QVariant endVal , int duration , QPixmap pixmap)
{
    QColor startColor = startVal.value<QColor>();
    IconColorAnimationManager& manager = *static_cast<IconColorAnimationManager*>(animationList[int(Type::IconColor)]);

    manager.setEnabled(true);
    QVariantAnimation& backgroundAnim = manager.animation();
    backgroundAnim.setStartValue(startColor);
    backgroundAnim.setEndValue(std::move(endVal));
    backgroundAnim.setDuration(std::move(duration));

    ImagePainter& painter = manager.painter();
    painter.setColor(startColor);
    painter.setPixmap(pixmap);
}
void AnimationTool::setDirectionForActives(QAbstractAnimation::Direction dir) const
{
    for(auto* manager : activeAnimList)
    {
        QVariantAnimation& anim = manager->animation();
        anim.setDirection(dir);
        anim.start();
    }
}
