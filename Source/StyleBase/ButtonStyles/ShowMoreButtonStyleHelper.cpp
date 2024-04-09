#include "ShowMoreButtonStyleHelper.h"
#include "StyleBase/StyleRepository.h"

bool ShowMoreButtonStyleHelper::mapsInitialized = false;
std::map<ShowMoreButtonStyleHelper::Type , QString> ShowMoreButtonStyleHelper::imgClosed_map;
std::map<ShowMoreButtonStyleHelper::Type , QString> ShowMoreButtonStyleHelper::imgOpen_map;

ShowMoreButtonStyleHelper::ShowMoreButtonStyleHelper(QPushButton* btn, ShowMoreButtonStyleHelper::Type type)
    :SelectableButtonStyleHelper(btn)
{
    if(btn)
        ShowMoreButtonStyleHelper::settings_setup();
}

ShowMoreButtonStyleHelper::Type ShowMoreButtonStyleHelper::getType(QString str)
{
    //the eighth character difffers in every single type name
    char ch = str[7].unicode();

    switch(ch)
    {
    case 'n':
        return ShowMoreButtonStyleHelper::Type::Plus_Minus;
    case 'H':
        return ShowMoreButtonStyleHelper::Type::Arrows_Horizontal;
    case 'V':
        return ShowMoreButtonStyleHelper::Type::Arrows_Vertical;
    default:
        return ShowMoreButtonStyleHelper::Type::Invalid;
    }
}

void ShowMoreButtonStyleHelper::setType(QString typeName)
{
    setType(getType(std::move(typeName)));
}
void ShowMoreButtonStyleHelper::setType(Type type)
{
    selected_pixmap   = imgOpen_map[type];
    unselected_pixmap = imgClosed_map[type];
}
QString ShowMoreButtonStyleHelper::typeName() const{ return _typeName; }
bool ShowMoreButtonStyleHelper::opened() const{ return Opened;}
void ShowMoreButtonStyleHelper::setState(bool state){ Opened = state;}

void ShowMoreButtonStyleHelper::drawButtonShape(const QStyleOptionButton* option , QPainter* painter)const noexcept
{

}
void ShowMoreButtonStyleHelper::drawLabel(const QStyleOptionButton* option , QPainter* painter) const noexcept
{
    QImage image = icon();
    QRect rect( QPoint((option->rect.width() - _iconSize.width()) / 2 , (option->rect.height() - _iconSize.height ()) / 2) ,_iconSize );
    painter->drawImage(std::move(rect) , std::move(image));
}
void ShowMoreButtonStyleHelper::updateAnimations(const QStyleOptionButton* option)const noexcept
{
    if(changedState)
    {
        QColor borderStart , borderEnd , iconStart , iconEnd;
        if(_selected)
        {
            borderStart = StyleRepository::EditButton::ConfirmState::hoverBorderColor();
            borderEnd = StyleRepository::EditButton::ConfirmState::borderColor();
            iconStart = StyleRepository::EditButton::ConfirmState::hoverIconColor();
            iconEnd = StyleRepository::EditButton::ConfirmState::iconColor();
            currentPixmap = &selected_pixmap;
        }
        else
        {
            borderStart = StyleRepository::EditButton::StartEditState::borderColor();
            borderEnd = StyleRepository::EditButton::StartEditState::hoverBorderColor();
            iconStart = StyleRepository::EditButton::StartEditState::iconColor();
            iconEnd = StyleRepository::EditButton::StartEditState::hoverIconColor();
            currentPixmap = &unselected_pixmap;
        }
        QVariantAnimation& borderAnim = animationList[int(AnimationTool::Type::BorderColor)]->animation();
        borderAnim.setStartValue(std::move(borderStart));
        borderAnim.setEndValue(std::move(borderEnd));

        auto& iconAnimManager = *static_cast<IconColorAnimationManager*>(animationList[int(AnimationTool::Type::IconColor)]);
        QVariantAnimation& iconAnim = iconAnimManager.animation();
        iconAnim.setStartValue(std::move(iconStart));
        iconAnim.setEndValue(std::move(iconEnd));
        ImagePainter& painter = iconAnimManager.painter();
        painter.setPixmap(*currentPixmap);
    }
    changedState = false;
    auto state = option->state;
    if(state & QStyle::State_MouseOver)
        setDirectionForActives(QAbstractAnimation::Direction::Forward);
    else
        setDirectionForActives(QAbstractAnimation::Direction::Backward);
}
void ShowMoreButtonStyleHelper::setManagingWidget(SelectableButton* button){ SelectableButtonStyleHelper::setManagingWidget(button); }

QPen   ShowMoreButtonStyleHelper::borderPen(const QStyleOptionButton* option ) const noexcept
{
    return QPen();
}
QBrush ShowMoreButtonStyleHelper::backgroundBrush(const QStyleOptionButton* option ) const noexcept
{
    return QBrush();
}
void ShowMoreButtonStyleHelper::settings_setup()
{
    Opened = false;
    Type type = Type::Arrows_Vertical;

    if(mapsInitialized == false)
    {
        //closed map
        QString Plus_MinusSS_Closed       = ":/ShowMoreIcons/ShowMorePlusMinus.png";
        QString Arrow_HorizontalSS_Closed = ":/ShowMoreIcons/ShowMoreArrowHorizontal.png";
        QString Arrow_VerticalSS_Closed   = ":/ShowMoreIcons/ShowMoreArrowVertical.png";

        imgClosed_map.emplace(ShowMoreButtonStyleHelper::Type::Plus_Minus , std::move(Plus_MinusSS_Closed));
        imgClosed_map.emplace(ShowMoreButtonStyleHelper::Type::Arrows_Horizontal , std::move(Arrow_HorizontalSS_Closed));
        imgClosed_map.emplace(ShowMoreButtonStyleHelper::Type::Arrows_Vertical , std::move(Arrow_VerticalSS_Closed));

        //open map
        QString Plus_MinusSS_Opened       = ":/ShowMoreIcons/ShowLessPlusMinus.png";
        QString Arrow_HorizontalSS_Opened = ":/ShowMoreIcons/ShowLessArrowHorizontal.png";
        QString Arrow_VerticalSS_Opened   = ":/ShowMoreIcons/ShowLessArrowVertical.png";

        imgOpen_map.emplace(ShowMoreButtonStyleHelper::Type::Plus_Minus , std::move(Plus_MinusSS_Opened));
        imgOpen_map.emplace(ShowMoreButtonStyleHelper::Type::Arrows_Horizontal , std::move(Arrow_HorizontalSS_Opened));
        imgOpen_map.emplace(ShowMoreButtonStyleHelper::Type::Arrows_Vertical , std::move(Arrow_VerticalSS_Opened));

        mapsInitialized = true;
    }

    selected_pixmap   = imgOpen_map[type];
    unselected_pixmap = imgClosed_map[type];
    const int duration = 250;
    _iconSize = { 24 , 24};

    AnimationTool::createTool(AnimationTool::Type::BackgroundColor , StyleRepository::Base::backgroundColor() , StyleRepository::Base::hoverBackgroundColor() , duration);
    AnimationTool::createTool(AnimationTool::Type::BorderColor , StyleRepository::EditButton::StartEditState::borderColor() ,
                              StyleRepository::EditButton::StartEditState::hoverBorderColor() , duration);
    AnimationTool::createIconColorTool(StyleRepository::EditButton::StartEditState::iconColor() ,
                                       StyleRepository::EditButton::StartEditState::hoverIconColor() , duration , unselected_pixmap);

    QPushButton* btn = static_cast<QPushButton*>(pManagingWidget);
    btn->setIcon(QPixmap(unselected_pixmap));
    btn->setIconSize(_iconSize);

    currentPixmap = &unselected_pixmap;
}

