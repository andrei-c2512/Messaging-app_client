#include "C_SelectableButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"

C_SelectableButtonStyleHelper::C_SelectableButtonStyleHelper(QPushButton* button , std::vector<AnimationPrototype*> prototypeList0 , std::vector<AnimationPrototype*> selected_prototypeList0)
    :CustomizableButtonStyleHelper(button , prototypeList0) , selected_prototypeList(selected_prototypeList0)
{
}
void C_SelectableButtonStyleHelper::updateAnimations(const QStyleOptionButton* option) const noexcept
{
    if(changedState)
    {
        const std::vector<AnimationPrototype*>* ptr = nullptr;
        ptr = (_selected) ? &selected_prototypeList : &prototypeList;

        for(unsigned i = 0 ; i < activeAnimList.size() ; i++)
        {
            AnimationPrototype* currentPrototype = (*ptr)[i];

            QVariantAnimation& anim =  activeAnimList[i]->animation();
            anim.setStartValue(currentPrototype->start);
            anim.setEndValue(currentPrototype->end);
            switch(currentPrototype->type)
            {
            case Type::IconColor:
                //static cast because derived types are already verfied
                if(const IconColorAnimationPrototype* p = static_cast<const IconColorAnimationPrototype*>(currentPrototype))
                {
                    //btn->setIcon(p->pixmap);
                    pManagingWidget_Button->setIconSize(p->iconSize);
                    //this is very costly because if i change the pixmap then I have to copy a big chunk of memory , then read all non transparent pixels , then note them yada yada
                    //i need to store them somehow
                    static_cast<IconColorAnimationManager*>(activeAnimList[i])->painter().setPixmap(p->pixmap);
                }
                break;
            default:
                break;
            }
        }
        changedState = false;
    }

    CustomizableButtonStyleHelper::updateAnimations(option);
}
