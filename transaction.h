#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDialog>
#include <QString>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <unistd.h>

namespace Ui {
class transaction;
}

struct Transaction{
    QString id;
    QString status;
    QString product;
    QString price;
    QString buyer;
    QString seller;
    QString address;
    QString delivery;
    QString d_num;
    QString broker;
    QString webtid;
};


class transaction : public QDialog
{
    Q_OBJECT

public:
    explicit transaction(QString uid, QWidget *parent = nullptr);
    void set_uid(QString uid) { user_id = uid; }
    void set_tid(QString t) { tid = t; }
    ~transaction();

private slots:
    void on_pushButton_clicked();
    void after_request();
    void on_pushButton_3_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void address_done(QString address);
    void delivery_done(QString delivery, QString d_num);
    void on_tableWidget_2_doubleClicked(const QModelIndex &index);
    void on_pushButton_2_clicked();

    void on_buyer_id_received(QString web_id);
    void on_sale_id_received(QString sale_id);
    void on_seller_id_received(QString seller_id, QString price, QString product, QString wid);

signals:
    void request_success();
    void buyer_id_received(QString web_id);
    void sale_id_received(QString sale_id);
    void seller_id_received(QString seller_id, QString price, QString product, QString wid);

private:
    Ui::transaction *ui;
    QString user_id;
    QString tid;
    QString rn;
    Transaction t;
    Transaction temp;
    void load_transaction_info(){
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM TRANSACTION_INFO WHERE ID='"+this->tid+"'");
        if (model.rowCount() == 1){
            this->t.id = model.record(0).value("ID").toString();
            this->t.status = model.record(0).value("STATUS").toString();
            this->t.product = model.record(0).value("PRODUCT").toString();
            this->t.price = model.record(0).value("PRICE").toString();
            this->t.seller = model.record(0).value("SELLER").toString();
            this->t.address = model.record(0).value("ADDRESS").toString();
            this->t.delivery = model.record(0).value("DELIVERY").toString();
            this->t.d_num = model.record(0).value("D_NUM").toString();
            this->t.broker = model.record(0).value("BROKER").toString();
            this->t.webtid = model.record(0).value("WEBTID").toString();
        }else{
            QMessageBox::warning(this, "Failed", "DB has no such data or data is corrupted");

        }
    }
    void update_transaction_status(){
        QSqlQuery query;
        QString sql = "UPDATE TRANSACTION_INFO SET STATUS='"+this->t.status+
                        "', ADDRESS='"+this->t.address+"', DELIVERY='"+this->t.delivery+"', D_NUM='"+this->t.d_num+
                        "', BROKER='"+this->t.broker+"' WHERE ID='"+this->t.id+"' ;";
        qDebug() << sql;
        query.exec(sql);
    }
    void set_rn(){
        char alpha[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        rn = "";
        for (int i = 0; i<10; i++)
            rn = rn + alpha[rand() % 10];
    }
    void insert_transaction(){
        QSqlQuery query;
        QSqlQueryModel model;

        model.setQuery("SELECT * FROM USER_INFO WHERE ID='"+temp.seller+"'");
        if (model.rowCount() == 1){
            QString sql = QString("INSERT INTO TRANSACTION_INFO"
                                  "(status, product, price, buyer, seller, broker, delivery, address, d_num, webtid) "
                                  "VALUES(1, '%1', '%2', '%3', '%4', 0, ' ', ' ', ' ', '%5');")
                                  .arg(this->temp.product, this->temp.price, user_id, this->temp.seller, this->temp.webtid);
            query.exec(sql);
        }
    }
};

#endif // TRANSACTION_H
