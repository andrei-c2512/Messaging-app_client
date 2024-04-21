#pragma once
#include "ContactView.h"
#include <QFlags>
#include "CustomStyledWidgets/CustomButton.h"
#include "Chat.h"
#include "Network/ServerInfoProcessor.h"
class GroupMemberOptions : public CustomButton {
public:
	enum class Features {
		Null = 0 ,
		YouAreAdmin = 1 ,
		IsAdmin = 2,
		Friend = 4 ,
		Stranger = 8
	};
	Q_DECLARE_FLAGS( Options, Features)

public:
	GroupMemberOptions(QWidget* parent, Features flags);
	GroupMemberOptions(QWidget* parent , CustomButtonStyleHelper* helper , Features flags);
	void setFlags(Features flags);
	Features lastFeatures() const;

	QAction* pMessage;
	QAction* pCall;
	QAction* pBlock;
	//if he's your friend
	QAction* pFriendAction;
	//if you are an admin
	QAction* pMute;
	QAction* pRemoveFromGroup;
private:
	void setupUi(Features flags);
private:
	QMenu* pMenu;
	//the usual

	Features _lastFeatures;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GroupMemberOptions::Options)

class ContactView_MembersSection : public ContactView {
	Q_OBJECT
public:
	ContactView_MembersSection(QWidget* parent = nullptr);
	void setIsAdmin(bool admin);
	void attatchOptions(ServerInfoProcessor& processor, Chat& page, GroupMemberOptions::Features flags);
private:
	void connectOptionsInit(ServerInfoProcessor& processor, Chat& page, GroupMemberOptions::Features flags);
	void connectFluctuatingOptions(ServerInfoProcessor& processor, Chat& page, GroupMemberOptions::Features flags);
signals:
	void switchToPrivateChat(int contactId);
private:
	QLabel* pAdminCrown;
	GroupMemberOptions* pOptions;
};
