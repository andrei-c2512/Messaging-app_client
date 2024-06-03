#pragma once
#include <QFlags>
#include <QObject>

class ContactInfo : public QObject {
    Q_OBJECT
public:
    enum class Status {
        Null = 0,
        Online = 1,
        HasRequest = 2,
        IsBlocked = 4,
        HasBlockedYou = 8,
        Friend = 16
    };
    Q_DECLARE_FLAGS(ContactStatus, Status)
public:
    ContactInfo(QObject* parent = nullptr);
    ContactInfo(QObject* parent, QString name, ContactStatus flags, QString lastSeen);
    void setFlags(ContactStatus f);
    void addFlags(ContactStatus f);
    void removeFlags(ContactStatus f);
    void setName(QString name);
    void setLastSeen(QString lastSeen);
    void setFriendList(std::vector<int> friendList0);
    void setId(int id);


    QString name() const;
    QString lastSeen() const;
    const std::vector<int>& friendList() const;
    int id() const;
    ContactStatus flags() const;


    bool operator<(const ContactInfo& info);
    bool operator>(const ContactInfo& info);
    bool operator==(const ContactInfo& info);

    static std::vector<ContactInfo*> subtractFromList(std::vector<ContactInfo*> list1, std::vector<ContactInfo*> list2);

signals:
    void removed(int id);
    //void moved(int id);
    //true for blocking , false for unblocking
    void gotBlocked(bool b);
    void blockedYou(bool b);
    void statusChanged(bool online);
private:
    int _id;
    QString _name;
    QString _lastSeen;
    ContactStatus _flags;

    std::vector<int> _friendList;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ContactInfo::ContactStatus)
