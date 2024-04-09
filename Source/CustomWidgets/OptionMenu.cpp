#include "OptionMenu.h"

OptionMenuObject::OptionMenuObject(QObject* parent)
    :QObject(parent)
{}

void OptionMenuObject::setOptionList(std::vector<SelectableButton*> option)
{
    optionList = std::move(option);
    for(unsigned ind = 0u ; ind < optionList.size() ; ind++)
        connect(optionList[ind] , SIGNAL(indexOnClick(int)) , this , SLOT(useOption()));

    if(option.size())
        lastBtn = option.back();

}
void OptionMenuObject::addOption(SelectableButton* btn)
{
    assert(btn != nullptr);
    btn->setIndex((int)optionList.size());
    optionList.emplace_back(btn);
    connect(btn , SIGNAL(clicked()) , this , SLOT(updateButtons()));
    lastBtn = btn;
}

void OptionMenuObject::updateButtons()
{
    // i know this is risky
    QObject* pObject = QObject::sender();
    SelectableButton* pushBtn = static_cast<SelectableButton*>(pObject);

    lastBtn->setState(false);
    pushBtn->setState(true);
    lastBtn = pushBtn;

}


void OptionMenuObject::useOption(int index) noexcept
{
    optionList[lastIndex]->setState(false);
    optionList[index]->setState(true);
    lastIndex = std::move(index);
    lastBtn = optionList[lastIndex];
}
void OptionMenuObject::removeOption(SelectableButton* btn)
{
    std::vector<SelectableButton*>::iterator it = std::begin(optionList) + (btn - (*std::begin(optionList)));
    optionList.erase(it);
}
OptionMenu::OptionMenu(QWidget* parent)
    :QFrame(parent) , pOptionMenu(new OptionMenuObject(this))
{
    QMargins _default = QMargins(0 , 0 , 0 ,0);

    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(_default);
    pLayout->addStretch(1);
    QFrame::setContentsMargins(_default);
}

void OptionMenu::setOptionList(std::vector<SelectableButton*> option)
{
    for(SelectableButton* button : pOptionMenu->optionList)
    {
        pLayout->removeWidget(button);
        button->deleteLater();
    }
    pOptionMenu->setOptionList(option);
    for(SelectableButton* button : pOptionMenu->optionList)
        pLayout->insertWidget(pLayout->count() - 1 , button);
}
void OptionMenu::addOption(SelectableButton* btn)
{
    pOptionMenu->addOption(btn);
    pLayout->insertWidget(pLayout->count() - 1 , btn);
}
void OptionMenu::removeOption(SelectableButton* btn)
{
    pOptionMenu->removeOption(btn);
}
void OptionMenu::useOption(int index){ pOptionMenu->useOption(index);}
