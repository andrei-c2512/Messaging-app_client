#include "ContactView.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ImagePainter.h"
#include "StyleBase/ButtonStyleRepository.h"



ContactView::ContactView(QWidget* parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_Hover);

    QMargins noMargins = { 0 , 0 , 0 , 0};
    setContentsMargins(0 , 3 , 0 , 3);

    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pProfilePicture = new ProfilePicLabel;
    pProfilePicture->setSizePolicy(QSizePolicy(QSizePolicy::Minimum , QSizePolicy::Minimum));

    pProfilePicture->setPixmap1(StyleRepository::Base::defaultPfpPixmap());

    pName = new QLabel;
    pName->setFont(StyleRepository::Base::standardFont());
    pName->setText("Username");

    pLayout->addWidget(pProfilePicture);
    pLayout->addWidget(pName);

    pInfo = nullptr;
    setObjectName("ContactView");
}


void ContactView::setProfilePicture(QPixmap pixmap){
    pixmap = pixmap.scaled(iconSize);

    ImagePainter painter = ImagePainter(this , std::move(pixmap));
    painter.setColor(StyleRepository::Base::textColor());

    pProfilePicture->setPixmap1(QPixmap::fromImage(painter.image()));

}
void ContactView::onInfoDeletion() { pInfo = nullptr; }

void ContactView::setContactInfo(ContactInfo& info)
{
    if (pInfo)
    {
        disconnect(pInfo, &ContactInfo::removed, this, &ContactView::emitRemove);
        disconnect(pInfo, &ContactInfo::destroyed, this, &ContactView::onInfoDeletion);
    }

    pInfo = &info;
    pName->setText(pInfo->name());
    //if it's empty then it's nullInfo
    if (pInfo->name().isEmpty() == false)
    {
        connect(pInfo, &ContactInfo::removed, this, &ContactView::emitRemove);
        connect(pInfo, &ContactInfo::statusChanged, this, [=](bool online) {
            ProfilePicLabel::Status status = (online) ? ProfilePicLabel::Status::Online : ProfilePicLabel::Status::Offline;
            pProfilePicture->setStatus(status);
            });
        connect(pInfo, &ContactInfo::destroyed, this, &ContactView::onInfoDeletion);
    }
    auto flags = info.flags();
    if (flags & ContactInfo::Status::Online)
        pProfilePicture->setStatus(ProfilePicLabel::Status::Online);
    else
        pProfilePicture->setStatus(ProfilePicLabel::Status::Offline);
}

void ContactView::emitRemove(int id){ emit remove(id);}
void ContactView::setContactInfo(UserInfo& info)
{
    pName->setText(info.name());
}
void ContactView::setName(QString name) { pName->setText(std::move(name));}
void ContactView::setNameVisible(bool visible) { pName->setVisible(visible);}
QString ContactView::name() const { return pName->text();}
QPixmap ContactView::pixmap() const { return pProfilePicture->pixmap();}


bool ContactView::operator==(const ContactView& rhs)
{
    return pInfo->id() == rhs.pInfo->id();
}

void ContactView::setNameFont(const QFont& font)
{
    pName->setFont(font);
}

const ContactInfo* ContactView::contactInfo() const { return pInfo; }

void ContactView::setStatusVisibility(bool enable)
{
    pProfilePicture->setStatusEnabled(enable);
}
