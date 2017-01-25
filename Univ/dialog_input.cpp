#include "dialog_input.h"
#include "ui_dialog_input.h"

Dialog_input::Dialog_input(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_input)
{
    ui->setupUi(this);
}

Dialog_input::~Dialog_input()
{
    delete ui;
}

void Dialog_input::on_pushButton_clicked()
{
    setResult(1);
    setVisible(false);
}

void Dialog_input::on_pushButton_2_clicked()
{
    setResult(0);
    setVisible(false);
}

QString Dialog_input::getLineEditName(){
    return ui->lineEdit->text();
}

void Dialog_input::setboxname(QString name){
        ui->groupBox->setTitle(name);
    }

void Dialog_input::setformname(QString name){
        setWindowTitle(name);
    }
