#include "buyer_transaction.h"
#include "ui_buyer_transaction.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
#include <QMessageBox>

buyer_transaction::buyer_transaction(QString user_id, QString tid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::buyer_transaction)
{
    ui->setupUi(this);
    set_uid(user_id);
    set_tid(tid);
}

buyer_transaction::~buyer_transaction()
{
    delete ui;
}



void buyer_transaction::on_submit_address_clicked()
{
    QString address = ui->address_edit->text();

    if (address.length() > 0){
        emit done(address);
        this->close();
    }
    else{
        QMessageBox::warning(this, "Too Short", "You need to write your address.");
    }
}
