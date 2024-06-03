QT  += core gui
QT += network
QT += multimedia
QT += websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BackgroundShiftOnMouseEffect.cpp \
    CustomStyle.cpp \
    CustomStyledWidgets/CustomButton.cpp \
    CustomStyledWidgets/LoadingAnimationWidget.cpp \
    CustomStyledWidgets/SelectableButton.cpp \
    CustomWidgets/ContactView.cpp \
    CustomWidgets/ContactView_MembersSection.cpp \
    CustomWidgets/ContactView_SocialPage.cpp \
    CustomWidgets/ContactView_UserSelector.cpp \
    CustomWidgets/CustomLineEdit.cpp \
    CustomWidgets/CustomTextEdit.cpp \
    CustomWidgets/MemeWidget.cpp \
    CustomWidgets/OptionMenu.cpp \
    CustomWidgets/OptionSection.cpp \
    CustomWidgets/ProfilePicLabel.cpp \
    CustomWidgets/ProfileView.cpp \
    CustomWidgets/UserSelectorWidget.cpp \
    CustomWidgets/SearchBar.cpp \
    CustomWidgets/SignInForm.cpp \
    CustomWidgets/WarningLabel.cpp \
    Network/ServerInfoProcessor.cpp \
    Network/UserInfo.cpp \
    PageManager.cpp \
    Pages/LoadingPage.cpp \
    Pages/LoginPage.cpp \
    Pages/MainPage.cpp \
    Pages/Page.cpp \
    Pages/RegisterPage.cpp \
    Pages/Subpages/ChatPage.cpp \
    Pages/Subpages/SocialPage.cpp \
    Pages/Subpages/SubpageManager.cpp \
    StyleBase/AnimationTool.cpp \
    StyleBase/ButtonStyleRepository.cpp \
    StyleBase/ButtonStyles/C_SelectableButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/ColorShiftButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/CustomButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/CustomizableButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/EditButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/MuteButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/SelectableButtonStyleHelper.cpp \
    StyleBase/ButtonStyles/ShowMoreButtonStyleHelper.cpp \
    StyleBase/ImagePainter.cpp \
    StyleBase/PaletteAdjuster.cpp \
    StyleBase/StyleHelper.cpp \
    StyleBase/StyleRepository.cpp \
    StyleBase/WindowStyleHelper.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BackgroundShiftOnMouseEffect.h \
    CustomStyle.h \
    CustomStyledWidgets/CustomButton.h \
    CustomStyledWidgets/LoadingAnimationWidget.h \
    CustomStyledWidgets/SelectableButton.h \
    CustomWidgets/ContactView.h \
    CustomWidgets/ContactView_MembersSection.h \
    CustomWidgets/ContactView_SocialPage.h \
    CustomWidgets/ContactView_UserSelector.h \
    CustomWidgets/CustomLineEdit.h \
    CustomWidgets/CustomTextEdit.h \
    CustomWidgets/MemeWidget.h \
    CustomWidgets/OptionMenu.h \
    CustomWidgets/OptionSection.h \
    CustomWidgets/ProfilePicLabel.h \
    CustomWidgets/ProfileView.h \
    CustomWidgets/SearchBar.h \
    CustomWidgets/SignInForm.h \
    CustomWidgets/WarningLabel.h \
    CustomWidgets/UserSelectorWidget.h \
    Network/ServerInfoProcessor.h \
    Network/UserInfo.h \
    PageManager.h \
    Pages/LoadingPage.h \
    Pages/LoginPage.h \
    Pages/MainPage.h \
    Pages/Page.h \
    Pages/RegisterPage.h \
    Pages/Subpages/ChatPage.h \
    Pages/Subpages/SocialPage.h \
    Pages/Subpages/SubpageManager.h \
    StyleBase/AnimationTool.h \
    StyleBase/ButtonStyleRepository.h \
    StyleBase/ButtonStyles/C_SelectableButtonStyleHelper.h \
    StyleBase/ButtonStyles/ColorShiftButtonStyleHelper.h \
    StyleBase/ButtonStyles/CustomButtonStyleHelper.h \
    StyleBase/ButtonStyles/CustomizableButtonStyleHelper.h \
    StyleBase/ButtonStyles/EditButtonStyleHelper.h \
    StyleBase/ButtonStyles/MuteButtonStyleHelper.h \
    StyleBase/ButtonStyles/SelectableButtonStyleHelper.h \
    StyleBase/ButtonStyles/ShowMoreButtonStyleHelper.h \
    StyleBase/ImagePainter.h \
    StyleBase/PaletteAdjuster.h \
    StyleBase/StyleHelper.h \
    StyleBase/StyleRepository.h \
    Tools.h \
    StyleBase/WindowStyleHelper.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
