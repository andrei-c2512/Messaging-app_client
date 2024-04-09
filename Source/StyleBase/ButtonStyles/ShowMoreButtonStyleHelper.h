
#ifndef SHOWMOREBUTTONSTYLEHELPER_H
#define SHOWMOREBUTTONSTYLEHELPER_H
#include "SelectableButtonStyleHelper.h"
#include "CustomStyledWidgets/SelectableButton.h"

class ShowMoreButtonStyleHelper : public SelectableButtonStyleHelper {
    Q_OBJECT
public:
    enum class Type{
        Plus_Minus,
        Arrows_Horizontal,
        Arrows_Vertical,
        Invalid
    };
public:
    ShowMoreButtonStyleHelper(QPushButton* btn, Type type = Type::Plus_Minus);
    bool opened() const;

    //use this one
    void setState(bool state);
    static Type getType(QString str) ;

    void setType(QString typeName);
    void setType(Type type);
    QString typeName() const;

    void drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept override;
    void drawButtonShape(const QStyleOptionButton* option , QPainter* painter)const noexcept override;
    void updateAnimations(const QStyleOptionButton* option)const noexcept override;
    void setManagingWidget(SelectableButton* button);
protected:
    QPen   borderPen(const QStyleOptionButton* option ) const noexcept override;
    QBrush backgroundBrush(const QStyleOptionButton* option ) const noexcept override;
    void settings_setup() override;
    using SelectableButtonStyleHelper::setManagingWidget;
protected:
    bool Opened;
    static bool mapsInitialized;

    static std::map<Type , QString> imgOpen_map;
    static std::map<Type , QString> imgClosed_map;
    QString _typeName;

};

#endif // SHOWMOREBUTTONSTYLEHELPER_H
