#include "ImagePainter.h"
#include "Tools.h"
ImagePainter::ImagePainter(QObject* parent , QPixmap pixmap)
    :QObject(parent)
{
    setPixmap(std::move(pixmap));
}

void ImagePainter::setPixmap(QPixmap pixmap) noexcept
{
    _original = std::move(pixmap);
    posList = Tools::getPixmapPixelList(_original);
    _image = _original.toImage();

    QRgb rgb = _color.rgb();
    for(const QPoint& pos : posList)
    {
        _image.setPixel(pos , rgb);
    }
}

QColor ImagePainter::color() const noexcept{ return _color;}
QImage ImagePainter::image() const noexcept{ return _image;}
QPixmap ImagePainter::pixmap() const noexcept{ return _original;}
QIcon* ImagePainter::icon() const noexcept { return new QIcon(QPixmap::fromImage(_image));}

void ImagePainter::setColor(QColor color) noexcept
{
    _color = color;
    QRgb rgb = _color.rgb();
    for(const QPoint& pos : posList)
    {
        _image.setPixel(pos , rgb);
    }
}
void ImagePainter::setPixmap(QPixmap pixmap , QColor color) noexcept
{
    _original = std::move(pixmap);
    _image = _original.toImage();
    _color = color;
    posList = Tools::getImagePixelList(_image);

    QRgb rgb = _color.rgb();
    for(const QPoint& pos : posList)
    {
        _image.setPixel(pos , rgb);
    }
}

QImage ImagePainter::paintImage(QPixmap pixmap, QColor color)
{
    QImage img = pixmap.toImage();
    
    auto posList = Tools::getImagePixelList(img);
    QRgb rgb = color.rgb();
    for (const QPoint& pos : posList)
    {
        img.setPixel(pos, rgb);
    }
    return img;
}