#include "transaction.h"
#include "ui_transaction.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "request.h"
#include <QModelIndex>
#include "buyer_transaction.h"
#include "seller_transaction.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QJsonObject>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include "secondhand_id.h"
#include <QSqlDatabase>

#define REQUEST 1
#define INITIATION 2
#define CASH_IN 3
#define DELIVERY 4
#define VERIFY 5
#define CLOSE 6
#define CANCELLED_BY_SELLER 7
#define CANCELLED_BY_BUYER 8


transaction::transaction(QString user_id, QWidget *parent) :

    QDialog(parent),
    ui(new Ui::transaction)
{
    ui->setupUi(this);
    set_uid(user_id);
    mydb = QSqlDatabase::database("qt_sql_default_connection");
    //load model for buy data
    QSqlQuery qry1;
    QSqlQuery qry2;
    qry1.prepare("SELECT ID, STATUS, PRODUCT, PRICE, SELLER FROM TRANSACTION_INFO WHERE BUYER= :id");
    qry1.bindValue(":id", user_id);
    qry1.exec();
    while(qry1.next()){
        QString tid = qry1.value(0).toString();
        QString status = qry1.value(1).toString();
        QString product =  qry1.value(2).toString();
        QString price =  qry1.value(3).toString();
        QString seller =  qry1.value(4).toString();

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(tid));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(status));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(product));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(price));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(seller));
    }

    //load model for sell data
    qry2.prepare("SELECT ID, STATUS, PRODUCT, PRICE, BUYER FROM TRANSACTION_INFO WHERE SELLER= :id");
    qry2.bindValue(":id", user_id);
    qry2.exec();
    while(qry2.next()){
        QString tid = qry2.value(0).toString();
        QString status = qry2.value(1).toString();
        QString product =  qry2.value(2).toString();
        QString price =  qry2.value(3).toString();
        QString buyer =  qry2.value(4).toString();

        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 0, new QTableWidgetItem(tid));
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 1, new QTableWidgetItem(status));
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 2, new QTableWidgetItem(product));
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 3, new QTableWidgetItem(price));
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 4, new QTableWidgetItem(buyer));
    }
}

transaction::~transaction()
{
    delete ui;
}

void transaction::on_pushButton_clicked()
{
    request* r;
    request request(user_id, this);
    r = &request;
    request.setModal(true); //modal approach
    connect(r, SIGNAL(request_success()), this, SLOT(after_request()));
    request.exec();
}

void transaction::after_request(){
    //load model for buy data
    QSqlQuery qry1;
    qry1.prepare("SELECT ID, STATUS, PRODUCT, PRICE, SELLER FROM TRANSACTION_INFO WHERE BUYER= :id");
    qry1.bindValue(":id", user_id);
    qry1.exec();
    int i = 0;
    while(qry1.next()){
        QString tid = qry1.value(0).toString();
        QString status = qry1.value(1).toString();
        QString product =  qry1.value(2).toString();
        QString price =  qry1.value(3).toString();
        QString seller =  qry1.value(4).toString();

        if(i==ui->tableWidget->rowCount()){
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        }
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tid));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(status));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(product));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(price));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(seller));
        ++i;
    }
}

void transaction::on_pushButton_3_clicked()
{
    QSqlQuery qry2;
    qry2.prepare("SELECT ID, STATUS, PRODUCT, PRICE, BUYER FROM TRANSACTION_INFO WHERE SELLER= :id");
    qry2.bindValue(":id", user_id);
    qry2.exec();
    int i = 0;

    while(qry2.next()){
        QString tid = qry2.value(0).toString();
        QString status = qry2.value(1).toString();
        QString product =  qry2.value(2).toString();
        QString price =  qry2.value(3).toString();
        QString buyer =  qry2.value(4).toString();

        if(i==ui->tableWidget_2->rowCount()){
            ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        }
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(tid));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(status));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(product));
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(price));
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(buyer));
        ++i;
    }
    after_request();
}

void transaction::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel model;
    QSqlQuery qry;
    QString balance;
    QString info;
    QString tid = ui->tableWidget->item(index.row(), 0)->text();
    this->set_tid(tid);

    if(mydb.transaction()){
        this->load_transaction_info();
        int status = this->t.status.toInt();

        switch(status)
        {
            case REQUEST:{
                info = "Seller has not accepted transaction yet.";
                QMessageBox::information(this, "Status : Request", info);
                break;
            }
            case INITIATION:{
                info = "Seller has accepted transaction.";
                QMessageBox::information(this, "Status : Request", info);
                model.setQuery("SELECT balance from user_info where id='" + user_id + "'");
                QString balance = model.record(0).value("balance").toString();
                QString price = this->t.price;
                if (price.toInt() > balance.toInt()){
                    info = "You have "+ balance + " won in your account. Price is "+ price +" won. Not enough money.";
                    QMessageBox::warning(this, "Not Enough Money", info);
                    break;
                }else{
                    info = price + " won withdrawed from your account\nYou have "+ QString::number(balance.toInt()-price.toInt()) + " won left";
                    QMessageBox::information(this, "Withdrawl", info);
                }
                info = "Please let us know your address to get the goods delivered.";
                QMessageBox::information(this, "Address Submit", info);
                buyer_transaction * bptr;
                buyer_transaction buyer_transaction(user_id, tid, this);
                bptr = &buyer_transaction;
                buyer_transaction.setModal(true);
                connect(bptr, SIGNAL(done(QString)), this, SLOT(address_done(QString)));
                buyer_transaction.exec();
                break;
            }
            case CASH_IN:{
                info = "Seller has not sent goods yet.";
                QMessageBox::information(this, "Status : Not Sent Yet", info);
                break;
            }
            case DELIVERY:{
                info = "Parcel Delivery Company : " + this->t.delivery +"\nTracking Number : " + this->t.d_num + "\nHave you received your post?";
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Delivery Check", info, QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes){
                    this->t.status = '5';
                    update_transaction_status();
                }else{
                    break;
                }
            }
            case VERIFY:{
                if (this->t.broker.toInt()){
                    QMessageBox::information(this, "Transaction Finished", "We already have checked the status of the goods.\nTransaction Completed");
                    this->t.status = '6';
                    update_transaction_status();
                }else{
                    QMessageBox::StandardButton reply2;
                    info = "Is the delivered goods in good condition?";
                    reply2 = QMessageBox::question(this, "Goods Check", info, QMessageBox::Yes|QMessageBox::No);
                    if (reply2 == QMessageBox::Yes){
                        this->t.status = '6';
                        update_transaction_status();
                    }else{
                        this->t.status = '8';
                        update_transaction_status();
                        //Buyer gets money back
                        model.setQuery("SELECT balance from user_info where id='" + this->t.buyer + "'");
                        balance = model.record(0).value("balance").toString();
                        qry.prepare("UPDATE user_info SET balance = '"+ QString::number(balance.toInt() + this->t.price.toInt()) +"' where id='" + this->t.buyer + "'");
                        qry.exec();
                        break;
                    }
                }
                //Seller gets money
                model.setQuery("SELECT balance from user_info where id='" + this->t.seller + "'");
                balance = model.record(0).value("balance").toString();
                qry.prepare("UPDATE user_info SET balance = '"+ QString::number(balance.toInt() + this->t.price.toInt()) +"' where id='" + this->t.seller + "'");
                qry.exec();
                break;
            }
            case CLOSE:{
                info = "It's a completed transaction.";
                QMessageBox::information(this, "Status : Finished", info);
                break;
            }
            case CANCELLED_BY_SELLER:{
                info = "Seller Rejected this Transaction.";
                QMessageBox::information(this, "Status : Rejected", info);
                break;
            }
            case CANCELLED_BY_BUYER:{
                info = "This transaction is cancelled due to seller's fraud.";
                QMessageBox::information(this, "Status : Cancelled", info);
                break;
            }
        }
        //to refresh table
        after_request();
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

void transaction::address_done(QString address){
    this->t.address = address;
    this->t.status = '3';
    update_transaction_status();
    QSqlQuery qry;
    QSqlQueryModel model;
    model.setQuery("SELECT balance from user_info where id='" + user_id + "'");
    QString balance = model.record(0).value("balance").toString();
    qry.prepare("UPDATE user_info SET balance = '"+ QString::number(balance.toInt() - this->t.price.toInt()) +"' where id='" + user_id + "'");
    qry.exec();
}

void transaction::delivery_done(QString delivery, QString d_num){
    if (this->t.broker.toInt()){
        this->t.delivery = "Broker";
        set_rn();
        this->t.d_num = this->rn;
    }else{
        this->t.delivery = delivery;
        this->t.d_num = d_num;
    }
    this->t.status = '4';
    update_transaction_status();
}

void transaction::on_tableWidget_2_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel model;
    QSqlQuery qry;
    QString info;
    QString tid = ui->tableWidget_2->item(index.row(), 0)->text();
    this->set_tid(tid);
    if(mydb.transaction()){
        this->load_transaction_info();
        int status = this->t.status.toInt();

        switch(status)
        {
            case REQUEST:{
                info = "" + this->t.buyer + " has requested transaction.\nGoods : " + this->t.product +"\nPrice : " + this->t.price +"\nWill you accept it?";
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "New Request", info, QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes){
                    info = "Broker Service ensures that the goods you sent is in good condition and the seller can make no fraud.\nService fee is 3000 won.\nWill you use it?";
                    QMessageBox::StandardButton reply2;
                    reply2 = QMessageBox::question(this, "Will you use Broker Service?", info, QMessageBox::Yes|QMessageBox::No);
                    if (reply2 == QMessageBox::Yes){
                            model.setQuery("SELECT balance from user_info where id='" + user_id + "'");
                            QString balance = model.record(0).value("balance").toString();
                            if (balance.toInt() < 3000){
                                QMessageBox::warning(this, "Not Enough Money", "You don't have enough money to use service.\nYou currently have" + balance + " won.");
                                break;
                            }else{
                                qry.prepare("UPDATE user_info SET balance = '"+ QString::number(balance.toInt() - 3000) +"' where id='" + user_id + "'");
                                qry.exec();
                                this->t.broker = '1';
                            }
                    }
                    this->t.status = '2';
                    update_transaction_status();
                }else{
                    this->t.status = '7';
                    update_transaction_status();
                    break;
                }
                break;
            }
            case INITIATION:{
                info = "Buyer has not withdrawed money yet";
                QMessageBox::information(this, "Status : Wait for Seller", info);
                break;
            }
            case CASH_IN:{
                info = "Buyer has paid " + this->t.price + " won.\n";
                if (this->t.broker.toInt()){
                    info.append("For the Broker Service please send the goods to 145 Anam-ro Seongbuk-Gu Seoul.");
                }else{
                    info.append("Please send the goods to " + this->t.address + ".");
                }
                QMessageBox::information(this, "Send your Goods", info);

                //Delivery check
                info = "Have you sent your goods?\nIf then please let us know Delivery Parcel Company Name and Tracking Number";
                QMessageBox::StandardButton reply3;
                reply3 = QMessageBox::question(this, "Delivery Check", info, QMessageBox::Yes|QMessageBox::No);
                if (reply3 == QMessageBox::Yes){
                        seller_transaction * sptr;
                        seller_transaction seller_transaction(this);
                        sptr = &seller_transaction;
                        seller_transaction.setModal(true);
                        connect(sptr, SIGNAL(delivery_done(QString, QString)), this, SLOT(delivery_done(QString,QString)));
                        seller_transaction.exec();
                        break;
                    }
                else{
                    break;
                }
            }
            case DELIVERY:{
            }
            case VERIFY:{
                info = "Goods is on delivery. Please wait until seller receives and check the goods.";
                QMessageBox::information(this, "On Delivery", info);
                break;
            }
            case CLOSE:{
                info = "It's a completed transaction.";
                QMessageBox::information(this, "Status : Finished", info);
                break;
            }
            case CANCELLED_BY_SELLER:{
                info = "You have rejected this Transaction.";
                QMessageBox::information(this, "Status : Rejected", info);
                break;
            }
            case CANCELLED_BY_BUYER:{
                info = "This transaction is cancelled due to your fault.";
                QMessageBox::information(this, "Status : Cancelled", info);
                break;
            }
        }
        //To refresh table
        on_pushButton_3_clicked();
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

void transaction::on_pushButton_2_clicked()
{
    //0. Valid Web ID Check
    secondhand_ID * ptr;
    secondhand_ID s_ID(this);
    ptr = &s_ID;
    s_ID.setModal(true);
    connect(ptr, SIGNAL(my_id_sig(QString)), this, SLOT(on_my_id_received(QString)));
    s_ID.exec();

    //1. make request
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    const QUrl url(QString("http://3.36.207.92/common/user/?username=%1").arg(this->my_id));//url
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    //2. Reply
    QNetworkReply *reply = mgr->get(request);

	
    //3. Process when finished
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QByteArray qb = reply->readAll();
            QJsonDocument json_response = QJsonDocument::fromJson(qb);

            //parse json
            QJsonArray json_array = json_response.array();

            foreach (const QJsonValue &value, json_array){
                QJsonObject json_obj = value.toObject();
                QString web_id = json_obj["username"].toString();
                QString compare = json_obj["broker_id"].toString();

                if (web_id == this->my_id && compare == user_id){
                    QString buyer_id_on_web = QString::number(json_obj["id"].toInt());
                    connect(this, SIGNAL(buyer_id_received(QString)), this, SLOT(on_buyer_id_received(QString)));
                    emit buyer_id_received(buyer_id_on_web);
                }else{
                    QString info = "Your ID is not registered in the Website.";
                    QMessageBox::warning(this, "Invalid ID", info);
                }
               }
        }
        else{
            QString err = reply->errorString();
        }
        reply->deleteLater();
    });
}

void transaction::on_buyer_id_received(QString web_id){
    //1. make request
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    const QUrl url(QString("http://3.36.207.92/board/api/bid_api/?buyer_id=%1&agreed=1").arg(web_id));//url
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //2. Reply
    QNetworkReply *reply = mgr->get(request);

    //3. Process when finished
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QByteArray qb = reply->readAll();
            QJsonDocument json_response = QJsonDocument::fromJson(qb);

            //parse json
            QJsonArray json_array = json_response.array();

            foreach (const QJsonValue &value, json_array){
                QJsonObject json_obj = value.toObject();
                QString sale_id_on_web = QString::number(json_obj["sale_id"].toInt());
                connect(this, SIGNAL(sale_id_received(QString)), this, SLOT(on_sale_id_received(QString)));
                emit sale_id_received(sale_id_on_web);
            }
        }
        else{
            QString err = reply->errorString();
        }
        reply->deleteLater();
    });
}


void transaction::on_sale_id_received(QString sale_id_on_web){
    //1. make request
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    const QUrl url(QString("http://3.36.207.92/board/api/sale_api/?id=%1").arg(sale_id_on_web));//url
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //2. Reply
    QNetworkReply *reply = mgr->get(request);


    //3. Process when finished
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QByteArray qb = reply->readAll();
            QJsonDocument json_response = QJsonDocument::fromJson(qb);

            //parse json
            QJsonArray json_array = json_response.array();

            foreach (const QJsonValue &value, json_array){
                QJsonObject json_obj = value.toObject();
                QString price = QString::number(json_obj["price"].toInt());
                QString product = json_obj["product"].toString();
                QString seller_id = QString::number(json_obj["seller_id"].toInt());
                connect(this, SIGNAL(seller_id_received(QString, QString, QString, QString)), this, SLOT(on_seller_id_received(QString, QString, QString, QString)));
                emit seller_id_received(seller_id, price, product, sale_id_on_web);
            }
        }
        else{
            QString err = reply->errorString();
        }
        reply->deleteLater();
    });
}

void transaction::on_seller_id_received(QString seller_id, QString price, QString product, QString wid){
    //1. make request
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    const QUrl url(QString("http://3.36.207.92/common/user/?id=%1").arg(seller_id));//url
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //2. Reply
    QNetworkReply *reply = mgr->get(request);


    //3. Process when finished
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QByteArray qb = reply->readAll();
            QJsonDocument json_response = QJsonDocument::fromJson(qb);

            //parse json
            QJsonArray json_array = json_response.array();

            foreach (const QJsonValue &value, json_array){
                QJsonObject json_obj = value.toObject();
                QString seller = json_obj["broker_id"].toString();
                this->temp.buyer = user_id;
                this->temp.price = price;
                this->temp.product = product;
                this->temp.webtid = wid;
                this->temp.seller = seller;
                insert_transaction();
                after_request();

            }
        }
        else{
            QString err = reply->errorString();
        }
        reply->deleteLater();
    });
}

void transaction::on_my_id_received(QString my_id){
    this->my_id = my_id;
}
