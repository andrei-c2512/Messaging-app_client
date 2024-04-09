
#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H
#include <QPixmap>
#include <QObject>
#include <QIcon>

class ImagePainter : public QObject{
    Q_OBJECT
public:
    ImagePainter(QObject* parent , QPixmap pixmap = QPixmap());
    QColor color() const noexcept;
    QImage image() const noexcept;
    QIcon* icon() const noexcept;
    QPixmap pixmap() const noexcept;

    void setPixmap(QPixmap pixmap) noexcept;
    void setPixmap(QPixmap pixmap , QColor color) noexcept;
    static QImage paintImage(QPixmap pixmap, QColor color);
public slots:
    void setColor(QColor color) noexcept;
private:
    QPixmap _original;
    QImage _image;
    std::vector<QPoint> posList;

    QColor _color;
};

#endif // IMAGEPAINTER_H
