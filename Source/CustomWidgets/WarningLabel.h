
#ifndef WARNINGLABEL_H
#define WARNINGLABEL_H
#include <QLabel>
#include <QGraphicsOpacityEffect>

class WarningLabel : public QLabel
{
    Q_OBJECT;
public:
    WarningLabel(QWidget* parent = nullptr);
    bool active() const;
    //set what has to be done to enable this warning
    void addWarningCondition(std::function<bool()> cond);
    void setWarningCondtions(std::vector<std::function<bool()>>);
    void setText(QString str);
public slots:
    void setState(bool state);
    void checkCondition();
private:
    bool _active;
    QGraphicsOpacityEffect* pEffect = nullptr;
    std::vector<std::function<bool()>> warningCondtionList;
};

#endif // WARNINGLABEL_H
