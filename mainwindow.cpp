#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupwindow.h"
#include "mainafterlogin.h"

#include <QCryptographicHash>
#include <QByteArray>
#include <QMessageBox>
#include <cstdio>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("test.db");

    if(!mydb.open())
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to open the databases");
        msgBox.exec();
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    signupwindow signupwindow;
    signupwindow.setModal(true); //modal approach여서 해당 버튼으로 열리는 새창이 꺼져야 기존 창을 사용가능
    signupwindow.exec();
}


void MainWindow::on_pushButton_2_clicked()
{
    QString user_pw,user_id;
    user_id = ui->lineEdit_id->text();
    user_pw = ui->lineEdit_pw->text();

    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("test.db");

    if(!mydb.open())
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to open the databases");
        msgBox.exec();
        exit(EXIT_FAILURE);
    }

    QSqlQuery qry;
    QString user_salt;

    qry.exec("select salt from user_info where id='"+user_id+"'");
    while(qry.next()){
        user_salt = qry.value(0).toString();
        qry.clear();
    }

    user_pw = user_pw + user_salt;
    char* hash_pw_tmp = user_pw.toLocal8Bit().data();


    QByteArray hash_pw(hash_pw_tmp);
    user_pw = QString(QCryptographicHash::hash((hash_pw),QCryptographicHash::Sha3_256).toHex());
    QString qry_str = "select * from user_info where id='"+user_id+"'and userpw='"+user_pw+"'";
    if(qry.exec(qry_str))
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if (count==1)
        {
            hide();
            qry.clear();
            QString user_pw_real; // for validation of password
            qry.exec("select userpw from user_info where id='"+user_id+"'");
            while(qry.next()){
                user_pw_real = qry.value(0).toString();
                qry.clear();
            }

            mainafterlogin mainafterlogin(user_id, this);

            mainafterlogin.setModal(true); //modal approach여서 해당 버튼으로 열리는 새창이 꺼져야 기존 창을 사용가능
            mainafterlogin.exec();
        }
        if (count>1)
        {
            QMessageBox msgBox;
            msgBox.setText("Duplicate ID and Password");
            msgBox.exec();
        }
        if (count<1)
        {
            QMessageBox msgBox;
            msgBox.setText("LOGIN FAILED: ID and Password is not correct");
            msgBox.exec();
        }
    }
    mydb.close();
}
