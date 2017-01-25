#ifndef DIALOG_DEL_H
#define DIALOG_DEL_H

#include <QDialog>

namespace Ui {
class Dialog_del;
}

class Dialog_del : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_del(QWidget *parent = 0);
    ~Dialog_del();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_del *ui;
};

#endif // DIALOG_DEL_H
