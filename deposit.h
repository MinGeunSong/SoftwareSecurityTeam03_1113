#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>
#include <QString>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>



#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

#include <QString>



namespace Ui {
class deposit;
}

class deposit : public QDialog
{
    Q_OBJECT

public:
    explicit deposit(QString user_id, QWidget *parent = nullptr);
    void set_uid(QString uid) { user_id = uid; }
    ~deposit();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::deposit *ui;
    QString user_id;
};

#endif // DEPOSIT_H
