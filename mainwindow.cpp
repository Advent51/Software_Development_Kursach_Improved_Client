/*ЭТО ГЛАВНОЕ ОКНО ИЗ КОТОРОГО ЗАПУСКАЮТСЯ ОСТАЛЬНЫЕ, КРОМЕ LOGIN/PASSWORD*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QFile>
#include "QIODevice"
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <QSysInfo>
#include <QFileDialog>
#include <QDataStream>
#include <QTextStream>
#include <Encryption.h>
#include <QActionGroup>
#include "synclist.h"
#include <QtSql>
#ifdef Q_OS_WIN
#include "windows.h"
#include <stdio.h>
#include <iostream>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings(this);
    QDir dirPath;
    QString folder1;
    folder1=dirPath.homePath();
    settingsIni = new QSettings(folder1+"/AppData/Local/Sinicin Inc/Kursach/settings.dll", QSettings::IniFormat, this);
    loadSettings();//загружаем настройки, сохранённые ранее (если они были)
}

MainWindow::~MainWindow()
{
    saveSettings();//перед закрытием сохраняем настройки. В них содержится сколько раз запускалась программа и флаг показания приветствия.
    delete ui;
}

void MainWindow::Runne()//Проверка на лицензию и благодарность =)
{
    if ((lic==true)&&(lic2==true)&&(flag==false))
    {
        QMessageBox::information(0, "Благодарность", "Спасибо за покупку лицензии!");
        flag=true;//Поблагодарить только один раз
    }

    if ((lic==true)&&(lic2==true)) flag2=true;//куплена
    else flag2=false;//не куплена

    if (flag2==false)
    {

        if ((mtimes>=3)||(mtimes2>=3))
        {QMessageBox::critical(this, "Eror: Триальный период закончился!", "Купите лицензию!"); nook(); if ((lic==false)||(lic2==false)) exit(1);}
        else {mtimes++; mtimes2++;}
    }
}

void MainWindow::saveSettings()//сохраняет данные (куплена лицензия/сколько раз запускалась/заблокирована или нет)
{
    slic="fgghrewlor"+QString::number(lic)+"jasdbvoiub3owjhbnv34sdf"; slic=stringEncrypt(slic, "kjzsfionjweh807348uhewghnui724");
    slic2="fgghrewlor"+QString::number(lic2)+"jasdbvoiub3owjhbnv34sdf"; slic2=stringEncrypt(slic2, "kjzsfionjweh807348uhewghnui724");
    stime1="ghrewlor"+QString::number(mtimes)+"jasdbvoiub3owjhbnv34sdf"; stime1=stringEncrypt(stime1, "jhasbvfijwbv762v3f478263f123");
    stime2="ghrewlor"+QString::number(mtimes2)+"jasdbvoiub3owjhbnv34sdf"; stime2=stringEncrypt(stime2, "jhasbvfijwbv762v3f478263f123");

    settings->setValue("License", slic);
    settings->setValue("Times", stime1);
    settings->setValue("Flag",flag);
    settingsIni->setValue("License", slic2);
    settingsIni->setValue("Times", stime2);
}

void MainWindow::loadSettings()//при запуске загружает предыдущие настройки
{
    QString zerrro="ghrewlor0jasdbvoiub3owjhbnv34sdf"; zerrro=stringEncrypt(zerrro, "jhasbvfijwbv762v3f478263f123");
    QString zerrr1="ghrewlor1jasdbvoiub3owjhbnv34sdf"; zerrr1=stringEncrypt(zerrr1, "jhasbvfijwbv762v3f478263f123");
    QString zerrr2="ghrewlor2jasdbvoiub3owjhbnv34sdf"; zerrr2=stringEncrypt(zerrr2, "jhasbvfijwbv762v3f478263f123");
    QString NOTzerrro="ghrewlor3jasdbvoiub3owjhbnv34sdf"; NOTzerrro=stringEncrypt(NOTzerrro, "jhasbvfijwbv762v3f478263f123");
    QString szerrro="fgghrewlor0jasdbvoiub3owjhbnv34sdf"; szerrro=stringEncrypt(szerrro, "kjzsfionjweh807348uhewghnui724");
    QString szerrr1="fgghrewlor0jasdbvoiub3owjhbnv34sdf"; szerrr1=stringEncrypt(szerrr1, "kjzsfionjweh807348uhewghnui724");
    slic=(settings->value("License", szerrro).toString());
    slic2=(settingsIni->value("License", szerrr1).toString());
    flag=(settings->value("Flag",0).toBool());
    stime1=(settings->value("Times", zerrro).toString());
    stime2=(settingsIni->value("Times", zerrro).toString());

    //int x= QString::compare(stime1, zerrro); QString brrr=QString::number(x); QMessageBox::information(0, tr("Debug"), brrr);

    if (((QString::compare(stime1,zerrro))==0)||((QString::compare(stime1,zerrr1))==0)||((QString::compare(stime2,zerrr2))==0)||((QString::compare(stime1,NOTzerrro))==0))
    { stime1=stime1;}
    else stime1=NOTzerrro;
    if (((QString::compare(stime2,zerrro))==0)||((QString::compare(stime2,zerrr1))==0)||((QString::compare(stime2,zerrr2))==0)||((QString::compare(stime2,NOTzerrro))==0))
    { stime2=stime2;}
    else stime2=NOTzerrro;
    if ((QString::compare(slic, szerrro)!=0)||(QString::compare(slic, szerrr1)!=0)) slic=slic;
    else slic=szerrro;
    if ((QString::compare(slic2, szerrro)!=0)||(QString::compare(slic2, szerrr1)!=0)) slic2=slic2;
    else slic=szerrro;

    slic=stringDecrypt(slic, "kjzsfionjweh807348uhewghnui724"); slic=slic.mid(10,1); lic=slic.toInt();
    slic2=stringDecrypt(slic2, "kjzsfionjweh807348uhewghnui724"); slic2=slic2.mid(10,1); lic2=slic2.toInt();
    stime1=stringDecrypt(stime1, "jhasbvfijwbv762v3f478263f123"); stime1=stime1.mid(8,1); mtimes=stime1.toInt();
    stime2=stringDecrypt(stime2, "jhasbvfijwbv762v3f478263f123"); stime2=stime2.mid(8,1); mtimes2=stime2.toInt();
}

void MainWindow::nook()//окошко надоедания "купи или проваливай"
{
    QMessageBox msgBox;
    msgBox.setText("The trial period passed.");
    msgBox.setInformativeText("Do you want to save your key or select the license?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Open);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Save:
        // Save was clicked
    {CreationFile();
        break;}
    case QMessageBox::Open:
    {LicenseSelect();
        break;}
    default:
        // should never be reached
        break;
    }
}

void MainWindow::on_pushButton_clicked()//обработка кнопки выхода
{
    close();
}

void MainWindow::CreationFile()//СОЗДАНИЕ КЛЮЧ-ФАЙЛА
{
    QString UID;
    QString folder2;
    folder2=QFileDialog::getSaveFileName(this, tr("Выберете место сохранения ключ-данных"), "C://", tr("Key file (*.key)"));
    if (folder2=="") return;
    QFile keyFile(folder2);
    FILE *lsofFile_p = popen("wmic path win32_physicalmedia get SerialNumber", "r");//вытаскиваем серийник материнки
    QTextStream data1(lsofFile_p);
    QString text1= data1.readAll();
    pclose(lsofFile_p);
    UID=text1;

    if(keyFile.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text)&&(UID!=""))//Копирование текста выбранного файла в новый + нужно добавить соль
    {
        QTextStream data2(&keyFile);
        QString text2;
        data2.setCodec("UTF-8");

        text2=UID;
        text2=stringEncrypt(text2,"65sdfgh98sdf1sdf4");//шифруем

        data2<<text2;
        keyFile.close();
        QMessageBox::information(0,"Success", "Ключ-файл был успешно создан!");
    }
    else QMessageBox::critical(0,"Проблема", "Ключ-файл не был создан!");
}

void MainWindow::on_action_triggered()
{
    CreationFile();
}

void MainWindow::LicenseSelect()//запускается, когда выбираем файл-лицензию, полученную с сервера.
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "License file (*.lic)");
    if (filename=="") return;
    QFile read_file;
    read_file.setFileName(filename);
    QString text1;
    if(read_file.open(QIODevice::ReadOnly|QIODevice::Text)) //Считываем лицензию
    {
        QTextStream out(&read_file);
        out.setCodec("UTF-8");          // Установка кодека
        text1 = out.readAll();       // Считываем весь файл в QString
        read_file.close();
    }

    text1=stringDecrypt(text1,"456789ApOiNtR55$%^&*&^%$#");//дешифруем то, что получили
    FILE *lsofFile_p = popen("wmic path win32_physicalmedia get SerialNumber", "r");//вытаскиваем серийник материнки
    QTextStream data1(lsofFile_p);
    QString text2= data1.readAll();
    pclose(lsofFile_p);

    if (text1==text2) {lic=lic2=true; QMessageBox::information(0, "License","Thank you for purchaising our product!");}
    else {QMessageBox::warning(0, "Oh, Pirate!","Please, buy your own license!");}
}

void MainWindow::on_action_2_triggered()
{
    LicenseSelect();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Kursach Client"),tr("<h2>Created by Nikita Sinicin SSAU</h2>"));
}

void MainWindow::setUser_name(QString username, QString password, int user_id)
{
    usernameMain = username;
    passwordMain = password;
    user_idMain = user_id;
    qDebug()<<"Username: "<<usernameMain;
    qDebug()<<"Password: "<<passwordMain;
    qDebug()<<"User ID: "<<user_idMain;
    this->Runne();
    this->show();
}

void MainWindow::on_pushButton_3_clicked()//Куда синхронизировать
{
    emit SignalToDriveData(user_idMain);
}

void MainWindow::on_pushButton_5_clicked()//список файлов на синхронизацию
{
    emit SignalToSyncData(user_idMain);
}

QString MainWindow::GetDriveID(QString dir1)
{
    QString cutted_dir_name = dir1;
    cutted_dir_name = cutted_dir_name.mid(0,3);
    QString text2="";
    char VolumeNameBuffer[100];
    char FileSystemNameBuffer[100];
    unsigned long VolumeSerialNumber;
    LPCSTR pather = cutted_dir_name.toStdString().c_str();

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

        //qDebug()<<"Volume name: "<<text1;
        //qDebug()<<"Serial ID: "<<text2;
        //qDebug()<<"File system: "<<text3;
    }
    return text2;
}

void MainWindow::SyncBegin()
{
    QSqlQuery qry;
    if(qry.exec("SELECT FilePath, Changed FROM SyncData WHERE Owner_ID=\'"+QString::number(user_idMain) + "\'"))
    {
        QSqlRecord rec = qry.record();

        while (qry.next())
        {
            QString filePath  = qry.value(rec.indexOf("FilePath")).toString();
            qDebug()<<"Path of file from basedata: "<<filePath;
            QDateTime fileDate = qry.value(rec.indexOf("Changed")).toDateTime();
            qDebug()<<"Date of file from basedata: "<<fileDate;

            //БЛОК ПРОВЕРКИ НЕОБХОДИМОСТИ СИНХРОНИЗАЦИИ

            QFileInfo checkFile(filePath);

            if(checkFile.isFile())//проверка существования файла
            {
                qDebug()<<"File exists (which need to be copied)";

                QFileInfo obFileInfo(filePath);
                QDateTime obStamp = obFileInfo.lastModified();
                qDebug()<<"Date of the real file: "<<obStamp;

                QFile file_to_copy;

                file_to_copy.setFileName(filePath);

                QSqlQuery qry1;
                if(qry1.exec("SELECT Drive_name, Serial FROM Drives WHERE User_ID=\'"+QString::number(user_idMain) + "\'"))
                {
                    QSqlRecord rec2 = qry1.record();

                    while (qry1.next())
                    {
                        QString filePath1  = qry1.value(rec2.indexOf("Drive_name")).toString();
                        //qDebug()<<"Path to drive: "<<filePath1;
                        QString Serial1  = qry1.value(rec2.indexOf("Serial")).toString();
                        //qDebug()<<"Drive's serial: "<<Serial1;

                        filePath1=filePath1+"/"+filePath.section('/', -1);//добавляем имя файла

                        QString drive_UID = MainWindow::GetDriveID(filePath1);
                        //qDebug()<<"Drive's UID to sync: "<<drive_UID;

                        if (drive_UID==Serial1)//Проверка на подмену носителя
                        {
                            QFileInfo checkFile2(filePath1);
                            if(checkFile2.isFile())
                            {
                                qDebug()<<"File exists";
                                QFileInfo obFileInfo2(filePath1);
                                QDateTime obStamp2 = obFileInfo2.lastModified();
                                if (obStamp2<obStamp)//Проверка на время последнего изменения
                                {
                                    file_to_copy.copy(filePath1);
                                    qDebug()<<"File copied succesfully to: "<<filePath1;

                                    QSqlQuery query;
                                    query.prepare("UPDATE SyncData SET Changed = :Changed WHERE FilePath = :FilePath AND Owner_ID = :Owner_ID;");

                                    QDateTime new_time = new_time.currentDateTime();
                                    qDebug()<<"NEW TIME: "<<new_time;
                                    query.bindValue(":Changed", new_time);
                                    query.bindValue(":FilePath", filePath);
                                    query.bindValue(":Owner_ID", user_idMain);
                                    query.exec();
                                }
                                else qDebug()<<"File is modern";
                            }
                            else
                            {
                                qDebug()<<"File does not exists";
                                file_to_copy.copy(filePath1);
                                qDebug()<<"File copied succesfully to: "<<filePath1;

                                QSqlQuery query;
                                query.prepare("UPDATE SyncData SET Changed = :Changed WHERE FilePath = :FilePath AND Owner_ID = :Owner_ID;");

                                QDateTime new_time = new_time.currentDateTime();
                                qDebug()<<"NEW TIME: "<<new_time;
                                query.bindValue(":Changed", new_time);
                                query.bindValue(":FilePath", filePath);
                                query.bindValue(":Owner_ID", user_idMain);
                                query.exec();
                            }
                        }
                        else
                        {
                            qDebug()<<"Drive had been changed! Incorrect drive:"<<filePath1<<"With Serial:"<<Serial1;
                        }
                    }
                }
            }
            else
            {
                qDebug() << "Файл был удалён из вне";
                QMessageBox::critical(this, "Eror: Не удалось найти файл", "Повторите попытку");
            }
        }
    }
}

void MainWindow::ConnectDB()
{
    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(Path_to_DB);
    QFileInfo checkFile(Path_to_DB);

    if(checkFile.isFile())
    {
        if(dbase.isOpen())
        {
            qDebug()<<"Already connected to database file";
            MainWindow::SyncBegin();
            dbase.close();
        }
        else
        {
            if(dbase.open())
            {
                qDebug()<<"Connected to database file";
                MainWindow::SyncBegin();
                dbase.close();
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

void MainWindow::on_pushButton_2_clicked()//синхронизировать
{
    MainWindow::ConnectDB();
}
