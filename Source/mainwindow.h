
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BackgroundShiftOnMouseEffect.h"
#include "PageManager.h"
#include "Network/ServerInfoProcessor.h"
#include "CustomWidgets/UserSelectorWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;
    bool eventFilter(QObject* obj, QEvent* ev) override;
private:
    Ui::MainWindow *ui;
    BackgroundShiftOnMouseEffect* pEffect;
    PageManager* pPageManager;
    ServerInfoProcessor* pServerInfoProcessor;
    UserSelectorWidget* pSelectorWidget;

};

#endif // MAINWINDOW_H
