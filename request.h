#ifndef REQUEST_H
#define REQUEST_H

#include <QDialog>
#include <QString>

namespace Ui {
class request;
}

class request : public QDialog
{
    Q_OBJECT

public:
    explicit request(QString user_id, QWidget *parent = 0);
    void set_uid(QString uid) { user_id = uid; }
    ~request();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::request *ui;
    QString user_id;

signals:
    void request_success();
};

#endif // REQUEST_H
