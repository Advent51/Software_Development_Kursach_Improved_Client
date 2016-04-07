/*ЭТОТ КЛАСС ВЫПОЛНЯЕТ ВСЮ РАБОТУ С ВНЕШНИМИ НОСИТЕЛЯМИ*/
#include "drivelist.h"
#include "ui_drivelist.h"
#include "QtSql"
#include "QDebug"
#include "mainwindow.h"
#include "logindialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QAbstractItemView>
#include <windows.h>

int itemox,itemoy;//глобальные параметры, используются в выборе выбранной ячейки из таблицы
//конструктор
DriveList::DriveList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DriveList)
{
    ui->setupUi(this);//подключаем интерфейс
    setModal( true );//устанавливаем модальность окна
    setWindowTitle( tr("Sync Data") );//устанавливаем имя окна
    ui->pushButton_3->setDisabled(true);//отключаем кнопку удаления. Она включится как только будет выбрана хоть одна ячейка, которую можно будет удалить
    ui->tableWidget->insertColumn(0);//в таблицу вставляем 2 колонки
    ui->tableWidget->insertColumn(1);
}
//деструктор
DriveList::~DriveList()
{
    delete ui;
}

QString DriveList::GetDriveID(QString dir1)//получаем Serial номер устройства хранения
{
    QString text2="";
    char VolumeNameBuffer[100];
    char FileSystemNameBuffer[100];
    unsigned long VolumeSerialNumber;
    LPCSTR pather = dir1.toStdString().c_str();

    BOOL GetVolumeInformationFlag = GetVolumeInformationA(
                pather,
                VolumeNameBuffer,
                100,
                &VolumeSerialNumber,
                NULL, //&MaximumComponentLength,
                NULL, //&FileSystemFlags,
                FileSystemNameBuffer,
                100
                );

    if(GetVolumeInformationFlag != 0)
    {
        QTextStream data1(VolumeNameBuffer);//Volume Name
        QString text1= data1.readAll();
        text2 = QString::number(VolumeSerialNumber);//Volume Serial Number
        QTextStream data3(FileSystemNameBuffer);//File System
        QString text3 = data3.readAll();

        qDebug()<<"Path: "<<text1;
        qDebug()<<"Serial ID: "<<text2;
        qDebug()<<"File system: "<<text3;
    }
    return text2;
}

void DriveList::SetId(int id)//получаем и устанавливаем ID авторизовавшегося пользователя
{
    user_id=id;
    qDebug()<<"ID Recieved: "<<user_id;
    this->show();//показываем это окно
    DriveList::ConnectDB();
}

void DriveList::ConnectDB()//подключаем базу данных
{
    dbase = QSqlDatabase::addDatabase("QSQLITE");//выбираем тип базы данных
    dbase.setDatabaseName(Path_to_DB);//прописываем путь до неё. Определена дефайном
    QFileInfo checkFile(Path_to_DB);//проверяем файл базы данных

    if(checkFile.isFile())//если существет
    {
        if(dbase.isOpen())//проверяем открыто ли подключение к базе данных
        {
            qDebug()<<"Already connected to database file";
            DriveList::PaintTab();//заполняем таблицу данными
        }
        else
        {
            if(dbase.open())//если открылась база данных
            {
                qDebug()<<"Connected to database file";
                DriveList::PaintTab();
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

void DriveList::on_pushButton_clicked()//close
{
    close();
}

void DriveList::on_pushButton_2_clicked()//add
{
    //выбираем путь и записываем в dir_name
    QString dir_name=QFileDialog::getExistingDirectory(this, tr("Choose Directory to sync"), "C://", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir_name=="") return;//если не выбрали, то не продолжаем
    else
    {
        qDebug()<<"Directory name: : "<<dir_name;
        QString cutted_dir_name = dir_name;
        cutted_dir_name = cutted_dir_name.mid(0,3);//обрезаем путь до обозначения самого носителя C:/Example/123 -> C:/ Это нужно, чтобы потом узнать Serial носителя
        qDebug()<<"Cutted directory name: : "<<cutted_dir_name;
        QString Serial = DriveList::GetDriveID(cutted_dir_name);

        QSqlQuery query;//создаём очередь запросов к бд
        //вставка новых данных в таблицу носителей
        query.prepare("INSERT INTO Drives (User_ID,Drive_name,Serial) "
                    "VALUES (:User_ID, :Drive_name, :Serial)");

            query.bindValue(":User_ID", user_id);
            query.bindValue(":Drive_name", dir_name);
            query.bindValue(":Serial", Serial);
            query.exec();
        DriveList::PaintTab();
    }
}

void DriveList::PaintTab()//Заполняем таблицу
{
    while (ui->tableWidget->rowCount() > 0)//если таблица уже была заполнена, то удаляем всё что в ней было, чтобы заполнить по новой
    {
        ui->tableWidget->removeRow(0);
    }
    QSqlQuery qry;
    if(qry.exec("SELECT Drive_name, Serial FROM Drives WHERE User_ID=\'"+QString::number(user_id) + "\'"))
    {
        QSqlRecord rec = qry.record();
        int i=0;
        while (qry.next())//пока можем переходить к следущей записи базы данных
        {
            ui->tableWidget->insertRow(i);//вставляем строку
            QString FilePath1  = qry.value(rec.indexOf("Drive_name")).toString();//вытаскиваем из бд значение и записываем в переменную
            QString Serial1 = qry.value(rec.indexOf("Serial")).toString();//вытаскиваем и записываем
            qDebug()<<"Drive: "<<FilePath1;
            qDebug()<<"Serial: "<<Serial1;

            QTableWidgetItem * item = new QTableWidgetItem("test");//создаём итем. Только итемами можно заполнить таблицу.
            item->setFlags(item->flags() &  ~Qt::ItemIsEditable);//запрещаем изменять строки
            item->setText(FilePath1);//задаём текст строки
            ui->tableWidget->setItem(i, 0, item);//устанавливаем итем в таблицу на i-строку и нулевой столбец

            QTableWidgetItem * item1 = new QTableWidgetItem("test1");//повторяем тоже самое для других данных вставляя во 1 столбец
            item1->setFlags(item1->flags() &  ~Qt::ItemIsEditable);
            item1->setText(Serial1);
            ui->tableWidget->setItem(i, 1, item1);

            i++;
        }
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//выделение только целыми строками в таблице
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//выделять только по одному элементу за раз
        ui->tableWidget->resizeColumnsToContents();//форматируем таблицу под размер данных
        ui->tableWidget->resizeRowsToContents();//форматируем таблицу под размер данных
        if (ui->tableWidget->rowCount()==0) ui->pushButton_3->hide();
        if (ui->tableWidget->rowCount()>0) ui->pushButton_3->show();

        //Создаём сигнал. Если была выбрана ячейка - разблокирем кнопку удаления.
        //Она была заблокирована, так как не было ничего выделено.
        //Если что-то выделилось, то выделение не пропадёт, лишь изменится что было выделено.
        //Таким образом кнопке удалить будет всегда что удалить.
        ui->tableWidget->connect(ui->tableWidget, SIGNAL (cellClicked(int, int)), this,SLOT (ButtonEnabler(int,int)));
    }
}

void DriveList::ButtonEnabler(int x, int y)//включаем кнопку
{
    ui->pushButton_3->setEnabled(true);//включаем кнопку
    itemox = x;//записываем какую строку выбрали
    itemoy = y;//какой столбец
}

void DriveList::on_pushButton_3_clicked()//deleting
{
    QSqlQuery qry;
    QTableWidgetItem* itm = ui->tableWidget->item(itemox,0);//выбираем выбранный ранее итем
    QString path = itm->text();//нужно для поиска удаляемой строки в бд
    qDebug()<<"Item selected to delete: "<<path;
    //Выполняем SQL запрос к базе данных с целью удалить выбранную строку.
    //Drives - таблица, хранящая все носители.
    //Drive_name - путь до папки/носителя.
    qry.exec( "DELETE FROM Drives WHERE User_ID=\'"+QString::number(user_id) + "\' AND Drive_name=\'" + path + "\'");
    DriveList::PaintTab();//перерисовываем таблицу
}
