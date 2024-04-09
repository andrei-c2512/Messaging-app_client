
#ifndef LOADINGPAGE_H
#define LOADINGPAGE_H
#include "Page.h"
#include <QVBoxLayout>
#include "CustomStyledWidgets/LoadingAnimationWidget.h"
#include <QLabel>
#include <QTimer>


class LoadingPage : public Page{
    Q_OBJECT
public:
    LoadingPage(QWidget* parent , ServerInfoProcessor& pProcessor);
private:
    void setupUi();
private:
    QVBoxLayout*            pLayout;
    LoadingAnimationWidget* pLoadingAnim;
    QLabel*                 pLoadingMessage;
    QTimer* pLabelUpdateTimer;
    int _currentDotCount;
};

#endif // LOADINGPAGE_H
