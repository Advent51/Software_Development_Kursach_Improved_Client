#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>
#include <QtSql>

#define Path_to_DB "C:/TEST/SinicinDB.db"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase dbase;
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    void setUsername( QString username );
    void setPassword( QString password );

    /*!
     * Sets a list of allowed usernames from which the user
     * can pick one if he does not want to directly edit it.
     '''usernames a list of usernames
    */
    void setUsernamesList( const QStringList usernames );
    //void DataBaseConnection();

private:
    Ui::LoginDialog *ui;

    void setUpGUI();

signals:
    void acceptLogin2( QString username, QString password, int user_id);

public slots:
    void DataBaseConnection();
    void Closing();
};

#endif // LOGINDIALOG_H
