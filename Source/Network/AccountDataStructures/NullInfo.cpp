#include "NullInfo.h"


NullInfo::NullInfo()
{
    pNullChat = new ChatInfo;
    pNullChat->setId(-1);
    pNullChat->setName("THIS SHOULD NOT SHOW UP");
    pNullMessage = new MessageInfo;
    pNullMessage->setText("DEFAULT MESSAGE: THIS SHOULD NOT SHOW UP");
    pNullMessage->setTimestamp(QDateTime(QDate(33, 4, 3), QTime(15, 33, 33)));
    pNullChat->setMessageHistory({ pNullMessage });

    pNullContact = new ContactInfo;
}
NullInfo::~NullInfo()
{
    pNullMessage->deleteLater();
    pNullChat->deleteLater();
    pNullContact->deleteLater();
}

MessageInfo& NullInfo::nullMessage() const noexcept { return *pNullMessage; }
ChatInfo& NullInfo::nullChat() const noexcept { return *pNullChat; }
ContactInfo& NullInfo::nullContact() const noexcept { return *pNullContact; }

NullInfo& NullInfo::instance()
{
    static NullInfo info;
    return info;
}
