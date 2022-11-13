#include "request.h"
#include "ui_request.h"
#include <QSqlQueryModel>
#include <QMessageBox>

request::request(QString user_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::request)
{
    ui->setupUi(this);
    set_uid(user_id);
}

request::~request()
{
    delete ui;
}

void request::on_buttonBox_accepted()
{
    QString product = QString();
    QString seller = QString();
    QString price = QString();

    product = ui->product_edit->text();
    seller = ui->seller_edit->text();
    price = ui->price_edit->text();

    QSqlQueryModel model;

    model.setQuery("SELECT * FROM USER_INFO WHERE ID='"+seller+"'");
    if (model.rowCount() == 1 && seller != user_id)
    {
        QString sql = QString("INSERT INTO TRANSACTION_INFO"
                              "(status, product, price, buyer, seller, broker, delivery, address, d_num) "
                              "VALUES(1, '%1', '%2', '%3', '%4', 0, ' ', ' ', ' ');").arg(product, price, user_id, seller);
        model.setQuery(sql);
        QMessageBox::about(this, "Success", "Transaction was Added Successfully");
        emit request_success();
        this->close();
    }else
    {
        QMessageBox::warning(this, "Failed", "Invalid Seller ID");
    }

}
