#include "SubpageManager.h"

SubpageManager::SubpageManager(QWidget* parent , ServerInfoProcessor& infoProcessor0)
    :QStackedWidget(parent) , infoProcessor(infoProcessor0)
{
    subPage_list = { new ChatPage(nullptr , infoProcessor)};
    subPage_list.emplace_back(new SocialPage(nullptr, infoProcessor , static_cast<ChatPage&>(*subPage_list[0]) , *this));
    for(Page* page : subPage_list)
        QStackedWidget::addWidget(page);

    setCurrentIndex(int(Type::Social));
}
void SubpageManager::setPage(Type type){ setCurrentIndex(int(type)); }
ChatPage& SubpageManager::chatPage(){
    return *static_cast<ChatPage*>(subPage_list[int(Type::Chat)]);
}

