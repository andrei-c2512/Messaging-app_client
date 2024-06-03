
#include "mainwindow.h"

#include <QApplication>
#include "CustomStyle.h"
#include "StyleBase/StyleRepository.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StyleRepository::initializeVariables();
    MainWindow w;
    a.setStyle(new CustomStyle);

    w.show();
    qDebug() << QApplication::applicationDirPath();
    return a.exec();
}
