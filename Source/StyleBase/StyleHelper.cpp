#include "StyleHelper.h"

StyleHelper::StyleHelper(QWidget* widget)
    :pManagingWidget(widget)
{}

const QWidget* StyleHelper::managingWidget() const{ return pManagingWidget;}
void StyleHelper::setManagingWidget( QWidget* widget)
{
    pManagingWidget = widget;
    settings_setup();
}
void StyleHelper::settings_setup(){}
