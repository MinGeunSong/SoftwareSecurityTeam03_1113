#ifndef SELLER_TRANSACTION_H
#define SELLER_TRANSACTION_H

#include <QDialog>

namespace Ui {
class seller_transaction;
}

class seller_transaction : public QDialog
{
    Q_OBJECT

public:
    explicit seller_transaction(QWidget *parent = 0);
    ~seller_transaction();

private:
    Ui::seller_transaction *ui;

signals:
    void delivery_done(QString delivery, QString d_num);
private slots:
    void on_pushButton_clicked();
};

#endif // SELLER_TRANSACTION_H
