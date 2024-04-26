
#ifndef PROFILEPICLABEL_H
#define PROFILEPICLABEL_H
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

class ProfilePicLabel : public QLabel{
public:
    enum class Status {
        Offline,
        Online
    };
public:
    using QLabel::QLabel;
    void setPixmap1(QPixmap pixmap);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void setStatusEnabled(bool enabled);
    void setStatus(Status status);
private:
    using QLabel::setPixmap;
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    bool _statusEnabled = false;
    Status status = Status::Offline;
};

#endif // PROFILEPICLABEL_H
