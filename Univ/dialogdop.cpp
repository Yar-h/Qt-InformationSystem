#include "dialogdop.h"
#include "ui_dialogdop.h"
#include <QMessageBox>
#include "types.h"

    bool exp=true;
    QTreeWidgetItem* pItem;

DialogDop::DialogDop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDop)
{
    ui->setupUi(this);
}

DialogDop::~DialogDop()
{
    delete ui;
}

void DialogDop::on_pushButton_clicked()
{
        setVisible(false);
}

void DialogDop::InitTree()
{
    ui->treeWidget1->setColumnCount(1);
    ui->treeWidget1->setHeaderLabel("");// Установить название колонки

    QStringList lst; //Корень
    lst<<"Список";
    pItem=new QTreeWidgetItem(ui->treeWidget1,lst,0);
    QVariant lev(0);
    pItem->setData(0,Qt::UserRole,lev);
    ui->treeWidget1->expandAll();// Развернуть элементы дерева
    ui->treeWidget1->setCurrentItem(pItem);//Делаем кореневой элемент текущим
    ui->treeWidget1->sortItems(0, Qt::AscendingOrder);
}

void DialogDop::ShowTree(void** start)
{
    QTreeWidgetItem* pItemP;
    QTreeWidgetItem* pItemP1;
    int cnt1, cnt2, cnt3;
    void** sub2;
    void** sub3;
    QString name;
    QStringList lst;
    bool add=true;

    cnt1=((int*)start)[POS_CNT];
    if (cnt1==0){return;}
    for(int i=0;i<cnt1;i++){
        sub2=((TSpec*)start[i])->Sub;
        cnt2=((int*)sub2)[POS_CNT];
        for(int j=0;j<cnt2;j++){
            sub3=((TSubj*)sub2[j])->Sub;
            cnt3=((int*)sub3)[POS_CNT];
            for(int k=0;k<cnt3;k++){
                add=true;
                name=((TLit*)sub3[k])->name;
                    for(int v=0;v<pItem->childCount();v++){
                        if (pItem->child(v)->text(0)==name){ add=false; break;}
                    }
                    if (add){ //esli literatyru esche nety
                pItemP = new QTreeWidgetItem(pItem);
                pItemP->setData(0, Qt::UserRole, 1);
                pItemP->setText(0, name);
                pItemP1 = new QTreeWidgetItem(pItemP);
                pItemP1->setData(0, Qt::UserRole, 2);
                pItemP1->setText(0, ((TSubj*)sub2[j])->name);
                    } else
                    { //esli literatyra est'
                        add=true;
                        for(int b=0;b<pItem->childCount();b++){ //idem po spisky literatyru
                            if (pItem->child(b)->text(0) == name){ //naihodim literatyry
                                for(int v=0;v<pItem->child(b)->childCount();v++){ //idem po spisky predmetov
                                    if (pItem->child(b)->child(v)->text(0) == ((TSubj*)sub2[j])->name){ add=false;
                                    }
                                    //esli dannuy predmet yje est'
                                }
                                if (add){ // esli predmeta nety dobavim ego
                                    pItemP=pItem->child(b);
                                    pItemP1 = new QTreeWidgetItem(pItemP);
                                    pItemP1->setData(0, Qt::UserRole, 2);
                                    pItemP1->setText(0, ((TSubj*)sub2[j])->name);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
void DialogDop::on_pushButton_2_clicked()
{
    if (exp){
    ui->treeWidget1->expandAll();
    exp=false;} else {
    ui->treeWidget1->reset();
    ui->treeWidget1->expandItem(pItem);
    exp=true;
    }
}
