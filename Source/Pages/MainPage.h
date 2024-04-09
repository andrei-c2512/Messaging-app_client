
#ifndef MAINPAGE_H
#define MAINPAGE_H
#include "Page.h"
#include <QHBoxLayout>
#include "CustomWidgets/OptionSection.h"
#include "Subpages/ChatPage.h"


class MainPage : public Page{
    Q_OBJECT
public:
    enum class SubPage{
      Chat,
      Friends
    };
public:
    MainPage(QWidget* parent , ServerInfoProcessor& processor);
private:
    void setupUi();
private:
    QHBoxLayout*        pLayout;
    OptionSectionFrame* pFrame;
    SubpageManager*     pManager;
};

#endif // MAINPAGE_H
