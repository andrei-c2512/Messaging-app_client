#include "ContactInfo.h"
#include "Tools.h"

ContactInfo::ContactInfo(QObject* parent) : QObject(parent)
{}

ContactInfo::ContactInfo(QObject* parent, QString name, ContactStatus flags0, QString lastSeen)
    : QObject(parent), _flags(flags0), _name(std::move(name)), _lastSeen(std::move(lastSeen))
{}
void ContactInfo::setFlags(ContactStatus f) { _flags = f; }
void ContactInfo::addFlags(ContactStatus f) {
    if (f & Status::Online)
        emit statusChanged(true);
    _flags = _flags | f;
}
void ContactInfo::removeFlags(ContactStatus f) {
    if (f & Status::Online)
        emit statusChanged(false);
    _flags = _flags & ~f;
}

void ContactInfo::setName(QString name) { _name = std::move(name); }
void ContactInfo::setLastSeen(QString lastSeen) { _lastSeen = std::move(lastSeen); }
void ContactInfo::setFriendList(std::vector<int> friendList0) { _friendList = std::move(friendList0); }
void ContactInfo::setId(int id0) { _id = id0; }

QString ContactInfo::name() const { return _name; }
QString ContactInfo::lastSeen() const { return _lastSeen; }
const std::vector<int>& ContactInfo::friendList() const { return _friendList; }
int ContactInfo::id() const { return _id; }
ContactInfo::ContactStatus ContactInfo::flags() const { return _flags; }



bool ContactInfo::operator<(const ContactInfo& info) { return _id < info._id; }
bool ContactInfo::operator>(const ContactInfo& info) { return _id > info._id; }
bool ContactInfo::operator==(const ContactInfo& info) { return _id == info._id; }

std::vector<ContactInfo*> ContactInfo::subtractFromList(std::vector<ContactInfo*> list1, std::vector<ContactInfo*> list2)
{
    Tools::sort<ContactInfo*>(list1, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    Tools::sort<ContactInfo*>(list2, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });


    std::vector<ContactInfo*> newVec;
    for (ContactInfo* c1 : list1)
    {
        bool subtracted = false;
        for (ContactInfo* c2 : list2)
        {
            if (c1->id() == c2->id())
                subtracted = true;
        }
        if (subtracted == false)
            newVec.emplace_back(c1);
    }


    return newVec;
}
