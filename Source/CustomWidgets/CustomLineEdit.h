#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H
#include <QLineEdit>
#include "WarningLabel.h"
#include <QVBoxLayout>

class CustomLineEdit : public QWidget
{
    Q_OBJECT;
public:
    CustomLineEdit(QWidget* parent = nullptr);
    void setCharacterLimit( unsigned int limit );
    void setPlaceHolderColor(const QColor& style);
    void setWarningUse(bool enabled);

    QLineEdit& lineEdit() const;
    QLineEdit* lineEditPointer() const;
    WarningLabel& warningLabel() const;
    unsigned int characterLimit();
    QSize sizeHint() const;
    bool warningUseEnabled() const;
signals:
    void limitReached(bool reached);
    void textChanged();
private slots:
    void update(const QString& str);
    void checkLimit(const QString&text);
    void emitTextChanged(const QString& str);
private:
    QColor _placeHolderColor;
    QColor _defaultColor;

    QVBoxLayout* pLayout;
    QLineEdit* pLineEdit;
    WarningLabel* pWarning;

    QString characterLimitWarningText;
    QString conditionWarningText;
    bool wasEmpty;
    unsigned int _characterLimit = 100;
    bool _warningEnabled;
};

#endif // CUSTOMLINEEDIT_H
