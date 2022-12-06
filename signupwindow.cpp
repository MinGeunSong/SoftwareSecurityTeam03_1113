#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <QtSql/QSqlQuery>
#include <QCryptographicHash>
#include <cctype>
signupwindow::signupwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signupwindow)
{
    ui->setupUi(this);
}
signupwindow::~signupwindow()
{
    delete ui;
}
QString tt = QString();
int iko_ = 0;
int check_qstring(QString qs_input, int flag) {
    if (qs_input.isEmpty() == true) {
        flag = 3;
        return flag;
    }
    for (int cc_t = 0; cc_t < qs_input.size(); cc_t = cc_t + 1) {
        if (qs_input.data()[cc_t].isLetterOrNumber() == 1) { //
            flag = 0;
        } else {
            flag = 1;
            break;
        }
    }
    return flag;
}
void signupwindow::on_pushButton_clicked()
{
    tt = ui->lineEdit->text();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    db.open();
    QSqlQuery query;
    int flag_100 = 2;
    flag_100 = check_qstring(tt,flag_100);
    query.exec("CREATE TABLE IF NOT EXISTS User_info(id TEXT primary key, userpw text, salt int, balance int) ;");
    QString ttt = QString("select * from user_info where id == '%1';").arg(tt);
    query.exec(ttt);
    QString id = QString();
    while(query.next()) {
        id = query.value(0).toString();
        std::string str = id.toStdString();
    }
    if (flag_100 == 1){
        ui->label_4->setText("ID must Eng or Num");
        iko_ = 0;
    } else if (flag_100 == 3) {
        ui->label_4->setText("type ID");
        iko_ = 0;
    } else if (tt.length() < 2) {
        ui->label_4->setText("type longer ID");
        iko_ = 0;
    } else if (id.isEmpty() == true) {
        ui->label_4->setText("Can use ID");
        iko_ = 1;
    } else {
        ui->label_4->setText("Can't use ID");
        iko_ = 0;
    }
    db.close();
}
void signupwindow::on_pushButton_2_clicked()
{
    QString dt = QString();
    dt = ui->lineEdit->text();
    QString ttt = QString();
    ttt = ui->lineEdit_2->text();
    QString ttt_c = QString();
    ttt_c = ui->lineEdit_3->text();
    srand(time(NULL));
    int salt = rand();
    salt = rand();
    int flag_200 = 2;
    flag_200 = check_qstring(ttt,flag_200);
    QString str_salt = QString::number(salt);
    QString hht = ttt + str_salt;
    char* h_t = hht.toLocal8Bit().data();
    QByteArray h_tt(h_t);
    QString hash = QString(QCryptographicHash::hash((h_tt),QCryptographicHash::Sha3_256).toHex());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    db.open();
    if (iko_ == 0) {
        ui->label_4->setText("Check ID");
    } else if (tt != dt) {
        ui->label_4->setText("Check ID");
    } else if (flag_200 == 3) {
        ui->label_5->setText("Type PW");
    } else if (flag_200 == 1) {
        ui->label_5->setText("PW must Eng or Num");
    } else if (ttt == ttt_c) {
        QSqlQuery query;
        QString qt = QString("INSERT INTO User_Info VALUES('%1', '%2', %3, 0);").arg(tt,hash,str_salt);
        query.exec(qt);
        db.close();
        this->close();
    } else {
        ui->label_5->setText("PW is different");
    }
    db.close();
}
