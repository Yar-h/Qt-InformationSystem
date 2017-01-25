#ifndef DIALOG_3_H
#define DIALOG_3_H

#include <QDialog>

namespace Ui {
class Dialog_3;
}

class Dialog_3 : public QDialog
{
    Q_OBJECT

public:
    QString getLineEditName();
    QString getLineEditAut();
    QString getLineEditYear();
    void setLineEditName(QString name);
    void setLineEditAut(QString name);
    void setLineEditYear(QString name);
    
public:
    explicit Dialog_3(QWidget *parent = 0);
    ~Dialog_3();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_3 *ui;
};

#endif // DIALOG_3_H
