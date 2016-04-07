#include "synclist.h"
#include "ui_synclist.h"
#include "QtSql"
#include "QDebug"
#include "mainwindow.h"
#include "logindialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QAbstractItemView>

int itemx,itemy;

SyncList::SyncList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncList)
{
    ui->setupUi(this);
    setModal( true );
    setWindowTitle( tr("Sync Data") );
    ui->pushButton_3->setDisabled(true);
    ui->tableWidget->insertColumn(0);
}

SyncList::~SyncList()
{
    delete ui;
}

void SyncList::SetId(int id)
{
    user_id=id;
    qDebug()<<"ID Recieved: "<<user_id;
    this->show();
    SyncList::ConnectDB();
}

void SyncList::ConnectDB()
{
    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(Path_to_DB);
    QFileInfo checkFile(Path_to_DB);

    if(checkFile.isFile())
    {
        if(dbase.isOpen())
        {
            qDebug()<<"Already connected to database file";
            SyncList::PaintTab();
        }
        else
        {
            if(dbase.open())
            {
                qDebug()<<"Connected to database file";
                SyncList::PaintTab();
            }
            else
            {
                qDebug() << "Что-то не так с соединением!";
                QMessageBox::critical(this, "Eror: Не удалось подключиться к базе данных", "Повторите попытку");
            }
        }
    }
    else
    {
        qDebug()<<"Database file does not exist";
    }
}

void SyncList::on_pushButton_clicked()//close
{
    close();
}

void SyncList::on_pushButton_2_clicked()//add
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://");
    if (filename=="") return;
    else
    {
        QSqlQuery query;
        query.prepare("INSERT INTO SyncData (FilePath,Owner_ID) "
                    "VALUES (:FilePath, :Owner_ID)");

            query.bindValue(":FilePath", filename);
            query.bindValue(":Owner_ID", user_id);
            query.exec();
        SyncList::PaintTab();
    }
}

void SyncList::PaintTab()//Paint
{
    while (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->removeRow(0);
    }
    QSqlQuery qry;
    if(qry.exec("SELECT FilePath FROM SyncData WHERE Owner_ID=\'"+QString::number(user_id) + "\'"))
    {
        QSqlRecord rec = qry.record();
        int i=0;
        while (qry.next())
        {
            ui->tableWidget->insertRow(i);
            QString FilePath1  = qry.value(rec.indexOf("FilePath")).toString();
            qDebug()<<"Line: "<<FilePath1;
            QTableWidgetItem * item = new QTableWidgetItem("test");
            item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
            item->setText(FilePath1);
            ui->tableWidget->setItem(i, 0, item);
            i++;
        }
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->connect(ui->tableWidget, SIGNAL (cellClicked(int, int)), this,SLOT (ButtonEnabler(int,int)));
        if (ui->tableWidget->rowCount()==0) ui->pushButton_3->hide();
        if (ui->tableWidget->rowCount()>0) ui->pushButton_3->show();
    }
}

void SyncList::ButtonEnabler(int x, int y)
{
    ui->pushButton_3->setEnabled(true);
    itemx = x;
    itemy = y;
}

void SyncList::on_pushButton_3_clicked()
{
    QSqlQuery qry;
    QTableWidgetItem* itm = ui->tableWidget->item(itemx,itemy);
    QString path = itm->text();
    qDebug()<<"Item selected to delete: "<<path;
    qry.exec( "DELETE FROM SyncData WHERE Owner_ID=\'"+QString::number(user_id) + "\' AND FilePath=\'" + path + "\'");
    SyncList::PaintTab();
}
