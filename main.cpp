#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "gestiondefichiers.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(ressources);
    MainWindow w;
    w.setStyle(QApplication::setStyle("Fusion"));
    w.show();

    return a.exec();

    return 0;
}
