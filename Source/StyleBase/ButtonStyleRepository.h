
#ifndef BUTTONSTYLEREPOSITORY_H
#define BUTTONSTYLEREPOSITORY_H
#include "ButtonStyles/CustomizableButtonStyleHelper.h"
#include "ButtonStyles/C_SelectableButtonStyleHelper.h"
namespace ButtonStyleRepository{
    CustomizableButtonStyleHelper* loginButton();
    CustomizableButtonStyleHelper* onlyTextButton();
    CustomizableButtonStyleHelper* goBackColor_LeftArrow();
    CustomizableButtonStyleHelper* friendListButton();
    CustomizableButtonStyleHelper* addFriendsButton();
    CustomizableButtonStyleHelper* searchButton();
    CustomizableButtonStyleHelper* acceptButton();
    CustomizableButtonStyleHelper* rejectButton();
    CustomizableButtonStyleHelper* moreOptionsButton();
    CustomizableButtonStyleHelper* messageButton();
    CustomizableButtonStyleHelper* removeBlockedUserButton();
    CustomizableButtonStyleHelper* plusButton();

    C_SelectableButtonStyleHelper* socialPageMenuButton(QPixmap map);

    C_SelectableButtonStyleHelper* showPasswordButton();
    C_SelectableButtonStyleHelper* selectableFriendListButton();
    C_SelectableButtonStyleHelper* selectableRequestListButton();
    C_SelectableButtonStyleHelper* selectableAddFriendButton();
    C_SelectableButtonStyleHelper* selectableBlockedListButton();

    C_SelectableButtonStyleHelper* pauseButton();
    C_SelectableButtonStyleHelper* muteButton();

    C_SelectableButtonStyleHelper* addFriendButton();
};

#endif // BUTTONSTYLEREPOSITORY_H
