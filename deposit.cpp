#include "deposit.h"
#include "ui_deposit.h"
#include <QMessageBox>
#include <QIntValidator>

//#include <Source.cpp>

using namespace std;

deposit::deposit(QString user_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deposit)
{
    ui->setupUi(this);
    set_uid(user_id);
    //Only Int can be input
    QIntValidator *roll = new QIntValidator(1,100000000);
    //ui->lineEdit->setInputMask("000000000000000");
    ui->lineEdit->setValidator(roll);

    //SQL
    mydb = QSqlDatabase::database("qt_sql_default_connection");

    if(!mydb.open())
        ui->label->setText("Failed to open the database");
    else
        ui->label->setText("Amount");


}

deposit::~deposit()
{
    delete ui;
}

void deposit::on_pushButton_clicked()
{
    QString info;
    if(mydb.transaction()){
        QString money = ui->lineEdit->text(); // money에 input bal saved
        int intmoney = money.toInt(); //Int type change
        //read balance data
        QSqlQuery qry1;
        qry1.prepare("SELECT balance from user_info where id='" + user_id + "'");
        //qry1실행
        if (!qry1.exec())
        {
            QMessageBox::information(this, "Title", "Could not read the DB");
        }
        else
        {
            qry1.exec();
            int originalbal;
            while(qry1.next())
            {
                originalbal = qry1.value(0).toInt();
            }
            int check = __INT_MAX__ - originalbal;
            if(check > intmoney){
                //original bal has the updated balance value
                originalbal = originalbal + intmoney;
                //update the new balance value to the DB
                QSqlQuery qry;
                qry.prepare("UPDATE user_info SET balance = '"+ QString::number(originalbal) +"' where id='" + user_id + "'");
                if (!qry.exec())
                {
                    QMessageBox::information(this, "Title", "Could not update the DB");
                }
                QMessageBox::information(this, "Title", money +" won has been deposited");
            }else{
                QMessageBox::warning(this, "Deposit Limit Warning", "You cannot have balance over " + QString::number(__INT_MAX__));
            }
        }
        if(!mydb.commit()){
            info = "DB Transaction commit failed, Rollback Occurs.";
            QMessageBox::warning(this, "Rollback", info);
            mydb.rollback();
        }
    }else{
        info = "Failed to Start DB Transaction Mode";
        QMessageBox::warning(this, "DB Transaction Mode Enter Failure.", info);
    }
}

void deposit::on_pushButton_2_clicked()
{
    QString info;
    if(mydb.transaction()){
        QString money = ui->lineEdit->text(); // money에 input bal saved
        int intmoney = money.toInt(); //Int type change
        //read balance data
        QSqlQuery qry1;
        qry1.prepare("SELECT balance from user_info where id='" + user_id + "'");
        //qry1실행
        if (!qry1.exec())
        {
            QMessageBox::information(this, "Title", "Could not read the DB");
        }
        else
        {
            qry1.exec();
            int originalbal;
            while(qry1.next())
            {
                originalbal = qry1.value(0).toInt();
            }
            //original bal has the updated balance value
            originalbal = originalbal - intmoney;
            if (originalbal < 0)
            {
                QMessageBox::information(this, "Title", "You do not have enough balance to withdraw");
            }
            else
            {
                //update the new balance value to the DB
                QSqlQuery qry;
                qry.prepare("UPDATE user_info SET balance = '"+ QString::number(originalbal) +"' where id='" + user_id + "'");
                if (!qry.exec())
                {
                    QMessageBox::information(this, "Title", "Could not update the DB");
                }
                QMessageBox::information(this, "Title", money +" won has been withdrawed");
            }
            if(!mydb.commit()){
                info = "DB Transaction commit failed, Rollback Occurs.";
                QMessageBox::warning(this, "Rollback", info);
                mydb.rollback();
            }
        }
    }else{
        info = "Failed to Start DB Transaction Mode";
        QMessageBox::warning(this, "DB Transaction Mode Enter Failure.", info);
    }
}
