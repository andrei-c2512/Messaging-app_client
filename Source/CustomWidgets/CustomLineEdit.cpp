#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget* parent)
    :QWidget(parent)
{
    wasEmpty = true;
    QMargins noMargins = QMargins(0 , 0 , 0 , 0);
    QWidget::setContentsMargins(noMargins);

    pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(noMargins);

    pLineEdit = new QLineEdit();
    pLineEdit->setTextMargins(noMargins);
    pLineEdit->setContentsMargins(noMargins);


    characterLimitWarningText = "Reached character limit of " + QString::number(_characterLimit);
    pLayout->addWidget(pLineEdit);
    _warningEnabled = false;
    pWarning = nullptr;
    connect(pLineEdit , SIGNAL(textChanged(const QString&)) , this , SLOT(emitTextChanged(const QString&)));
    connect(pLineEdit , SIGNAL(textChanged(const QString&)) , this , SLOT(update(const QString&)));
}
void CustomLineEdit::update(const QString& str)
{
    Q_UNUSED(str);

    QPalette palette = pLineEdit->palette();
    if(pLineEdit->text().isEmpty() && wasEmpty == false)
    {
        _defaultColor = palette.color(QPalette::Text);
        palette.setColor(QPalette::Text , _placeHolderColor);
        pLineEdit->setPalette(std::move(palette));
        wasEmpty = true;
    }
    else
    {
        if(wasEmpty)
        {
            palette.setColor(QPalette::Text , _defaultColor);
            pLineEdit->setPalette(std::move(palette));
            wasEmpty = false;
        }
    }
}
void CustomLineEdit::emitTextChanged(const QString& str)
{
    emit textChanged();
}
QSize CustomLineEdit::sizeHint() const
{
    QMargins margins = pLayout->contentsMargins() + QWidget::contentsMargins();
    QSize lineEditSize = pLineEdit->sizeHint();

    return QSize(lineEditSize.width() + margins.left() + margins.right() , lineEditSize.height() + margins.top() + margins.bottom());
}

void CustomLineEdit::checkLimit(const QString&text)
{
    QString copy = text;
    if(text.size() > _characterLimit)
    {
        copy.erase(std::begin(text) + _characterLimit , std::end(text));
        pLineEdit->setText(std::move(copy));
        emit limitReached(true);
    }
    else if(text.size() == _characterLimit)
    {
        pWarning->setText(characterLimitWarningText);
        emit limitReached(true);
    }
    else
    {
        emit limitReached(false);
    }
}

void CustomLineEdit::setPlaceHolderColor(const QColor& color)
{
    _placeHolderColor = color;
    if(pLineEdit->text().isEmpty() && wasEmpty == false)
    {
        QPalette palette = pLineEdit->palette();
        _defaultColor = palette.color(QPalette::Text);
        palette.setColor(QPalette::Text , _placeHolderColor);
        pLineEdit->setPalette(std::move(palette));
        wasEmpty = true;
    }
}

void CustomLineEdit::setCharacterLimit(unsigned int limit)
{
    _characterLimit = limit;
    characterLimitWarningText = "Reached character limit of " + QString::number(_characterLimit);
}
unsigned int CustomLineEdit::characterLimit()
{
    return _characterLimit;
}
QLineEdit& CustomLineEdit::lineEdit() const
{
    return *pLineEdit;
}
QLineEdit* CustomLineEdit::lineEditPointer() const
{
    return pLineEdit;
}

WarningLabel& CustomLineEdit::warningLabel() const
{
    assert(pWarning != nullptr);
    return *pWarning;
}
void CustomLineEdit::setWarningUse(bool enabled)
{
    _warningEnabled = std::move(enabled);
    if(_warningEnabled)
    {
        pWarning = new WarningLabel;
        pWarning->addWarningCondition([this](){
            if(this->pLineEdit->text().length() >= this->characterLimit())
            {
                pWarning->setText(characterLimitWarningText);
                return true;
            }
            else
                return false;
        });
        pLayout->addWidget(pWarning);
        connect(pLineEdit , SIGNAL(textChanged(const QString&)) , this , SLOT(checkLimit(const QString&)));
        connect(this , SIGNAL(textChanged()) , pWarning , SLOT(checkCondition()));
    }
    else
    {
        if(pWarning)
            pWarning->deleteLater();
        pWarning = nullptr;
        disconnect(pLineEdit , SIGNAL(textChanged(const QString&)) , this , SLOT(update(const QString&)));
        disconnect(pLineEdit , SIGNAL(textChanged(const QString&)) , this , SLOT(checkLimit(const QString&)));
    }
}
bool CustomLineEdit::warningUseEnabled() const
{
    return _warningEnabled;
}

