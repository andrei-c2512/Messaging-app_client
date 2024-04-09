
#ifndef MEMEWIDGET_H
#define MEMEWIDGET_H
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QMovie>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QStyle>
#include <QAudioOutput>
#include <QSizePolicy>
#include "StyleBase/ImagePainter.h"
#include "CustomStyledWidgets/SelectableButton.h"
#include <QResizeEvent>

class MemeWidget : public QWidget{
    Q_OBJECT
    Q_PROPERTY(float opacity READ opacity WRITE setOpacity)
public:
    MemeWidget(QWidget* parent = nullptr);
    QMediaPlayer& player() noexcept;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    QMovie& movie() noexcept;
    bool event(QEvent* event) override;
    float opacity() const;
    void setOpacity(float val);
protected:
    void resizeEvent(QResizeEvent* e) override;
private:
    void setupUi();
private:
    QLabel* pMovieLabel;
    QHBoxLayout*  pPlacementLayout;
    QVBoxLayout*  pWidgetLayout;
    SelectableButton* pSoundButton;
    SelectableButton* pPauseButton;
    QSlider*      pVolumeControl;

    std::vector<QGraphicsOpacityEffect*> _effectList;
    QPropertyAnimation* pAnimation;
    QMovie* pMovie;
    bool _paused;

    QMediaPlayer* pPlayer;
    QAudioOutput* pOutput;

    int lastValue = 0;
    float _opacity;
};

#endif // MEMEWIDGET_H
