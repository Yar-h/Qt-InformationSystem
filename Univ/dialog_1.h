#ifndef DIALOG_1_H
#define DIALOG_1_H

#include <QDialog>

namespace Ui {
class Dialog_1;
}

class Dialog_1 : public QDialog
{
    Q_OBJECT
    
public:
    QString getLineEditName();
    QString getLineEditCode();
    QString getLineEditTime();
    void setLineEditName(QString name);
    void setLineEditCode(QString name);
    void setLineEditTime(QString name);

public:
    explicit Dialog_1(QWidget *parent = 0);
    ~Dialog_1();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_1 *ui;
};

#endif // DIALOG_1_H
