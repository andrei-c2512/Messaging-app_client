#include "ProfilePicLabel.h"

void ProfilePicLabel::paintEvent(QPaintEvent* event)
{
    //const int dim = std::min(width() , height());
    //const int x = (width() - dim) / 2;
    //const int y = (height() - dim) / 2;

    QPainter painter(this);
    QPainterPath path;
    path.addEllipse(rect());
    painter.setClipPath(std::move(path));

    int min = std::min(width() , height());
    painter.drawPixmap(QRect(QPoint((width() - min) / 2 , (height() - min) / 2 ) , QSize(min , min)) , pixmap());
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

