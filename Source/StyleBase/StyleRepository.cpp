#include "StyleRepository.h"


//color difference factor
static int fact = 20;
static bool qMenuStyleInit = false;
static bool blockPixmapInit = false;
static bool removePixmapInit = false;
static bool callPixmapInit = false;

static QString qMenuStyleStr;
static std::unique_ptr<QPixmap> _removePixmap;
static std::unique_ptr<QPixmap> _callPixmap;
static std::unique_ptr<QPixmap> _blockPixmap;

QSize pfpImageSize = { 32 , 32 };
static std::unique_ptr<QPixmap> _pfpPixmap;

void StyleRepository::initializeVariables()
{
    _removePixmap = std::make_unique<QPixmap>(QPixmap::fromImage(ImagePainter::paintImage(QPixmap(":/Images/Images/Icons/RemoveIcon.png"), QColor(215, 38, 61))));
    _callPixmap = std::make_unique<QPixmap>(QPixmap::fromImage(ImagePainter::paintImage(QPixmap(":/Images/Images/Icons/CallIcon.png"), "white")));
    _blockPixmap = std::make_unique<QPixmap>(QPixmap::fromImage(ImagePainter::paintImage(QPixmap(":/Images/Images/Icons/BlockIcon.png"), QColor(215, 38, 61))));
    _pfpPixmap = std::make_unique<QPixmap>(QPixmap::fromImage(ImagePainter::paintImage(QPixmap(":/Images/Images/Icons/defaultPfpIcon.jpg").scaled(pfpImageSize), "white")));
}

/*Base colors beggin */
QColor StyleRepository::Base::textColor() { return QColor(235, 239, 235); }
QColor StyleRepository::Base::backgroundColor(){ return QColor(32, 35, 37 , 120);}
QColor StyleRepository::Base::borderColor(){ return QColor(235, 239, 235);}
QFont  StyleRepository::Base::chatFont(){ return QFont("Times" , 15);}
QFont  StyleRepository::Base::standardFont() { return QFont("Times" , 22);}
QFont  StyleRepository::Base::headerFont() {
    QFont font("Times" , 30);
    font.setBold(true);
    return font;
}
QFont StyleRepository::Base::chatViewTitleFont() { 
    QFont f("Times", 12);
    f.setBold(true);
    return f;
}

QFont StyleRepository::Base::membersHeaderFont() {
    QFont font("Times", 16);
    font.setBold(true);
    return font; 
}

QFont StyleRepository::Base::chatViewMessageFont() { return QFont("Times" , 9);}
QFont StyleRepository::Base::informativeTextFont() { return QFont("Times" , 10);}
QFont StyleRepository::Base::mediumSizeButtonFont() { return QFont("Times" , 15);}
QFont StyleRepository::Base::optionSectionHeaderFont() { return QFont("Times" , 22);}

QColor StyleRepository::Base::hoverTextColor() { return QColor(255, 255, 255); }
QColor StyleRepository::Base::hoverBackgroundColor(){return QColor(9, 10, 11);}
QColor StyleRepository::Base::hoverBorderColor() { return QColor(255, 255, 255);};

QPixmap StyleRepository::Base::defaultPfpPixmap() { return *_pfpPixmap;  }

/*Base colors end */

/* QMenu related */

QString StyleRepository::MenuStyle::qMenuStyleSheet()
{
    if (qMenuStyleInit)
        return qMenuStyleStr;
    else
    {
        QFile file = QFile(":/styles/menuStyle.qss");
        if (file.exists() == false)
            qDebug() << "WHAT";
        if (file.open(QFile::ReadOnly) == false)
            qDebug() << file.error();

        qMenuStyleStr = file.readAll();
        qMenuStyleInit = true;

        file.close();
        return qMenuStyleStr;
    }
}

QPixmap StyleRepository::MenuStyle::callPixmap(){ return *_callPixmap;}
QPixmap StyleRepository::MenuStyle::removePixmap(){ return *_removePixmap;}
QPixmap StyleRepository::MenuStyle::blockPixmap(){ return *_blockPixmap;}

QFont StyleRepository::MenuStyle::menuFont() { return QFont("Tiems", 14); }


/*Add Button beggin */
QColor  StyleRepository::AddButton::borderColor(){ return QColor(22, 193, 114);}
QColor  StyleRepository::AddButton::iconColor(){ return QColor(22, 193, 114);}

//QColor StyleRepository::AddButton::hoverBorderColor(){ return QColor(179, 234, 113);}
QColor StyleRepository::AddButton::hoverBorderColor(){ return QColor(35, 231, 139);}
QColor StyleRepository::AddButton::hoverIconColor(){ return QColor(35, 231, 139);}

QColor StyleRepository::SelectableButton::selectedTextColor() { return QColor(34, 73, 231);}
QColor StyleRepository::SelectableButton::selectedBorderColor() { return QColor(34, 73, 231);}
/*Add Button end */

/*Edit Button beggin */
QColor StyleRepository::EditButton::ConfirmState::iconColor(){  return QColor(22, 193, 114);  }
QColor StyleRepository::EditButton::ConfirmState::borderColor(){  return QColor(22, 193, 114);   }
QColor StyleRepository::EditButton::ConfirmState::hoverIconColor() { return QColor(35, 231, 139); }
QColor StyleRepository::EditButton::ConfirmState::hoverBorderColor() { return QColor(35, 231, 139); }
QString StyleRepository::EditButton::ConfirmState::icon() { return QString(":/Images/Images/Icons/Checked.png");}

QColor StyleRepository::EditButton::StartEditState::iconColor() { return QColor(255, 162, 31);}
QColor StyleRepository::EditButton::StartEditState::borderColor() { return QColor(255, 162, 31);}
QColor StyleRepository::EditButton::StartEditState::hoverIconColor() { return QColor(255, 179, 71);}
QColor StyleRepository::EditButton::StartEditState::hoverBorderColor() { return QColor(255, 179, 71);}
QString StyleRepository::EditButton::StartEditState::icon() { return QString(":/Images/Images/Icons/EditLineIcon.png");}
/*Edit Button end */

/*Mute button(for music) beggin */
QString StyleRepository::MuteButton_Sound::muteIcon(){return QString(":/Images/Images/Icons/mute_icon_music.png");}
QColor StyleRepository::MuteButton_Sound::unselectedIconColor() { return QColor();}

QString StyleRepository::MuteButton_Sound::unmuteIcon() { return QString(":/Images/Images/Icons/unmute_icon_music.png");}
QColor StyleRepository::MuteButton_Sound::selectedColor(){ return QColor();}

int StyleRepository::MuteButton_Sound::iconSizeIncreasePercentage() { return 20;}
/*Mute button(for music) end */
