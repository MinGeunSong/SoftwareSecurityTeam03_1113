#include "seller_transaction.h"
#include "ui_seller_transaction.h"
#include <QMessageBox>

seller_transaction::seller_transaction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::seller_transaction)
{
    ui->setupUi(this);
}

seller_transaction::~seller_transaction()
{
    delete ui;
}

void seller_transaction::on_pushButton_clicked()
{
    if(ui->d_edit->text().length()==0 || ui->d_num_edit->text().length()==0){
        QMessageBox::warning(this, "No Input Error", "You have to write proper inputs for each field.");
    }else{
        emit delivery_done(ui->d_edit->text(), ui->d_num_edit->text());
        this->close();
    }
}
