#ifndef DIALOG_2_H
#define DIALOG_2_H

#include <QDialog>

namespace Ui {
class Dialog_2;
}

class Dialog_2 : public QDialog
{
    Q_OBJECT

public:
    QString getLineEditName();
    QString getLineEditDur();
    QString getLineEditFio();
    QString getLineEditSemestr();
    void setLineEditName(QString name);
    void setLineEditDur(QString name);
    void setLineEditFio(QString name);
    void setLineEditSemestr(QString name);
    
public:
    explicit Dialog_2(QWidget *parent = 0);
    ~Dialog_2();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_2 *ui;
};

#endif // DIALOG_2_H
