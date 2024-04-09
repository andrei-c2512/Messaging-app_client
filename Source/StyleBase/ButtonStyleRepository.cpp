#include "ButtonStyleRepository.h"

CustomizableButtonStyleHelper* ButtonStyleRepository::loginButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(65, 191, 155) , QColor(107, 242, 154) , false),
                                            new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(65, 191, 155) , QColor(107, 242, 154) , false),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(37, 80, 89) , QColor(20, 30, 38) , false)};
    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}
C_SelectableButtonStyleHelper* ButtonStyleRepository::showPasswordButton()
{
    QSize iconSize = { 32 , 32};
    QPixmap defaultIcon = QPixmap(":/images/OpenEye.png");
    QPixmap selectedIcon = QPixmap(":/images/ClosedEye.png");
    std::vector<AnimationPrototype*> defaultSettings  = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new AnimationPrototype(AnimationTool::Type::BorderColor , 	QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(37, 80, 89) , QColor(20, 30, 38) , false) ,
                                                         new IconColorAnimationPrototype(QColor(235, 239, 235) , QColor(235, 239, 235) , true , std::move(defaultIcon) , iconSize)};

    std::vector<AnimationPrototype*> selectedSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new AnimationPrototype(AnimationTool::Type::BorderColor , 	QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new AnimationPrototype(AnimationTool::Type::TextColor , QColor(37, 80, 89) , QColor(20, 30, 38) , false) ,
                                                          new IconColorAnimationPrototype(QColor(235, 239, 235) , QColor(235, 239, 235) , true , std::move(selectedIcon) , iconSize)};

    return new C_SelectableButtonStyleHelper(nullptr , std::move(defaultSettings) , std::move(selectedSettings));
}
CustomizableButtonStyleHelper* ButtonStyleRepository::onlyTextButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(65, 191, 155) , QColor(107, 242, 154) , false)};
    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}
CustomizableButtonStyleHelper* ButtonStyleRepository::goBackColor_LeftArrow()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , false , QPixmap(":/images/LeftArrow.png") , QSize(48 , 48))};
    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}

CustomizableButtonStyleHelper* ButtonStyleRepository::friendListButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	14, 21, 27) , QColor(14, 21, 27) , true),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(14, 21, 27) , QColor(14, 21, 27) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(222, 217, 226) , QColor(107, 242, 154) , false),
                                             new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , false , QPixmap(":/images/FriendListIcon2.jpg") , QSize(48 , 48))};

    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}
CustomizableButtonStyleHelper* ButtonStyleRepository::addFriendsButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(65, 191, 155) , QColor(107, 242, 154) , false),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(65, 191, 155) , QColor(107, 242, 154) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(20, 30, 38) , QColor(20, 30, 38) , true),
                                             new IconColorAnimationPrototype(QColor(20, 30, 38) , QColor(20, 30, 38) , false , QPixmap(":/images/AddFriendIcon.png") , QSize(32 , 32))};

    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}

CustomizableButtonStyleHelper* ButtonStyleRepository::acceptButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(35, 53, 67) , QColor(14, 21, 27) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new IconColorAnimationPrototype(QColor(49, 237, 115) , QColor(105, 242, 153) , true , QPixmap(":/Icons/AcceptIcon.png") , QSize(32 , 32))};

    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}
CustomizableButtonStyleHelper* ButtonStyleRepository::rejectButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(35, 53, 67) , QColor(14, 21, 27) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                             new IconColorAnimationPrototype(QColor(235, 81, 96) , QColor(239, 35, 60) , true , QPixmap(":/Icons/RejectIcon.png") , QSize(32 , 32))};

    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}
CustomizableButtonStyleHelper* ButtonStyleRepository::moreOptionsButton()
{
    std::vector<AnimationPrototype*> vec = { new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                         new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(35, 53, 67) , QColor(14, 21, 27) , true),
                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                         new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , true , QPixmap(":/Icons/MoreOptionsIcon.png") , QSize(32 , 32)) };

    return new CustomizableButtonStyleHelper(nullptr, std::move(vec));
}
CustomizableButtonStyleHelper* ButtonStyleRepository::messageButton()
{
    std::vector<AnimationPrototype*> vec = { new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                         new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(35, 53, 67) , QColor(14, 21, 27) , true),
                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                         new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , true , QPixmap(":/Icons/MessageIcon.png") , QSize(32 , 32)) };

    return new CustomizableButtonStyleHelper(nullptr, std::move(vec));
}

C_SelectableButtonStyleHelper* ButtonStyleRepository::socialPageMenuButton(QPixmap map)
{
    QSize iconSize = { 32 , 32};
    QPixmap icon = map;

    std::vector<AnimationPrototype*> defaultSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	14, 21, 27) , QColor(14, 21, 27) , false),
                                                         new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(14, 21, 27) , QColor(14, 21, 27) , true),
                                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(222, 217, 226) , QColor(107, 242, 154) , false),
                                                         new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , false , icon , iconSize)};

    std::vector<AnimationPrototype*> selectedSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(28, 42, 53) , QColor(28, 42, 53) , true),
                                                          new AnimationPrototype(AnimationTool::Type::BorderColor , 	QColor(28, 42, 53) , QColor(28, 42, 53) , true),
                                                          new AnimationPrototype(AnimationTool::Type::TextColor , QColor(222, 217, 226) , QColor(07, 242, 154) , false) ,
                                                          new IconColorAnimationPrototype(QColor(235, 239, 235) , QColor(107, 242, 154) , true , std::move(icon) , iconSize)};

    return new C_SelectableButtonStyleHelper(nullptr , std::move(defaultSettings) , std::move(selectedSettings));
}

C_SelectableButtonStyleHelper* ButtonStyleRepository::selectableFriendListButton()
{
    return socialPageMenuButton(QPixmap(":/Icons/FriendListIcon2.jpg"));
}
C_SelectableButtonStyleHelper* ButtonStyleRepository::selectableRequestListButton()
{
    return socialPageMenuButton(QPixmap(":/Icons/RequestsIcon.png"));

}
C_SelectableButtonStyleHelper* ButtonStyleRepository::selectableAddFriendButton()
{
    return socialPageMenuButton(QPixmap(":/Icons/AddFriendIcon.png"));

}
C_SelectableButtonStyleHelper* ButtonStyleRepository::selectableBlockedListButton()
{
    return socialPageMenuButton(QPixmap(":/Icons/BlockedIcon.png"));

}


CustomizableButtonStyleHelper* ButtonStyleRepository::searchButton()
{
    std::vector<AnimationPrototype*> vec = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(14, 21, 27) , QColor(28, 42, 54) , true),
                                             new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(14, 21, 27) , QColor(28, 42, 54) , true),
                                             new AnimationPrototype(AnimationTool::Type::TextColor , QColor(220, 214, 247) , QColor(255, 255, 255) , true),
                                             new IconColorAnimationPrototype(QColor(220, 214, 247) , QColor(255, 255, 255) , false , QPixmap(":/Icons/SearchIcon.png") , QSize(32 , 32))};
    return new CustomizableButtonStyleHelper(nullptr , std::move(vec));
}


C_SelectableButtonStyleHelper* ButtonStyleRepository::pauseButton()
{
    QSize iconSize = { 32 , 32};
    QPixmap icon1 = QPixmap(":/Icons/PauseIcon.svg");
    QPixmap icon2 = QPixmap(":/Icons/PlayIcon.png");

    std::vector<AnimationPrototype*> defaultSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , false , std::move(icon1) , iconSize)};

    std::vector<AnimationPrototype*> selectedSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new AnimationPrototype(AnimationTool::Type::BorderColor , 	QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new IconColorAnimationPrototype(QColor(235, 239, 235) , QColor(107, 242, 154) , false , std::move(icon2) , iconSize)};

    return new C_SelectableButtonStyleHelper(nullptr , std::move(defaultSettings) , std::move(selectedSettings));
}
C_SelectableButtonStyleHelper* ButtonStyleRepository::muteButton()
{
    QSize iconSize = { 48 , 48};
    QPixmap icon2 = QPixmap(":/images/mute_icon_music.png");
    QPixmap icon1 = QPixmap(":/images/unmute_icon_music.png");

    std::vector<AnimationPrototype*> defaultSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new IconColorAnimationPrototype(QColor(222, 217, 226) , QColor(107, 242, 154) , false , std::move(icon1) , iconSize)};

    std::vector<AnimationPrototype*> selectedSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new AnimationPrototype(AnimationTool::Type::BorderColor , 	QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                          new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true) ,
                                                          new IconColorAnimationPrototype(QColor(235, 239, 235) , QColor(107, 242, 154) , true , std::move(icon2) , iconSize)};

    return new C_SelectableButtonStyleHelper(nullptr , std::move(defaultSettings) , std::move(selectedSettings));
}

C_SelectableButtonStyleHelper* ButtonStyleRepository::addFriendButton()
{
    QSize iconSize = { 48 , 48};
    QPixmap icon1 = QPixmap(":/Icons/AddFriendIcon.png");
    QPixmap icon2 = QPixmap(":/Icons/AcceptIcon.png");

    std::vector<AnimationPrototype*> defaultSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                                        new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                                        new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                                         new IconColorAnimationPrototype(QColor(235, 239, 235) , QColor(107, 242, 154) , true , std::move(icon1) , iconSize)};

    std::vector<AnimationPrototype*> selectedSettings = {new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(	14, 21, 27) , QColor(14, 21, 27) , true),
                                                          new AnimationPrototype(AnimationTool::Type::BorderColor , 	QColor(	14, 21, 27) , QColor(14, 21, 27) , true),
                                                          new AnimationPrototype(AnimationTool::Type::TextColor , QColor(	0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true) ,
                                                          new IconColorAnimationPrototype(QColor(107, 242, 154) , QColor(107, 242, 154) , true , std::move(icon2) , iconSize)};

    return new C_SelectableButtonStyleHelper(nullptr , std::move(defaultSettings) , std::move(selectedSettings));
}


CustomizableButtonStyleHelper* ButtonStyleRepository::removeBlockedUserButton()
{
    std::vector<AnimationPrototype*> vec = { new AnimationPrototype(AnimationTool::Type::BackgroundColor , 	QColor(35, 53, 67) , QColor(14, 21, 27) , false),
                                         new AnimationPrototype(AnimationTool::Type::BorderColor, QColor(35, 53, 67) , QColor(14, 21, 27) , true),
                                         new AnimationPrototype(AnimationTool::Type::TextColor , QColor(0, 0, 0 , 0) , QColor(0, 0, 0 , 0) , true),
                                         new IconColorAnimationPrototype(QColor("white") , QColor(235, 81, 96) , true , QPixmap(":/Icons/RemoveUserIcon.png") , QSize(32 , 32)) };

    return new CustomizableButtonStyleHelper(nullptr, std::move(vec));
}