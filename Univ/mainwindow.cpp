#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "types.h"
#include "func.h"
#include <QString>
#include <stdio.h>
#include <QTextCodec>
#include <QInputDialog>
#include <QDialog>
#include <QFileDialog>
#include <QStringList>
#include <QDir>
#include "dialog_1.h"
#include "dialog_2.h"
#include "dialog_3.h"
#include "dialog_del.h"
#include "dialog_input.h"
#include "dialogdop.h"
#include <QMessageBox>


bool Created = false;
void** START;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_create_triggered()
{
    if (Created==true){return;};
    extern void** START;
    START=InitArray();
    Created=true;
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));//Установка русской кодировки


    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("");// Установить название колонки

    QStringList lst; //Корень
    lst<<"Список";
    QTreeWidgetItem* pItem=new QTreeWidgetItem(ui->treeWidget,lst,0);
    QVariant lev(0);
    pItem->setData(0,Qt::UserRole,lev);
    ui->treeWidget->expandAll();// Развернуть элементы дерева
    ui->treeWidget->setCurrentItem(pItem);//Делаем кореневой элемент текущим
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
    showInTree(START,ui->treeWidget);
    return;
}

void MainWindow::on_action_AddEl_triggered()
{
    if (!Created){return;}
    int posAdd=0;
    int pos=0;
    bool findOk;
    extern void** START;
    QTreeWidgetItem* cur_item = ui->treeWidget->currentItem();//Уровень выбранного элемента
    int lvl=cur_item->data(0,Qt::UserRole).toInt();
    if (lvl>2) return; //Выход, если уровень больше 2

    QTreeWidgetItem* pItem = new QTreeWidgetItem(cur_item);//Запрос места для нового элемента дерева
    QVariant lvlnew(lvl+1);
    pItem->setData(0,Qt::UserRole,lvlnew);
    switch(lvl+1){
    case 1:{ //узел 1 уровня
    Dialog_1 *dialog1 = new Dialog_1();//Create window
    dialog1->setModal(true);//Установка диалога модальным
    if (dialog1->exec()==1){ //Откыртие окна
        void* pnew=new TSpec;//Создание нового элемента
        QString specname=dialog1->getLineEditName();
        ((TSpec*)pnew)->name=specname;
        ((TSpec*)pnew)->code=dialog1->getLineEditCode();
        ((TSpec*)pnew)->time=dialog1->getLineEditTime();
        ((TSpec*)pnew)->Sub=InitArray();
        findEl(START,specname,posAdd, findOk,compSpec);
        addEl(START,pnew,posAdd);
        pItem->setText(0, specname);
        delete dialog1;
    }else {delete(pItem);}break;
    }
    case 2:{ //узел 2 уровня
    {
    Dialog_2 *dialog2 = new Dialog_2();
    dialog2->setModal(true);
    if (dialog2->exec()==1){
        findEl(START, cur_item->text(0), pos, findOk, compSpec);
        if (!findOk) return;
        void** start2 = ((TSpec*)(START[pos]))->Sub;
        TSubj* pnew = new TSubj;
        QString name2=dialog2->getLineEditName();
        (pnew)->name = name2;
        (pnew)->dur = dialog2->getLineEditDur();
        (pnew)->fio=dialog2->getLineEditFio();
        (pnew)->semestr=dialog2->getLineEditSemestr();
        (pnew)->Sub = InitArray();
        findEl(start2,name2, posAdd, findOk,compSubj);
        addEl(start2, pnew, posAdd);
        ((TSpec*)(START[pos]))->Sub = start2;
        /*QMessageBox ms;
        QString c = QString("CNT %1; SIZE %2").arg((((int*)start2)[POS_CNT])).arg((((int*)start2)[POS_SIZE]));
        ms.setText(c);ms.exec();*/
        pItem->setText(0,name2);
        delete dialog2;
    }
    else {delete pItem;};
    break;
    }
    }
    case 3: {
         Dialog_3 *dialog3 = new Dialog_3();
         dialog3->setModal(true);
         if (dialog3->exec()==1){
             findEl(START, cur_item->parent()->text(0), posAdd, findOk, compSpec);
                    if (!findOk) return;
                     void** start2=((TSpec*)START[posAdd])->Sub;
             findEl(start2, cur_item->text(0), pos, findOk, compSubj);
                if (!findOk) return;
                void ** start3=((TSubj*)start2[pos])->Sub;
                void* pnew=new TLit;
                QString name3=dialog3->getLineEditName();
                ((TLit*)pnew)->name= name3;
                ((TLit*)pnew)->aut=dialog3->getLineEditAut();
                ((TLit*)pnew)->year=dialog3->getLineEditYear();
                findEl(start3, name3, posAdd, findOk, compLit);
                addEl(start3, pnew,posAdd);
                ((TSubj*)start2[pos])->Sub=start3;
                pItem->setText(0, name3);
                delete(dialog3);
         } else{
             delete pItem;};
         break;
    }

    }
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
    ui->treeWidget->expandAll();
    showInTree(START,ui->treeWidget);
}

void MainWindow::on_action_DelEl_triggered()
{
    if (!Created){return;}
    extern void** START;
    int pos=0;
    bool findOk;
    QTreeWidgetItem* cur_item = ui->treeWidget->currentItem();//Уровень выбранного элемента
    int lvl=cur_item->data(0,Qt::UserRole).toInt();
    Dialog_del *dialog1 = new Dialog_del();//Create window
    dialog1->setModal(true);//Установка диалога модальным
    if (dialog1->exec()==1){ //Откыртие окна
          if (lvl==0){ //корневой уровень
              delList(START); //удаление всего списка
              Created=false;
              MainWindow::on_treeWidget_itemSelectionChanged();
              ui->treeWidget->clear();
              ui->tableWidget->clear();
              ui->tableWidget->setColumnCount(0);
              ui->tableWidget->setRowCount(0);
              return;
        }
        if(lvl==1){
            delEl(START,cur_item->text(0),1);
        }

        if (lvl==2){
            findEl(START,cur_item->parent()->text(0),pos,findOk,compSpec);
            if (!findOk)return;
                   void** stLeaf=((TSpec*)START[pos])->Sub;
                   delEl(stLeaf, cur_item->text(0),2);
        }
        if (lvl==3){
            findEl(START,cur_item->parent()->parent()->text(0),pos,findOk,compSpec);
            if (!findOk)return;
            void** stLeaf1=((TSpec*)START[pos])->Sub;
            findEl(stLeaf1,cur_item->parent()->text(0),pos,findOk,compSubj);
            if (!findOk)return;
            void** stLeaf2=((TSubj*)stLeaf1[pos])->Sub;
            QString name=cur_item->text(0);
            findEl(stLeaf2, name,pos,findOk,compLit);
            delEl(stLeaf2,name,3);
        }
    }
    delete cur_item;
    showInTree(START,ui->treeWidget);
}

void MainWindow::on_action_change_triggered()
{
    if (!Created){return;}
    extern void** START;
    int pos=0;
    bool findOk;
    QTreeWidgetItem* cur_item = ui->treeWidget->currentItem();//Уровень выбранного элемента
    int lvl=cur_item->data(0,Qt::UserRole).toInt();

        if(lvl==1){
            Dialog_1 *dialog1 = new Dialog_1();//Create window
            dialog1->setModal(true);//Установка диалога модальным
            findEl(START, cur_item->text(0), pos, findOk, compSpec);
            dialog1->setLineEditName(((TSpec*)START[pos])->name);
            dialog1->setLineEditCode(((TSpec*)START[pos])->code);
            dialog1->setLineEditTime(((TSpec*)START[pos])->time);
            if (dialog1->exec()==1){ //Откыртие окна
                void* pnew=new TSpec;
                QString specname=dialog1->getLineEditName();
                ((TSpec*)pnew)->name=specname;
                ((TSpec*)pnew)->code=dialog1->getLineEditCode();
                ((TSpec*)pnew)->time=dialog1->getLineEditTime();
                ((TSpec*)pnew)->Sub=((TSpec*)START[pos])->Sub;
            int cnt=((int*)START)[POS_CNT];
            delete((TSpec*)START[pos]);
            void* p1=START+pos;
            void* p2=START+pos+1;
            memmove(p1,p2,(cnt-pos)*4);
            ((int*)START)[POS_CNT]--;
            findEl(START,specname,pos, findOk,compSpec);
            addEl(START,pnew,pos);
            }
        }

        if (lvl==2){
            int pos2=0;
            findEl(START,cur_item->parent()->text(0),pos2,findOk,compSpec);
            if (!findOk)return;
                   Dialog_2 *dialog2 = new Dialog_2();//Create window
                   dialog2->setModal(true);
                   void** start2=((TSpec*)START[pos2])->Sub;
                   findEl(start2, cur_item->text(0), pos, findOk, compSubj);
                   dialog2->setLineEditName(((TSubj*)start2[pos])->name);
                   dialog2->setLineEditDur(((TSubj*)start2[pos])->dur);
                   dialog2->setLineEditFio(((TSubj*)start2[pos])->fio);
                   dialog2->setLineEditSemestr(((TSubj*)start2[pos])->semestr);
                   if (dialog2->exec()==1){
                   void* pnew=new TSubj;
                   QString name2=dialog2->getLineEditName();
                   ((TSubj*)pnew)->name = name2;
                   ((TSubj*)pnew)->dur = dialog2->getLineEditDur();
                   ((TSubj*)pnew)->fio=dialog2->getLineEditFio();
                   ((TSubj*)pnew)->semestr=dialog2->getLineEditSemestr();
                   ((TSubj*)pnew)->Sub=((TSubj*)start2[pos])->Sub;

                   int cnt=((int*)start2)[POS_CNT];
                   delete((TSubj*)start2[pos]);
                   void* p1 = start2 + pos + 1;
                   void* p2 = start2 + pos;
                   memmove(p2, p1, (cnt-pos)*4);
                   ((int*)start2)[POS_CNT]--;

                   ((TSpec*)START[pos2])->Sub=start2;
                   findEl(start2,name2, pos, findOk,compSubj);
                   addEl(start2, pnew, pos);
        }
        }
        if (lvl==3){
            int pos3=0;
            findEl(START, cur_item->parent()->parent()->text(0), pos, findOk, compSpec);
            if (!findOk) return;
            Dialog_3 *dialog3 = new Dialog_3();//Create window
            dialog3->setModal(true);
            void** start2 = ((TSpec*)(START[pos]))->Sub;
            findEl(start2, cur_item->parent()->text(0), pos3, findOk, compSubj);
            void** start3 = ((TSubj*)(start2[pos3]))->Sub;
            findEl(start3, cur_item->text(0), pos, findOk, compLit);
            dialog3->setLineEditName(((TLit*)start3[pos])->name);
            dialog3->setLineEditAut(((TLit*)start3[pos])->aut);
            dialog3->setLineEditYear(((TLit*)start3[pos])->year);
               if (dialog3->exec()==1){
                   void* pnew=new TLit;
                   QString name3=dialog3->getLineEditName();
                   ((TLit*)pnew)->name= name3;
                   ((TLit*)pnew)->aut=dialog3->getLineEditAut();
                   ((TLit*)pnew)->year=dialog3->getLineEditYear();

                   int cnt=((int*)start3)[POS_CNT];
                   delete((TLit*)start3[pos]);
                   void* p1=start3+pos+1;
                   void* p2=start3+pos;
                   memmove(p2,p1,(cnt-pos)*4);
                   ((int*)start3)[POS_CNT]--;
                   ((TSubj*)start2[pos3])->Sub=start3;
                   findEl(start3,name3, pos, findOk,compLit);
                   addEl(start3, pnew, pos);
               }
        }
    delete cur_item;
    showInTree(START,ui->treeWidget);

}

void MainWindow::on_action_find_triggered()
{
    if (!Created) return;
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
    int cnt1=((int*)START)[POS_CNT]; //Количество элементов 1 уровня
    QString str; //Строка с подробной информацией об элементе
    if(cnt1==0)return; //Если нету ни одного элемента - выйти
        QMessageBox msgBox; // Создаем окно для вывода информации и задаем параметры
        msgBox.setMinimumWidth(400);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::No);
    Dialog_input *dialog = new Dialog_input();//Создания окна для ввода данных для поиска
    dialog->setModal(true); //Установка окна модальным
     if (dialog->exec()==1){
         QString key=dialog->getLineEditName(); //Получение ключа для поиска
          if (key=="")return;
          for(int i=0;i<cnt1;i++){ //Проход циклом по первому уровню
                if (((TSpec*)START[i])->name==key){
                str.sprintf("Имя - %s\nКод - %s\nДлительность обучения - %s",((TSpec*)START[i])->name.toAscii().constData(),((TSpec*)START[i])->code.toAscii().constData(),((TSpec*)START[i])->time.toAscii().constData());
                msgBox.setWindowTitle("Специальность");
                msgBox.setText(str);
                msgBox.exec(); //Вывод данных
                }
              void** start2=((TSpec*)START[i])->Sub; //Вход в 2 подуровень
              int cnt2=((int*)start2)[POS_CNT]; //Количество элементов 2 уровня
              for (int k=0;k<cnt2;k++){//Проход циклом по второму уровню
                  if (((TSubj*)start2[k])->name==key){
                    str.sprintf("Имя - %s\nКоличество часов - %s\nФИО преподавателя - %s\nСеместр - %s",((TSubj*)start2[k])->name.toAscii().constData(),((TSubj*)start2[k])->dur.toAscii().constData(),((TSubj*)start2[k])->fio.toAscii().constData(),((TSubj*)start2[k])->semestr.toAscii().constData());
                    msgBox.setWindowTitle("Предмет");
                    msgBox.setText(str);
                    msgBox.exec(); //Вывод данных
                    }
                  void** start3=((TSubj*)start2[k])->Sub; //Вход в 3 подуровень
                  int cnt3=((int*)start3)[POS_CNT]; //Количество элементов 3 уровня
                  for (int j=0;j<cnt3;j++){//Проход циклом по третьему уровню
                        if (((TLit*)start3[j])->name==key){
                        str.sprintf("Имя - %s\nАвтор - %s\nГод издания - %s",((TLit*)start3[j])->name.toAscii().constData(),((TLit*)start3[j])->aut.toAscii().constData(),((TLit*)start3[j])->year.toAscii().constData());
                        msgBox.setWindowTitle("Литература");
                        msgBox.setText(str);
                        msgBox.exec(); //Вывод данных
                        }
                  }
              }
          }
     }
     delete dialog;
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    extern void** START;
    if (START == NULL) return;

    QTreeWidgetItem* cur_item = ui->treeWidget->currentItem();
    if (cur_item == NULL) return;
    int lvl = cur_item->data(0, Qt::UserRole).toInt();

    showInTable(START, ui->tableWidget, cur_item, lvl);
}

void MainWindow::on_action_findLit_triggered()
{
    if (START == NULL) return;
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
        int pos;
        int count=0;
        QString inName;
        QMessageBox ms;
        bool ok;
        QTableWidgetItem* p;
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnWidth(0, ui->tableWidget->width()/2.5);
        ui->tableWidget->setColumnWidth(1, ui->tableWidget->width()/2.5);
        ui->tableWidget->setColumnWidth(2, ui->tableWidget->width()/6.1);
            QStringList lablist;
            lablist <<"Имя" << "ФИО автора" << "Год издания";
            ui->tableWidget->setHorizontalHeaderLabels(lablist);
        Dialog_input *dialog = new Dialog_input();//Создания окна для ввода данных для поиска
        dialog->setModal(true); //Установка окна модальным
        dialog->setboxname("Введите специальность для поиска");
         if (dialog->exec()==1){
             QString key=dialog->getLineEditName();
             if (key=="")return;
             findEl(START, key, pos, ok, compSpec);
             if (!ok){
                 QString c = QString("Специальность не найдена");
                 ms.setText(c);ms.exec();
                 return;
             }
             void** start2=((TSpec*)START[pos])->Sub; //Вход в подуровень
             int cnt2=((int*)start2)[POS_CNT]; //Количество элементов 2 уровня
             if (cnt2==0){
                 QString c = QString("У специальности нет литературы");
                 ms.setText(c);ms.exec();
                 return;}
             for (int i=0;i<cnt2;i++){
                 void** start3=((TSubj*)start2[i])->Sub; //Вход в подуровень
                 int cnt3=((int*)start3)[POS_CNT]; //Количество элементов 3 уровня
                 if (cnt3==0)continue;
                 for (int k=0; k<cnt3; k++){
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                    inName = ((TLit*)start3[k])->name;
                    p = new QTableWidgetItem(inName);
                    ui->tableWidget->setItem(count, 0, p);
                    inName = ((TLit*)start3[k])->aut;
                    p = new QTableWidgetItem(inName);
                    ui->tableWidget->setItem(count, 1, p);
                    inName = ((TLit*)start3[k])->year;
                    p = new QTableWidgetItem(inName);
                    ui->tableWidget->setItem(count, 2, p);
                    count++;
                 }

             }
} else return;
         if (ui->tableWidget->rowCount()==0){
             QString c = QString("У специальности нет литературы");
             ms.setText(c);ms.exec();
         }
}

void MainWindow::on_action_findSubj_triggered()
{
    if (START == NULL) return;
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
        int pos;
        int count=0;
        QString inName;
        QMessageBox ms;
        bool ok;
        int cnt1,cnt2,cnt3;
        void** sub2;
        void** sub3;
        QTableWidgetItem* p;
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(4);
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnWidth(0, ui->tableWidget->width()/2.5);
        ui->tableWidget->setColumnWidth(1, ui->tableWidget->width()/2.5);
        ui->tableWidget->setColumnWidth(2, ui->tableWidget->width()/2.5);
        ui->tableWidget->setColumnWidth(2, ui->tableWidget->width()/2.5);
            QStringList lablist;
            lablist << "Название" << "Длительность обучения" << "ФИО преподавателя" << "Семестр";
            ui->tableWidget->setHorizontalHeaderLabels(lablist);
        Dialog_input *dialog = new Dialog_input();//Создания окна для ввода данных для поиска
        dialog->setModal(true); //Установка окна модальным
        dialog->setboxname("Введите литературу для поиска");
         if (dialog->exec()==1){
             QString key=dialog->getLineEditName();
             if (key=="")return;
             cnt1=((int*)START)[POS_CNT];
             if (cnt1==0){return;}
                 for(int i=0;i<cnt1;i++){
                     sub2=((TSpec*)START[i])->Sub;
                     cnt2=((int*)sub2)[POS_CNT];
                     for(int j=0;j<cnt2;j++){
                         sub3=((TSubj*)sub2[j])->Sub;
                         cnt3=((int*)sub3)[POS_CNT];
                         if (cnt3==0) continue;
                         findEl(sub3,key,pos,ok,compLit);
                         if (ok){
                             ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                             inName = ((TSubj*)sub2[j])->name;
                             p = new QTableWidgetItem(inName);
                             ui->tableWidget->setItem(count, 0, p);
                             inName = ((TSubj*)sub2[j])->dur;
                             p = new QTableWidgetItem(inName);
                             ui->tableWidget->setItem(count, 1, p);
                             inName = ((TSubj*)sub2[j])->fio;
                             p = new QTableWidgetItem(inName);
                             ui->tableWidget->setItem(count, 2, p);
                             inName = ((TSubj*)sub2[j])->semestr;
                             p = new QTableWidgetItem(inName);
                             ui->tableWidget->setItem(count, 3, p);
                             count++;
                         }
                 }
             }
} else return;
         if (ui->tableWidget->rowCount()==0){
             QString c = QString("У специальности нет литературы");
             ms.setText(c);ms.exec();
         }

}

void MainWindow::on_action_open_triggered()
{
    if (!Created) return;
    extern void** START;
    //QString fname = QFileDialog::getOpenFileName(this,tr("Load"),QDir::currentPath(),tr("Data (*.db)") );
    Dialog_input *dialog = new Dialog_input();//Создания окна для ввода данных для поиска
    dialog->setModal(true); //Установка окна модальным
    dialog->setboxname("Введите имя для загрузки");
    dialog->setformname("Load");
     if (dialog->exec()==1){
         QString name=dialog->getLineEditName();
         if (name=="")return;
         name+=".db";
        if (loadArray(START, name.toLocal8Bit().data()) == false){
            };
     }
    showInTree(START, ui->treeWidget);
}

void MainWindow::on_action_save_triggered()
{
    if (!Created) return;
    extern void** START;
    //QString fname = QFileDialog::getSaveFileName(this, tr("Save"), QDir::currentPath(),tr("Data (*.db)"));
    Dialog_input *dialog = new Dialog_input();//Создания окна для ввода данных для поиска
    dialog->setModal(true); //Установка окна модальным
    dialog->setboxname("Введите имя для сохранения");
    dialog->setformname("Save");
     if (dialog->exec()==1){
         QString name=dialog->getLineEditName();
         if (name=="")return;
         name+=".db";
        if (saveArray(START, name.toLocal8Bit().data()) == false){
            }
     }
}



void MainWindow::on_actionDop_triggered()
{
    /*QMessageBox ms;
     QString c = QString("ZE ");
      ms.setText(c);ms.exec();*/
    if (Created==false){return;};
    extern void** START;
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));//Установка русской кодировки

    DialogDop *dialog = new DialogDop();//Создания окна для ввода данных для поиска
    dialog->setModal(true); //Установка окна модальным
     dialog->show();
     dialog->InitTree();
     dialog->ShowTree(START);




}
