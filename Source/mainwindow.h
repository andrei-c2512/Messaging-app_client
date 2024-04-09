
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BackgroundShiftOnMouseEffect.h"
#include "PageManager.h"
#include "Network/ServerInfoProcessor.h"


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
private:
    Ui::MainWindow *ui;
    BackgroundShiftOnMouseEffect* pEffect;
    PageManager* pPageManager;
    ServerInfoProcessor* pServerInfoProcessor;
};

#endif // MAINWINDOW_H
