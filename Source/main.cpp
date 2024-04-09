
#include "mainwindow.h"

#include <QApplication>
#include "CustomStyle.h"
#include "StyleBase/StyleRepository.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setStyle(new CustomStyle);
    StyleRepository::initializeVariables();
    w.show();
    return a.exec();
}
