#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSettings>
#include <QMainWindow>
#include "QString"
#include <QtSql>

#define Path_to_DB "C:/TEST/SinicinDB.db"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString slic, slic2, stime1, stime2,usernameMain,passwordMain;
    bool lic,lic2,flag,flag2,snaka;
    int mtimes, mtimes2,user_idMain;
    QSettings keysettings;
    QSqlDatabase dbase;
    void saveSettings();
    void loadSettings();
    void Runne();
    void nook();
    void CreationFile();
    void LicenseSelect();
    void ConnectDB();
    void SyncBegin();
    QString GetDriveID(QString dir1);

private slots:
    void on_pushButton_clicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

signals:
    void SignalToSyncData(int id);
    void SignalToDriveData(int id);

public slots:
    void setUser_name(QString username, QString password, int user_id);

private:
    Ui::MainWindow *ui;
    QSettings *settings,*settingsIni;
};

#endif // MAINWINDOW_H
