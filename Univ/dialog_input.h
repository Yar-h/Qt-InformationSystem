#ifndef DIALOG_INPUT_H
#define DIALOG_INPUT_H

#include <QDialog>

namespace Ui {
class Dialog_input;
}

class Dialog_input : public QDialog
{
    Q_OBJECT

public:
    QString getLineEditName();
    void setboxname(QString name);
    void setformname(QString name);
    
public:
    explicit Dialog_input(QWidget *parent = 0);
    ~Dialog_input();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_input *ui;
};

#endif // DIALOG_INPUT_H
