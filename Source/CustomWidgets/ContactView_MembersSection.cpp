#include "ContactView_MembersSection.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ButtonStyleRepository.h"

GroupMemberOptions::GroupMemberOptions(QWidget* parent, Features flags) 
	:GroupMemberOptions(parent , nullptr, flags)
{}

GroupMemberOptions::GroupMemberOptions(QWidget* parent, CustomButtonStyleHelper* helper, Features flags) 
	: CustomButton(parent , helper)
{
	pMessage = pCall = pBlock = pFriendAction = pMute = pRemoveFromGroup = nullptr;
	setupUi(flags);
}

void GroupMemberOptions::setupUi(Features flags)
{
	pMenu = new QMenu;
	pMenu->setStyleSheet(StyleRepository::MenuStyle::qMenuStyleSheet());

	QFont menuFont = StyleRepository::MenuStyle::menuFont();
	pMessage = new QAction;
	pMessage->setText("Message");
	pMessage->setFont(menuFont);
	pMessage->setIcon(StyleRepository::MenuStyle::messagePixmap());

	pCall = new QAction;
	pCall->setText("Call");
	pCall->setFont(menuFont);
	pCall->setIcon(StyleRepository::MenuStyle::callPixmap());

	pBlock = new QAction;
	pBlock->setText("Block");
	pBlock->setFont(menuFont);
	pBlock->setIcon(StyleRepository::MenuStyle::blockPixmap());

	pMenu->addAction(pMessage);
	pMenu->addAction(pCall);
	pMenu->addAction(pBlock);

	if (flags & GroupMemberOptions::Features::Friend)
	{
		pFriendAction = new QAction;
		pFriendAction->setText("Add friend");
		pFriendAction->setFont(menuFont);
	}
	else
	{
		pFriendAction = new QAction;
		pFriendAction->setText("Unfriend");
		pFriendAction->setFont(menuFont);
	}
	pMenu->addAction(pFriendAction);

	if (flags & GroupMemberOptions::Features::YouAreAdmin)
	{
		pMute = new QAction;
		pMute->setText("Mute");
		pMute->setFont(menuFont);

		pRemoveFromGroup = new QAction;
		pRemoveFromGroup->setText("Remove");
		pRemoveFromGroup->setFont(menuFont);

		pMenu->addAction(pMute);
		pMenu->addAction(pRemoveFromGroup);
	}
	setMenu(pMenu);
	lastFeatures = flags;
}

void GroupMemberOptions::setFlags(Features flags)
{
	QFont menuFont = StyleRepository::MenuStyle::menuFont();

	//removing old features
	pMenu->removeAction(pFriendAction);
	pFriendAction->deleteLater();

	if (lastFeatures & GroupMemberOptions::Features::YouAreAdmin)
	{
		pMenu->removeAction(pMute);
		pMenu->removeAction(pRemoveFromGroup);

		pMute->deleteLater();
		pRemoveFromGroup->deleteLater();
	}

	//adding the requiered features
	if (flags & GroupMemberOptions::Features::Friend)
	{
		pFriendAction = new QAction;
		pFriendAction->setText("Add friend");
		pFriendAction->setFont(menuFont);
	}
	else
	{
		pFriendAction = new QAction;
		pFriendAction->setText("Unfriend");
		pFriendAction->setFont(menuFont);
	}
	pMenu->addAction(pFriendAction);

	if (flags & GroupMemberOptions::Features::YouAreAdmin)
	{
		pMute = new QAction;
		pMute->setText("Mute");
		pMute->setFont(menuFont);

		pRemoveFromGroup = new QAction;
		pRemoveFromGroup->setText("Remove");
		pRemoveFromGroup->setFont(menuFont);

		pMenu->addAction(pMute);
		pMenu->addAction(pRemoveFromGroup);
	}

	lastFeatures = flags;
}

ContactView_MembersSection::ContactView_MembersSection(QWidget* parent) : ContactView(parent)
{
	pAdminCrown = new QLabel;
	pAdminCrown->setPixmap(StyleRepository::Base::crownPixmap());
	pLayout->addWidget(pAdminCrown);
	pLayout->addStretch(1);
	pAdminCrown->setVisible(false);
	pOptions = nullptr;
}
void ContactView_MembersSection::setIsAdmin(bool admin) { pAdminCrown->setVisible(admin); }
void ContactView_MembersSection::attatchOptions(ServerInfoProcessor& processor, Chat& page, GroupMemberOptions::Features flags)
{
	if (pOptions == nullptr)
	{
		pOptions = new GroupMemberOptions(nullptr, ButtonStyleRepository::moreOptionsButton(), flags);
		pLayout->addWidget(pOptions);
	}
	else
		pOptions->setFlags(flags);
	pLayout->addWidget(pOptions);
}