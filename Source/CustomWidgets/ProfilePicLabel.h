
#ifndef PROFILEPICLABEL_H
#define PROFILEPICLABEL_H
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

class ProfilePicLabel : public QLabel{
public:
    using QLabel::QLabel;
    void setPixmap1(QPixmap pixmap);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
private:
    using QLabel::setPixmap;
protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // PROFILEPICLABEL_H
