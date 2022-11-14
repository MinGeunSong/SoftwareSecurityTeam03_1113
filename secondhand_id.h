#ifndef SECONDHAND_ID_H
#define SECONDHAND_ID_H

#include <QDialog>

namespace Ui {
class secondhand_ID;
}

class secondhand_ID : public QDialog
{
    Q_OBJECT

public:
    explicit secondhand_ID(QWidget *parent = 0);
    ~secondhand_ID();

private slots:
    void on_pushButton_clicked();

private:
    Ui::secondhand_ID *ui;


signals:
    void my_id_sig(QString);
};

#endif // SECONDHAND_ID_H
