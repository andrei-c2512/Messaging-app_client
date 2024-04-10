#include "ProfileView.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ImagePainter.h"

ProfileView::ProfileView(QWidget* parent)
    :QWidget(parent)
{
    QMargins noMargins = { 0 , 0 , 0 , 0};
    setContentsMargins(0 , 3 , 0 , 3);

    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pProfilePicture = new ProfilePicLabel;
    QPixmap pixmap = QPixmap("::/Images/Images/Icons/defaultPfpIcon.jpg");
    pixmap = pixmap.scaled(iconSize);

    ImagePainter painter = ImagePainter(this , std::move(pixmap));
    painter.setColor(StyleRepository::Base::textColor());

    pProfilePicture->setPixmap1(QPixmap::fromImage(painter.image()));

    pName = new QLabel;
    pName->setFont(StyleRepository::Base::standardFont());
    pName->setText("Username");

    pLayout->addWidget(pProfilePicture);
    pLayout->addWidget(pName);
    pLayout->addStretch(1);
}
void ProfileView::setProfilePicture(QPixmap pixmap){
    pixmap = pixmap.scaled(iconSize);

    ImagePainter painter = ImagePainter(this , std::move(pixmap));
    painter.setColor(StyleRepository::Base::textColor());

    pProfilePicture->setPixmap1(QPixmap::fromImage(painter.image()));

}
void ProfileView::setName(QString name) { pName->setText(std::move(name));}
void ProfileView::setNameVisible(bool visible) { pName->setVisible(visible);}
QString ProfileView::name() const { return pName->text();}
QPixmap ProfileView::pixmap() const { return pProfilePicture->pixmap();}
