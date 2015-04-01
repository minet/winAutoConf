#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;

    translator.load("translate_" + QLocale::system().name().section('_', 0, 0));

    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
