
#ifndef COLORREPOSITORY_H
#define COLORREPOSITORY_H
#include <QColor>
#include <QFont>
#include <QImage>
#include <QFile>
#include <qdebug.h>
#include "StyleBase/ImagePainter.h"

namespace StyleRepository{
    //this function is for initializing some pixmaps. I can't initialize them at compile time , because I can't use a QPixmap constructor before a QApp si made
    void initializeVariables();
    namespace Base{
    QColor textColor();
    QColor backgroundColor();
    QColor borderColor();
    //the font used on buttons or QTextEdit
    QFont  standardFont();
    //font for a title
    QFont  headerFont();
    //the font used in a chat room
    QFont chatUserFont();
    QFont chatFont();
    //font used for warnings , informative messages from the app etc
    QFont informativeTextFont();
    QFont chatViewTitleFont();
    QFont chatViewMessageFont();
    QFont mediumSizeButtonFont();
    QFont optionSectionHeaderFont();
    QFont membersHeaderFont();

    QColor hoverTextColor();
    QColor hoverBackgroundColor();
    QColor hoverBorderColor();

    QPixmap defaultPfpPixmap();
    QPixmap crownPixmap();

    QString qSliderStyle();
    }
    namespace MenuStyle {
        QString qMenuStyleSheet();
        QPixmap callPixmap();
        QPixmap removePixmap();
        QPixmap blockPixmap();
        QPixmap messagePixmap();
        QFont menuFont();
    }
    namespace ToolBar {
        QPixmap leavePixmap();
        QPixmap addPixmap();
    }
    namespace AddButton{
    QColor backgroundColor();
    QColor borderColor();
    QColor iconColor();

    QColor hoverBackgroundColor();
    QColor hoverBorderColor();
    QColor hoverIconColor();
    }
    /* only introducing selected color , as we will use the base colors for default and hover */
    /* if you want IconColoring , reimplement SelectableButtonStyleHelper yourself */
    namespace SelectableButton{
    QColor selectedTextColor();
    QColor selectedBorderColor();
    }

    namespace EditButton{
    namespace ConfirmState
    {
    QColor iconColor();
    QColor borderColor();
    QColor hoverIconColor();
    QColor hoverBorderColor();
    QString icon();
    }
    namespace StartEditState
    {
    QColor iconColor();
    QColor borderColor();
    QColor hoverIconColor();
    QColor hoverBorderColor();
    QString icon();
    }
    }
    namespace MuteButton_Sound{
    QString muteIcon();
    QColor unselectedIconColor();

    QString unmuteIcon();
    QColor selectedColor();

    int iconSizeIncreasePercentage();
    }
}

namespace ButtonStyleRepository{

}
#endif // COLORREPOSITORY_H
