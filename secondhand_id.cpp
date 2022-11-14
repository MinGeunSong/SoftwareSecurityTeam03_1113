#include "secondhand_id.h"
#include "ui_secondhand_id.h"

secondhand_ID::secondhand_ID(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::secondhand_ID)
{
    ui->setupUi(this);
}

secondhand_ID::~secondhand_ID()
{
    delete ui;
}

void secondhand_ID::on_pushButton_clicked()
{
    emit secondhand_ID::my_id_sig(ui->lineEdit->text());
    this->close();
}
