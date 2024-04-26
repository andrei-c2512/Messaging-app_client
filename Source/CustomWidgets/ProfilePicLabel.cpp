#include "ProfilePicLabel.h"

void ProfilePicLabel::paintEvent(QPaintEvent* event)
{
    //const int dim = std::min(width() , height());
    //const int x = (width() - dim) / 2;
    //const int y = (height() - dim) / 2;

    QPainter painter(this);
    QPainterPath path;
    path.addEllipse(rect());
    //painter.setClipPath(std::move(path));

    int min = std::min(width() , height());
    painter.drawPixmap(QRect(QPoint((width() - min) / 2 , (height() - min) / 2 ) , QSize(min , min)) , pixmap());

    if (_statusEnabled)
    {
        int dim = float(min) / 2.5f;
        QRect statusRect = QRect(width() / 2 + min / 2 - dim, height() / 2 + min / 2 - dim, dim, dim);
        //QRect statusRect = QRect(width() / 2 , height() / 2 , dim , dim);
        QColor c;
        if (status == Status::Online)
            c = QColor(107, 242, 154);
        else if (status == Status::Offline)
            c = "gray";

        QBrush b(c);
        painter.setBrush(b);
        painter.drawEllipse(statusRect);
    }
}
void ProfilePicLabel::setPixmap1(QPixmap pixmap)
{
    int w = pixmap.width();
    int h = pixmap.height();

    int dim = std::min(w , h);
    pixmap = pixmap.copy(QRect(QPoint((w - dim) / 2 , (h - dim) / 2) , QSize(dim , dim)));

    QLabel::setPixmap(pixmap);
}

QSize ProfilePicLabel::sizeHint() const
{
    QSize size = pixmap().size();
    int max = std::max(size.width() , size.height());
    return { max , max};
}
QSize ProfilePicLabel::minimumSizeHint() const { return sizeHint();}

void ProfilePicLabel::setStatusEnabled(bool enabled)
{
    _statusEnabled = enabled;
}

void ProfilePicLabel::setStatus(Status status0)
{
    status = status0;
}
