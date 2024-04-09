
#ifndef LOADINGANIMATIONWIDGET_H
#define LOADINGANIMATIONWIDGET_H
#include <QWidget>
#include <QPainter>
#include <QTimer>
#define  Pi 3.14159265358979323846  /* pi */

class LoadingAnimationWidget : public QWidget{
    Q_OBJECT
public:
    LoadingAnimationWidget(QWidget* parent = nullptr);
    void setCycleDuration(int msec);

    int cycleDuration() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void stop();
    void start();
protected:
    void paintEvent(QPaintEvent* ev) override;
private:
    unsigned char alphaForN(int n);
private slots:
    void updateCircles();
private:
    int _cycleDuration;
    int _currentStart;
    QColor _circleColor;
    QTimer* pTimer;
};

#endif // LOADINGANIMATIONWIDGET_H
