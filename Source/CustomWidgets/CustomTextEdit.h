
#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QHBoxLayout>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTextCursor>
#include <QImageReader>
class CustomTextEdit : public QTextEdit{
    Q_OBJECT;
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged());
public:
    CustomTextEdit(QWidget* parent = nullptr);
    QColor getPlaceHolderColor() const;
    QColor getTextColor() const;

    void setPlaceHolderColor(const QColor& color);
    void setShrinkToFit(bool enabled);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    using QTextEdit::textColor;
    using QTextEdit::setTextColor;
    void setVisible(bool visible ) override;
    void insertImage(QUrl url);
private slots:
    void updateMaxHeight(const QSizeF& size);
signals:
    void textColorChanged();
private:
    QColor placeholderColor;

    bool _shrinkToFit;
    int _maxHeight;

};




#endif // CUSTOMTEXTEDIT_H
