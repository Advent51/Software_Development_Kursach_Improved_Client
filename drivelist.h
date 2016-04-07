#ifndef DRIVELIST_H
#define DRIVELIST_H

#include <QDialog>
#include <QtSql>
#include <QTableWidget>
#include <QString>
#define Path_to_DB "C:/TEST/SinicinDB.db"

namespace Ui {
class DriveList;
}

class DriveList : public QDialog
{
    Q_OBJECT

public:
    explicit DriveList(QWidget *parent = 0);
    ~DriveList();
    void ConnectDB();
    QSqlDatabase dbase;
    int user_id;
    void PaintTab();
    QString GetDriveID(QString dir1);

private:
    Ui::DriveList *ui;

signals:
    void cellPressed(int x, int y);

public slots:
    void SetId(int id);
    void ButtonEnabler(int x,int y);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // DRIVELIST_H
