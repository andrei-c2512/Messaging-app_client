#include "ContactView_UserSelector.h"


ContactView_UserSelector::ContactView_UserSelector(QWidget* parent)  : ContactView(parent)
{
    pCheckbox = nullptr;
    pLayout->addStretch(1);
}

void ContactView_UserSelector::attatchCheckbox()
{
    if (pCheckbox == nullptr)
    {
        pCheckbox = new UserCheckBox;
        pLayout->addWidget(pCheckbox);
    }
}
bool ContactView_UserSelector::checked() const
{
    return pCheckbox->isChecked();
}

void ContactView_UserSelector::setChecked(bool status)
{
    return pCheckbox->setChecked(status);
}
