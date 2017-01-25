#include "dialog_1.h"
#include "ui_dialog_1.h"

Dialog_1::Dialog_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_1)
{
    ui->setupUi(this);
}

Dialog_1::~Dialog_1()
{
    delete ui;
}

void Dialog_1::on_pushButton_clicked()
{
    setResult(1);
    setVisible(false);
}

void Dialog_1::on_pushButton_2_clicked()
{
    setResult(0);
    setVisible(false);
}

QString Dialog_1::getLineEditName(){
    return ui->lineEdit_name->text();
}
void Dialog_1::setLineEditName(QString name){
    ui->lineEdit_name->setText(name);
}
QString Dialog_1::getLineEditCode(){
    return ui->lineEdit_code->text();
}
void Dialog_1::setLineEditCode(QString name){
    ui->lineEdit_code->setText(name);
}
QString Dialog_1::getLineEditTime(){
    return ui->lineEdit_dur->text();
}
void Dialog_1::setLineEditTime(QString name){
    ui->lineEdit_dur->setText(name);
}
