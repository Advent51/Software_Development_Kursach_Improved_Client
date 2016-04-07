/*ЭТОТ КЛАСС РЕАЛИЗУЕТ LOGIN/PASSWORD AUTHORIZATION И ЗАУСКАЕТСЯ ПЕРВЫМ ПОСЛЕ MAIN*/
#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setUpGUI();
    setWindowTitle( tr("User Login") );
    setModal( true );
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setUpGUI()
{
    // initialize the username combo box so that it is editable
    ui->comboUsername->setEditable( true );
    // initialize the password field so that it does not echo
    // characters
    ui->editPassword->setEchoMode( QLineEdit::Password );

    // initialize the labels
    ui->labelUsername->setText( tr( "Username" ) );
    ui->labelUsername->setBuddy( ui->comboUsername );
    ui->labelPassword->setText( tr( "Password" ) );
    ui->labelPassword->setBuddy( ui->editPassword );

    // initialize buttons
    ui->buttons->button( QDialogButtonBox::Ok )->setText( tr("Login") );
    ui->buttons->button( QDialogButtonBox::Cancel )->setText( tr("Abort") );

    // connects slots
    connect( ui->buttons->button( QDialogButtonBox::Cancel ),
             SIGNAL (clicked()),
             this,
             SLOT (Closing())
             );

    connect( ui->buttons->button( QDialogButtonBox::Ok ),
             SIGNAL (clicked()),
             this,
             SLOT (DataBaseConnection()));
}

void LoginDialog::setUsername(QString username)
{
    bool found = false;
    for( int i = 0; i < ui->comboUsername->count() && ! found ; i++ )
        if( ui->comboUsername->itemText( i ) == username ){
            ui->comboUsername->setCurrentIndex( i );
            found = true;
        }

    if( ! found ){
        int index = ui->comboUsername->count();
        qDebug() << "Select username " << index;
        ui->comboUsername->addItem( username );

        ui->comboUsername->setCurrentIndex( index );
    }

    // place the focus on the password field
    ui->editPassword->setFocus();
}

void LoginDialog::setPassword(QString password)
{
    ui->editPassword->setText( password );
}

void LoginDialog::setUsernamesList(const QStringList usernames)
{
    ui->comboUsername->addItems( usernames );
}

void LoginDialog::Closing()
{
    this->close();
}

void LoginDialog::DataBaseConnection()
{
    QString username = ui->comboUsername->currentText();
    QString password = ui->editPassword->text();
    //int user_id;

    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(Path_to_DB);
    QFileInfo checkFile(Path_to_DB);

    if(checkFile.isFile())
    {
        if(dbase.isOpen())
        {
            qDebug()<<"Already connected to database file";
            QSqlQuery qry;
            if(qry.exec("SELECT Username, Password, user_id FROM Login WHERE Username=\'"+username + "\' AND Password=\'" + password + "\'"))
            {
                if (qry.next())
                {
                    QSqlRecord rec = qry.record();

                    //nNumber  = query.value(rec.indexOf("number")).toInt();
                    QString username1  = qry.value(rec.indexOf("Username")).toString();
                    QString password1 = qry.value(rec.indexOf("Password")).toString();
                    int user_id1 = qry.value(rec.indexOf("user_id")).toInt();

                    //логин/пароль сошлись = авторизован
                    emit acceptLogin2( username1, // current username
                                       password1, // current password
                                       user_id1);
                    dbase.close();
                    close();
                }
                else QMessageBox::critical(this, "Eror: Неверный логин/пароль", "Повторите попытку");
            }
        }
        else
        {
            if(dbase.open())
            {
                qDebug()<<"Connected to database file";

                QSqlQuery qry;
                if(qry.exec("SELECT Username, Password, user_id FROM Login WHERE Username=\'"+username + "\' AND Password=\'" + password + "\'"))
                {
                    if (qry.next())
                    {
                        QSqlRecord rec = qry.record();

                        //nNumber  = query.value(rec.indexOf("number")).toInt();
                        QString username1  = qry.value(rec.indexOf("Username")).toString();
                        QString password1 = qry.value(rec.indexOf("Password")).toString();
                        int user_id1 = qry.value(rec.indexOf("user_id")).toInt();

                        //логин/пароль сошлись = авторизован
                        emit acceptLogin2( username1, // current username
                                           password1, // current password
                                           user_id1);
                        dbase.close();
                        close();
                    }
                    else QMessageBox::critical(this, "Eror: Неверный логин/пароль", "Повторите попытку");
                }
            }
            else
            {
                qDebug() << "Что-то не так с соединением!";
                QMessageBox::critical(this, "Eror: Не удалось подключиться к базе данных", "Повторите попытку");
                //return;
            }
        }
    }
    else
    {
        qDebug()<<"Database file does not exist";
    }
}
