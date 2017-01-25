#include "dialog_del.h"
#include "ui_dialog_del.h"

Dialog_del::Dialog_del(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_del)
{
    ui->setupUi(this);
}

Dialog_del::~Dialog_del()
{
    delete ui;
}

void Dialog_del::on_pushButton_clicked()
{
    setResult(1);
    setVisible(false);
}

void Dialog_del::on_pushButton_2_clicked()
{
    setResult(0);
    setVisible(false);
}
