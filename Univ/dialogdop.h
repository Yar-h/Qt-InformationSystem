#ifndef DIALOGDOP_H
#define DIALOGDOP_H

#include <QDialog>

namespace Ui {
class DialogDop;
}

class DialogDop : public QDialog
{
    Q_OBJECT
    
public:
    void InitTree();
    void ShowTree(void** start);

public:
    explicit DialogDop(QWidget *parent = 0);
    ~DialogDop();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogDop *ui;
};

#endif // DIALOGDOP_H
