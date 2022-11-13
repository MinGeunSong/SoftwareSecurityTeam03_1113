#ifndef MAINAFTERLOGIN_H
#define MAINAFTERLOGIN_H

#include <QDialog>
#include <QString>

namespace Ui {
class mainafterlogin;
}

class mainafterlogin : public QDialog
{
    Q_OBJECT

public:
    explicit mainafterlogin(QString user_id, QWidget *parent = nullptr);
    void set_uid(QString uid) { user_id = uid; }
    ~mainafterlogin();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::mainafterlogin *ui;
    QString user_id;
};

#endif // MAINAFTERLOGIN_H
