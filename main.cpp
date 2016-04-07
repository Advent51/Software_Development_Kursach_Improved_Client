#include "mainwindow.h"
#include "logindialog.h"
#include "drivelist.h"
#include "synclist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("Sinicin Incorporated");//задание ветки реестра организации
    QApplication::setApplicationName("Kursach_Client");//задание ветки реестра программы

    /*Инициализируем классы*/
    MainWindow w;
    SyncList s;
    DriveList d;
    LoginDialog loginDialog;

    //loginDialog.setUsername( "Luca" ); // optional

    /*Связываем разные классы. Это необходимо для перехода по действию из одного окна(класса) в другое*/
    QObject::connect(&loginDialog, SIGNAL (acceptLogin2(QString,QString, int)), &w, SLOT (setUser_name(QString,QString,int)));
    QObject::connect(&w, SIGNAL (SignalToSyncData(int)), &s, SLOT (SetId(int)));
    QObject::connect(&w, SIGNAL (SignalToDriveData(int)), &d, SLOT (SetId(int)));

    loginDialog.show();

    return a.exec();
}
