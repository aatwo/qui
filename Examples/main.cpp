#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName( "aatwo" );
    a.setApplicationName( "Qui Examples" );
    a.setApplicationDisplayName( "Qui Examples" );
    a.setApplicationVersion( "0.0.1" );

    MainWindow w;
    w.show();

    return a.exec();
}
