#ifndef BUYER_TRANSACTION_H
#define BUYER_TRANSACTION_H

#include <QDialog>
#include <QString>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>

namespace Ui {
class buyer_transaction;
}

class buyer_transaction : public QDialog
{
    Q_OBJECT

public:
    explicit buyer_transaction(QString user_id, QString tid, QWidget *parent = 0);
    void set_uid(QString uid) { user_id = uid; }
    void set_tid(QString tid) { t_id = tid; }
    ~buyer_transaction();

private:
    Ui::buyer_transaction *ui;
    QString user_id;
    QString t_id;

signals:
    void done(QString address);
private slots:
    void on_submit_address_clicked();
};

#endif // BUYER_TRANSACTION_H
