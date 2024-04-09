
#ifndef BACKGROUNDSHIFTONMOUSEEFFECT_H
#define BACKGROUNDSHIFTONMOUSEEFFECT_H
#include <QPixmap>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include "PageManager.h"

class PixmapPortion {
public:
    PixmapPortion(QPixmap* pixmap , QRect portion);
    void setPortion(QRect rect);
    bool customPortion() const;
    const QRect& portion() const;

    QPixmap* pPixmap = nullptr;
    QPixmap pixmapPortion() const;
    void reset();
private:
    bool _customPortion;
    QRect _portion;

    QRect _standardPortion;
};

class BackgroundShiftOnMouseEffect : public QWidget{
    Q_OBJECT
public:
    BackgroundShiftOnMouseEffect(QWidget* parent , QPixmap background , float scale = 2);
    void setScale(int scale) noexcept;
    void setBackground(QPixmap pixmap) noexcept;

    int scale() const;
    QPixmap background() const;
protected:
    void paintEvent(QPaintEvent* ev) noexcept override ;
    void resizeEvent(QResizeEvent* ev) override;
private:
    void scaleBackground();
    void calculateVelocity(QPoint mousePos) noexcept;
    void updateRectPos() noexcept;
    QPointF posToBackgroundPos(const QPointF& pos) const noexcept;
public slots:
    void updateBackground() noexcept;
    void updateAccordingToPage(PageManager::PageType type);
private:
    QRect _scaledBackgroundRect;
    QPixmap _originalBackground;


    std::vector<QPixmap> _backgroundMatrix;
    std::vector<PixmapPortion> _drawingMatrix;

    int _bgWidth;
    int _bgHeight;
    QRectF _current;

    int _scale;
    QPoint oldMousePos;
    QTimer* redrawTimer;
    QTimer* measureTimer;
    QPointF velocity;

    //the time from the current pos in the pixmap to the desired position
    const float moveInterval = 1000.0f;
    const float speed = 1.0f;
    const float decreaseFactor = 0.99f;

    int redrawsPerSec = 0;

    QSize chunkSize = {40 , 40};
};

#endif // BACKGROUNDSHIFTONMOUSEEFFECT_H
