#include "dialog_2.h"
#include "ui_dialog_2.h"

Dialog_2::Dialog_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_2)
{
    ui->setupUi(this);
}

Dialog_2::~Dialog_2()
{
    delete ui;
}

void Dialog_2::on_pushButton_clicked()
{
    setResult(1);
    setVisible(false);
}

void Dialog_2::on_pushButton_2_clicked()
{
    setResult(0);
    setVisible(false);
}

QString Dialog_2::getLineEditName(){
    return ui->lineEdit_name->text();
}
QString Dialog_2::getLineEditDur(){
    return ui->lineEdit_dur->text();
}
QString Dialog_2::getLineEditFio(){
    return ui->lineEdit_fio->text();
}
QString Dialog_2::getLineEditSemestr(){
    return ui->lineEdit_semestr->text();
}
void Dialog_2::setLineEditName(QString name){
    ui->lineEdit_name->setText(name);
}
void Dialog_2::setLineEditDur(QString name){
    ui->lineEdit_dur->setText(name);
}
void Dialog_2::setLineEditFio(QString name){
    ui->lineEdit_fio->setText(name);
}
void Dialog_2::setLineEditSemestr(QString name){
    ui->lineEdit_semestr->setText(name);
}

