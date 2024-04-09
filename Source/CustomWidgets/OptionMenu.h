
#ifndef OPTIONMENU_H
#define OPTIONMENU_H
#include <thread>
#include <QHBoxLayout>
#include "CustomStyledWidgets/SelectableButton.h"

class OptionMenuObject : public QObject{
    Q_OBJECT
public:
    OptionMenuObject(QObject* parent = nullptr);
    void setOptionList(std::vector<SelectableButton*> option);
    void addOption(SelectableButton* btn);
    void useOption(int index) noexcept;
    void removeOption(SelectableButton* btn);
private slots:
    void updateButtons();
private:
    std::vector<SelectableButton*> optionList;
    friend class OptionMenu;
    int lastIndex = 0;
    SelectableButton* lastBtn = nullptr;
};

class OptionMenu : public QFrame{
    Q_OBJECT
public:
    OptionMenu(QWidget* parent = nullptr);
    void setOptionList(std::vector<SelectableButton*> option);
    void addOption(SelectableButton* btn);
    void removeOption(SelectableButton* btn);
    void setDefaultOption(int index);
    void useOption(int index);
private:
    OptionMenuObject* pOptionMenu;
    QHBoxLayout* pLayout = nullptr;
    int _default = 0;
};

#endif // OPTIONMENU_H
