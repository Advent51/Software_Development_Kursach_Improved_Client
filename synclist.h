#ifndef SYNCLIST_H
#define SYNCLIST_H

#include <QDialog>
#include <QtSql>
#include <QTableWidget>
#define Path_to_DB "C:/TEST/SinicinDB.db"

namespace Ui {
class SyncList;
}

class SyncList : public QDialog
{
    Q_OBJECT

public:
    explicit SyncList(QWidget *parent = 0);
    ~SyncList();
    void ConnectDB();
    QSqlDatabase dbase;
    int user_id;
    void PaintTab();

private:
    Ui::SyncList *ui;

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

#endif // SYNCLIST_H
