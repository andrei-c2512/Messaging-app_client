#include "Page.h"

Page::Page(QWidget* parent , ServerInfoProcessor& serverInfoProcessor0)
    :QWidget(parent) , serverInfoProcessor(serverInfoProcessor0)
{
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("Page");
}
