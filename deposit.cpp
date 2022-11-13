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
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("test.db");

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
    QString money = ui->lineEdit->text(); // money에 input bal saved
    int intmoney = money.toInt(); //Int type change
    //QMessageBox::information(this, "Title", money +" won has been deposited/withdrawed");


    if (ui->checkBox->isChecked() == true)
    {
        int baltype = 1;

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
            originalbal = originalbal + (baltype * intmoney);
            //update the new balance value to the DB
            QSqlQuery qry;
            qry.prepare("UPDATE user_info SET balance = '"+ QString::number(originalbal) +"' where id='" + user_id + "'");
            if (!qry.exec())
            {
                QMessageBox::information(this, "Title", "Could not update the DB");
            }
            QMessageBox::information(this, "Title", money +" won has been deposited");
        }

    }
    else
    {
        int baltype = -1;
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
            originalbal = originalbal + (baltype * intmoney);
            if (originalbal < 0)
            {
                QMessageBox::information(this, "Title", "You have no enough balance to withdraw");
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
        }
    }

}




void deposit::on_checkBox_stateChanged(int arg1)
{

}


