#include "mainafterlogin.h"
#include "ui_mainafterlogin.h"
#include "deposit.h"
#include "transaction.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QDebug>

mainafterlogin::mainafterlogin(QString user_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainafterlogin)
{
    ui->setupUi(this);
    set_uid(user_id);
    ui->label->setText("User name: "+ user_id);
}

mainafterlogin::~mainafterlogin()
{
    delete ui;
}

void mainafterlogin::on_pushButton_clicked()
{
    deposit deposit(user_id, this);
    deposit.setModal(true); //modal approach여서 해당 버튼으로 열리는 새창이 꺼져야 기존 창을 사용가능
    deposit.exec();
}


void mainafterlogin::on_pushButton_2_clicked()
{
    const char *sql_q;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    db.open();

    QSqlQuery query;

    sql_q = "CREATE TABLE IF NOT EXISTS TRANSACTION_INFO("\
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
            "STATUS TINYINT NOT NULL," \
            "PRODUCT VARCHAR(30) NOT NULL,"\
            "PRICE INT NOT NULL,"\
            "ADDRESS TEXT NOT NULL,"\
            "DELIVERY TEXT NOT NULL,"\
            "D_NUM TEXT NOT NULL,"\
            "BROKER TINYINT NOT NULL,"\
            "BUYER TEXT NOT NULL,"\
            "SELLER TEXT NOT NULL,"\
            "WEBTID TEXT UNIQUE,"\
            "FOREIGN KEY(BUYER) REFERENCES USER_INFO(ID)"\
            "FOREIGN KEY(SELLER) REFERENCES USER_INFO(ID));";

    query.exec(sql_q);
    transaction transaction(user_id, this);
    transaction.setModal(true); //modal approach
    transaction.exec();
}




void mainafterlogin::on_pushButton_3_clicked()
{
    this->close();
}

