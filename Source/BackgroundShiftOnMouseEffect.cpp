#include "BackgroundShiftOnMouseEffect.h"


PixmapPortion::PixmapPortion(QPixmap* pixmap , QRect portion) : pPixmap(pixmap) ,  _customPortion(false) , _portion(portion)
{
    _standardPortion = std::move(portion);
}
void PixmapPortion::setPortion(QRect rect) {
    _portion = std::move(rect);
    _customPortion = true;
}
void PixmapPortion::reset()
{
    _portion = _standardPortion;
    pPixmap = nullptr;
}

bool PixmapPortion::customPortion() const{ return _customPortion; }
const QRect& PixmapPortion::portion() const { return _portion; }
QPixmap PixmapPortion::pixmapPortion() const { return pPixmap->copy(_portion);}


BackgroundShiftOnMouseEffect::BackgroundShiftOnMouseEffect(QWidget* parent , QPixmap background , float scale)
    :QWidget(parent) , _originalBackground(std::move(background)) , _scale(scale)
{
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    _scaledBackgroundRect = _originalBackground.rect();
    _scaledBackgroundRect.setSize(_scaledBackgroundRect.size() * scale);
    scaleBackground();

    qDebug() << "Background widget width: " << width();
    qDebug() << "Background widget height: " << height();
    _current = QRect(QPoint(_scaledBackgroundRect.width() / 2 - width() / 2 , _scaledBackgroundRect.height() / 2 - height() / 2 ), size() );
    redrawTimer = new QTimer;
    redrawTimer->start(17);

    measureTimer = new QTimer(this);
    measureTimer->start(1000);
    /*connect(measureTimer , &QTimer::timeout , this , [=](){
        qDebug() << "Redraw count:" <<redrawsPerSec;
        redrawsPerSec = 0;
    });*/
    connect(redrawTimer , SIGNAL(timeout())  , this , SLOT(updateBackground()));
    velocity = { 0.0f , 0.0f};

}

void BackgroundShiftOnMouseEffect::scaleBackground()
{
    int w = _originalBackground.width();
    int h = _originalBackground.height();


    int remainsX = w % chunkSize.width();
    int remainsY = h % chunkSize.height();
    //matrix width
    _bgWidth = w / chunkSize.width() + bool(remainsX);
    //matrix height
    _bgHeight = h / chunkSize.height() + bool(remainsY);

    _backgroundMatrix.resize(_bgHeight * _bgWidth);

    unsigned deltaY ;
    for(int y = 0 ; y < _bgHeight - 1; y += 1)
    {
        deltaY = y * _bgWidth;
        for(int x = 0 ; x < _bgWidth - 1; x += 1)
        {
            _backgroundMatrix[deltaY + x] = _originalBackground.copy(QRect(QPoint(x * chunkSize.width() ,y *chunkSize.height()) , chunkSize));
            _backgroundMatrix[deltaY + x] = _backgroundMatrix[deltaY + x].scaled(chunkSize * _scale);
        }
    }
    QRect auxRect;
    //going through the last row
    int lastRowHeight = (remainsY) ? chunkSize.height() : remainsY;
    deltaY = (_bgHeight - 1) * _bgWidth;
    for(int x = 0 ; x < _bgWidth -1 ; x++)
    {
        auxRect = QRect(x * chunkSize.width() , (_bgHeight - 1) *chunkSize.height() , chunkSize.width() , lastRowHeight);
        _backgroundMatrix[deltaY + x] = _originalBackground.copy(auxRect);
        _backgroundMatrix[deltaY + x] = _backgroundMatrix[deltaY + x].scaled(auxRect.size() * _scale);
    }


    //going through the last column
    int lastRowWidth = (remainsX) ? chunkSize.width() : remainsX;
    for(int y = 0 ; y < _bgHeight -1 ; y++)
    {
        auxRect = QRect((_bgWidth - 1) * chunkSize.width() , y *chunkSize.height() , lastRowWidth ,  chunkSize.height());
        _backgroundMatrix[y * _bgWidth + (_bgWidth - 1)] = _originalBackground.copy( auxRect );
        _backgroundMatrix[y * _bgWidth + (_bgWidth - 1)] = _backgroundMatrix[y * _bgWidth + (_bgWidth - 1)].scaled(auxRect.size() * _scale);
    }
    auxRect = QRect(QPoint((_bgWidth -1) * chunkSize.width() , (_bgHeight - 1) * chunkSize.height()) , QSize(lastRowWidth , lastRowHeight));

    _backgroundMatrix.back() = _originalBackground.copy(auxRect);
    _backgroundMatrix.back() = _backgroundMatrix.back().scaled(auxRect.size() * _scale);
    chunkSize *= _scale;
}
void BackgroundShiftOnMouseEffect::setScale(int scale) noexcept{
    _scale = std::move(scale);
    _scaledBackgroundRect.setSize( _scale * _scaledBackgroundRect.size() );
}
void BackgroundShiftOnMouseEffect::setBackground(QPixmap pixmap) noexcept
{
    _originalBackground = std::move(pixmap);
    _scaledBackgroundRect.setSize( _scale * _scaledBackgroundRect.size() );
}
QPixmap BackgroundShiftOnMouseEffect::background() const { return _originalBackground;}
int BackgroundShiftOnMouseEffect::scale() const{return _scale;}

void BackgroundShiftOnMouseEffect::paintEvent(QPaintEvent* ev) noexcept
{
    Q_UNUSED(ev);
    QPainter painter(this);
    QRect _current1 = QRect(_current.left() , _current.top() , _current.width() , _current.height());

    //_current1.adjust(_scaledBackgroundRect.width() / 2 , _scaledBackgroundRect.height() / 2 , _scaledBackgroundRect.width() / 2 , _scaledBackgroundRect.height() / 2);
    QPoint topLeft = {_current1.left() / chunkSize.width() ,
                      _current1.top() / chunkSize.height()};

    QPoint bottomRight = { _current1.right() / chunkSize.width() + bool(_current1.right() % chunkSize.width()) ,
                          _current1.bottom() / chunkSize.height() + bool(_current1.bottom() % chunkSize.height())};

    int drawingMatrixW = bottomRight.x() - topLeft.x();
    int drawingMatrixH = bottomRight.y() - topLeft.y();
    _drawingMatrix.resize(drawingMatrixW * drawingMatrixH , PixmapPortion(nullptr , QRect(QPoint() , chunkSize)));


    for(int y = topLeft.y() ; y < bottomRight.y() ; y++)
    {
        //drawing matrix row index auxiliary
        int drawingIndAux = (y - topLeft.y()) * drawingMatrixW;
        //pixmap matrix row index auxiliary
        int pixmapIndAux = y * _bgWidth;

        for(int x = topLeft.x() ; x < bottomRight.x() ; x++)
        {
            _drawingMatrix[drawingIndAux + x - topLeft.x()].pPixmap = &_backgroundMatrix[pixmapIndAux + x];
        }
    }


    //first , we have to get the sizes of the corner pixmaps
    //topLeft corner

    QSize tlCornerSize = QSize(chunkSize.width() - (_current1.left() - topLeft.x() * chunkSize.width()) ,
                               chunkSize.height() - (_current1.top() - topLeft.y() * chunkSize.height()));
    //topRight corner
    QSize trCornerSize =  QSize(_current1.right() - (bottomRight.x() - 1) * chunkSize.width()  ,
                              chunkSize.height() - (_current1.top() - topLeft.y() * chunkSize.height()));
    //bottomLeft corner
    QSize blCornerSize = QSize(chunkSize.width() - (_current1.left() - topLeft.x() * chunkSize.width()),
                               _current1.bottom() - (bottomRight.y()  - 1)* chunkSize.height());
    //bottomRight
    QSize brCornerSize = QSize( _current1.right() - (bottomRight.x() - 1) * chunkSize.width(),
                               _current1.bottom() - (bottomRight.y()  - 1)* chunkSize.height());



    //top margin
    _drawingMatrix[0].setPortion(QRect( QPoint(chunkSize.width() - tlCornerSize.width() , chunkSize.height() - tlCornerSize.height()), tlCornerSize));
    _drawingMatrix[drawingMatrixW - 1 ].setPortion(QRect(QPoint(0 , chunkSize.height() - trCornerSize.height() ) , trCornerSize));
    _drawingMatrix[drawingMatrixW * (drawingMatrixH - 1)].setPortion(QRect(QPoint( chunkSize.width() - blCornerSize.width(), 0 ) , blCornerSize));
    _drawingMatrix.back().setPortion(QRect( QPoint(0 , 0), brCornerSize));

    for(int x = 1 ; x < drawingMatrixW - 1; x++)
    {
        _drawingMatrix[x].setPortion(QRect(QPoint(0 , chunkSize.height() - tlCornerSize.height()) , QSize(chunkSize.width() , _drawingMatrix[0].portion().height())));
        _drawingMatrix[drawingMatrixW * (drawingMatrixH - 1) + x].setPortion(QRect(QPoint() , QSize(chunkSize.width() ,
                                                                             _drawingMatrix[drawingMatrixW * (drawingMatrixH - 1)].portion().height())));
    }

    for(int y = 1 ; y < drawingMatrixH - 1; y++)
    {
        _drawingMatrix[y * drawingMatrixW].setPortion(QRect(QPoint( chunkSize.width() - tlCornerSize.width() , 0 ) , QSize(_drawingMatrix[0].portion().width() , chunkSize.height())));
        _drawingMatrix[y * drawingMatrixW + drawingMatrixW - 1 ].setPortion(QRect(QPoint() , QSize(_drawingMatrix[drawingMatrixW - 1].portion().width(),  chunkSize.height())));
    }

    QPoint pos = QPoint(0 , 0);
    for(int y = 0 ; y < drawingMatrixH ; y++)
    {
        int aux = y * drawingMatrixW;
        pos.setX(0);
        for(int x = 0 ; x < drawingMatrixW ; x++)
        {
            int ind = aux + x;
            if(_drawingMatrix[ind].customPortion())
                painter.drawPixmap(QRect(pos , _drawingMatrix[ind].portion().size()), *_drawingMatrix[ind].pPixmap , _drawingMatrix[ind].portion());
            else
                painter.drawPixmap(QRect(pos , _drawingMatrix[ind].portion().size()), *_drawingMatrix[ind].pPixmap);

            pos.setX(pos.x() +  _drawingMatrix[ind].portion().width());

        }
        pos.setY( pos.y() + _drawingMatrix[aux].portion().height());
    }


    for(auto& obj : _drawingMatrix)
        obj.reset();
}

void BackgroundShiftOnMouseEffect::updateBackground() noexcept
{
    QPoint globalMousePos = QCursor::pos();
    if(oldMousePos != globalMousePos)
    {
        calculateVelocity(globalMousePos);
        oldMousePos = std::move(globalMousePos);
    }
    QPoint lastRectPos = QPoint(_current.x() , _current.y());
    updateRectPos();
    //std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::time_point::clock::now();
    velocity *= decreaseFactor;
    if(lastRectPos != QPoint(_current.x() , _current.y()))
    {
        //redrawsPerSec++;
    }
    update();
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::time_point::clock::now();
    //std::chrono::duration dur = end - beg;

    //qDebug() << dur.count() * 1000;
    //qDebug() << "Rect pos:" << _current.x() << ' ' << _current.y();
}
void BackgroundShiftOnMouseEffect::updateRectPos() noexcept
{
    //if(_current.right()+ velocity.x() > _scaledBackground.width())
    //    velocity.setX(0);
    //else if(_current.x() + velocity.x() < 0)
    //{
    //    velocity.setX(0); _current.setX(0);
    //}
    //if(_current.bottom() + velocity.y() > _scaledBackground.height())
    //    velocity.setY(0);
    //else if(_current.y() + velocity.y() < 0)
    //{
    //    velocity.setY(0); _current.setY(0);
    //}
    _current.setRect(_current.x() + velocity.x() , _current.y() + velocity.y(), width() , height());
    //qDebug() << "Rect pos:" << _current.x() << ' ' << _current.y();
}


void BackgroundShiftOnMouseEffect::calculateVelocity(QPoint mousePos) noexcept
{
    QPointF newMousePosF = posToBackgroundPos(std::move(mousePos));
    QPointF oldMousePosF = posToBackgroundPos(oldMousePos);

    velocity += QPointF((newMousePosF.x() - oldMousePosF.x()) / moveInterval * speed , (newMousePosF.y() - oldMousePosF.y()) / moveInterval * speed);
    //qDebug() << velocity.x() << ' ' << velocity.y();
}
QPointF BackgroundShiftOnMouseEffect::posToBackgroundPos(const QPointF& pos) const noexcept
{

    /* Code that shows in more detail how I calculated this , it's all commented so that you can understand
     * All I did was convert this:
    QPointF mousePos = pos - QPointF(width() / 2 , height() / 2);

    float xRatio = mousePos.x() / float(width() / 2);
    float yRatio = mousePos.y() / float(height() / 2);

    //center coordinate for the big picture
    QPointF center = QPoint(_scaledBackground.width() / 2 , _scaledBackground.height() / 2);

    //the current center position
    float centerX = center.x() + width()  / 2  * (_scale) * xRatio;
    float centerY = center.y() + height() / 2  * (_scale) * yRatio;

    To an equation
    */

    return QPointF(_scaledBackgroundRect.width() + _scale * float(pos.x() - width() * 0.5f ) , _scaledBackgroundRect.height() + _scale * float(pos.y() - height() * 0.5f ));
}
void BackgroundShiftOnMouseEffect::resizeEvent(QResizeEvent* ev)
{
    qDebug() << "Background widget in resizeEvent width: " << width();
    qDebug() << "Background widget in resizeEvent height: " << height();
    _current = QRect(QPoint(_scaledBackgroundRect.width() / 2 - width() / 2 , _scaledBackgroundRect.height() / 2 - height() / 2 ), size() );
    oldMousePos = QPoint(_scaledBackgroundRect.width() / 2 - width() / 2 , _scaledBackgroundRect.height() / 2 - height() / 2 );
    QWidget::resizeEvent(ev);
}

void BackgroundShiftOnMouseEffect::updateAccordingToPage(PageManager::PageType type)
{
    switch(type)
    {
    case PageManager::PageType::Loading:
        setVisible(true);
        break;
    case PageManager::PageType::Login:
        setVisible(true);
        break;
    case PageManager::PageType::Register:
        setVisible(true);
        break;
    default:
        setVisible(false);
        break;
    }
}
