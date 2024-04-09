#include "CustomTextEdit.h"

CustomTextEdit::CustomTextEdit(QWidget* parent)
    :QTextEdit(parent)
{
    placeholderColor = "gray";

    QTextEdit::setContentsMargins(0 , 0 , 0 , 0);
    QTextEdit::document()->setDocumentMargin(0);

    _shrinkToFit = false;
    connect(this , SIGNAL(textColorChanged()) , this , SLOT(update()));
}

QColor CustomTextEdit::getPlaceHolderColor() const
{
    return placeholderColor;
}

void CustomTextEdit::setPlaceHolderColor(const QColor& color)
{
    placeholderColor = color;
}
void CustomTextEdit::setShrinkToFit(bool enabled)
{
    _shrinkToFit = std::move(enabled);
    if(_shrinkToFit)
        connect(QTextEdit::document()->documentLayout() , SIGNAL(documentSizeChanged(const QSizeF&)) , this , SLOT(updateMaxHeight(const QSizeF&)));
    else
        disconnect(QTextEdit::document()->documentLayout() , SIGNAL(documentSizeChanged(const QSizeF&)) , this , SLOT(updateMaxHeight(const QSizeF&)));
}
void CustomTextEdit::updateMaxHeight(const QSizeF& size)
{
    //for some reason when I resize and the text has only one line , it has a weird visual bug when I write , adding some space fixes it
    // but I can't figure out where that space comes from. Shitty ass documentation
    const int val = 10;
    QTextEdit::setMaximumHeight(size.height() + val);
    QFrame::setMaximumHeight(size.height() + val);
    _maxHeight = size.height() + val;
}

QSize CustomTextEdit::sizeHint() const
{
    if(_shrinkToFit)
    {
        return QSize(QTextEdit::sizeHint().width() , _maxHeight);
    }
    else
        return QTextEdit::sizeHint();
}
QSize CustomTextEdit::minimumSizeHint() const
{
    return sizeHint();
}

void CustomTextEdit::setVisible(bool visible )
{
    QTextEdit::setVisible(std::move(visible));
}
