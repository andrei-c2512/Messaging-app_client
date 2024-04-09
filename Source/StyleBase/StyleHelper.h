
#ifndef STYLEHELPER_H
#define STYLEHELPER_H
#include <QWidget>
//a class that is friend with ColorShiftAnimationTool , which will enable acces to this for inhereting classes
class StyleHelper{
public:
    StyleHelper(QWidget* widget);
    const QWidget* managingWidget() const;
    void setManagingWidget(QWidget* widget);
protected:
    virtual void settings_setup();
protected:
    QWidget* pManagingWidget = nullptr;
};

#endif // STYLEHELPER_H
