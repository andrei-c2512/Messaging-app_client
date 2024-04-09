
#ifndef C_SELECTABLEBUTTONSTYLEHELPER_H
#define C_SELECTABLEBUTTONSTYLEHELPER_H
#include "CustomizableButtonStyleHelper.h"

//C stand for 'Customizable'
class C_SelectableButtonStyleHelper : public CustomizableButtonStyleHelper{
public:
    C_SelectableButtonStyleHelper(QPushButton* button , std::vector<AnimationPrototype*> default_prototypeList0 , std::vector<AnimationPrototype*> selected_prototypeList0);
    virtual void updateAnimations(const QStyleOptionButton* option) const noexcept override;
protected:
    std::vector<AnimationPrototype*> selected_prototypeList;
};

#endif // C_SELECTABLEBUTTONSTYLEHELPER_H
