#pragma once
#include "ContactView.h"
#include <QCheckBox>

class UserCheckBox : public QCheckBox {
    //so that I can style it in qss
    Q_OBJECT
public:
    using QCheckBox::QCheckBox;
};


class ContactView_UserSelector : public ContactView {
public:
    ContactView_UserSelector(QWidget* parent = nullptr);
    void attatchCheckbox();
    //works only if the options widget with the checkbox is active
    void setChecked(bool status);
    bool checked() const;
private:
    UserCheckBox* pCheckbox;
};