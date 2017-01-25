#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void on_action_create_triggered();

private slots:

    void on_action_AddEl_triggered();

    void on_action_DelEl_triggered();

    void on_action_change_triggered();

    void on_action_find_triggered();

    void on_treeWidget_itemSelectionChanged();

    void on_action_findLit_triggered();

    void on_action_findSubj_triggered();

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_actionDop_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
