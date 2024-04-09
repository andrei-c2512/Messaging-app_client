
#ifndef ANIMATIONTOOL_H
#define ANIMATIONTOOL_H
#include <QVariantAnimation>
#include <QTimer>
#include "StyleHelper.h"
#include "ImagePainter.h"


class AnimationManager : public QObject{
    Q_OBJECT
public:
    AnimationManager(QObject* parent);
    QVariantAnimation& animation();
    const QVariantAnimation* panimation();
    void setEnabled(bool enabled);
    bool isEnabled() const;
signals:
    void animationEnabled();
protected:
    bool _enabled;
    QVariantAnimation* pAnimation = nullptr;
};

class IconColorAnimationManager : public AnimationManager{
public:
    IconColorAnimationManager(QObject* parent);
    ImagePainter& painter();
private:
    ImagePainter* pPainter;
};

//for color shift animating
/* As implementa un sistem in care precizezi si start triggerul/ stop triggerul
 * Insa fiindca toate start/stop triggerurile mele sunt hoverEnter/hoverLeave
 *  nu prea e nevoie , momentan*/

class AnimationTool : public QObject , public StyleHelper{
    Q_OBJECT;
public:
    enum class Type{
        BorderColor = 0,
        BackgroundColor = 1,
        TextColor = 2,
        IconColor = 3,
        IconSize = 4 ,
        Margins = 5 ,
        LeftMargin = 6 ,
        TopMargin = 7 ,
        RightMargin = 8,
        BottomMargin = 9,
        Count
    };
public:
    AnimationTool(QWidget* widget = nullptr);
    virtual ~AnimationTool() = default;
    AnimationManager& tool(Type type);
    const AnimationManager& tool(Type type) const;
    void createTool(Type type , QVariant startVal , QVariant endVal , int duration);
    void createIconColorTool(QVariant startVal , QVariant endVal , int duration , QPixmap pixmap);
protected slots:
    void connectAnimation();
    void cntUpdater(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void scheduleUpdate();
    void setDirectionForActives(QAbstractAnimation::Direction dir) const;
protected:
    std::vector<AnimationManager*> animationList;
    mutable std::vector<AnimationManager*> activeAnimList;
    bool    updateScheduled = false;
    QTimer* updateTimer;
    int animationCounter;
};

#endif // ANIMATIONTOOL_H
