#include "dialog_3.h"
#include "ui_dialog_3.h"

Dialog_3::Dialog_3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_3)
{
    ui->setupUi(this);
}

Dialog_3::~Dialog_3()
{
    delete ui;
}

void Dialog_3::on_pushButton_clicked()
{
    setResult(1);
    setVisible(false);
}

void Dialog_3::on_pushButton_2_clicked()
{
    setResult(0);
    setVisible(false);
}

QString Dialog_3::getLineEditName(){
    return ui->lineEdit_name->text();
}
QString Dialog_3::getLineEditAut(){
    return ui->lineEdit_aut->text();
}
QString Dialog_3::getLineEditYear(){
    return ui->lineEdit_year->text();
}
void Dialog_3::setLineEditName(QString name){
    return ui->lineEdit_name->setText(name);
}
void Dialog_3::setLineEditAut(QString name){
    return ui->lineEdit_aut->setText(name);
}
void Dialog_3::setLineEditYear(QString name){
    return ui->lineEdit_year->setText(name);
}
