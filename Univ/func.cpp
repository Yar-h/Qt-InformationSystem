#include "func.h"
#include <stdio.h>
#include <QMessageBox>
#include <QFile>

int compSpec(void* p, QString key){
    return QString::localeAwareCompare(((TSpec*)p)->name, key);
}
int compSubj(void* p, QString key){
    return QString::localeAwareCompare(((TSubj*)p)->name, key);
}
int compLit(void* p, QString key){
    return QString::localeAwareCompare(((TLit*)p)->name, key);
}

void** InitArray (){
    void** start= new void*[SIZE_ARR+2];
    start+=2;
    ((int*)start)[POS_CNT]=0;
    ((int*)start)[POS_SIZE]=SIZE_ARR;
    return start;
}

void expandArr (void**  &start){
    int size_old=((int*)start)[POS_SIZE];
    int cnt_old=((int*)start)[POS_CNT];
    void** start2= new void*[size_old+DELTA+2];
    void* p1=start-2;
    void* p2=start2;
    memmove(p2,p1,(size_old*sizeof(void*)+2*sizeof(int)));
    start2=start2+2;
    ((int*)start2)[POS_CNT]=cnt_old;
    ((int*)start2)[POS_SIZE]=size_old+DELTA;
    free(start);
    start=start2;
}

void addEl(void** &start,  void* pnew, int posAdd){
    int size=((int*)start)[POS_SIZE];
    int count=((int*)start)[POS_CNT];
    if (size==count){expandArr(start);}
    if (posAdd==count){start[posAdd]=pnew;} else
    {
        void* pos1=start+posAdd;
        void* pos2=start+posAdd+1;
        memmove(pos2,pos1,(4*(count-posAdd)));
        start[posAdd]=pnew;
    }
    count++;
    ((int*)start)[POS_CNT]=count;
}

void delEl(void** &start, QString key, int lvl){
    int pos,cnt,cnt2,cnt3;
    bool ok;
    void** sub2;
    void** sub3;
    cnt=((int*)start)[POS_CNT];
    if (lvl==1){
        findEl(start,key,pos,ok,compSpec);
        if((cnt==0)||(ok==false)||(pos<0)){return;}
        sub2=((TSpec*)start[pos])->Sub;
        cnt2=((int*)sub2)[POS_CNT];
        for(int i=0;i<cnt2;i++){
            sub3=((TSubj*)sub2[i])->Sub;
            cnt3=((int*)sub3)[POS_CNT];
            for(int j=0;j<cnt3;j++){
                delete((TLit*)sub3[j]);
            }
            delete((TSubj*)sub2[i]);
        }
        delete((TSpec*)start[pos]);
    void* p1=start+pos;
    void* p2=start+pos+1;
    memmove(p1,p2,(cnt-pos)*4);
    ((int*)start)[POS_CNT]--;
    }
    if (lvl == 2){
        findEl(start, key, pos, ok, compSubj);
        if ( (cnt==0) || (pos<0) || !ok) return;
        void** sub3 = ((TSubj*)start[pos])->Sub;
        int cnt3 = ((int*)sub3)[POS_CNT];
        for (int i=0; i<cnt3; i++){
            delete (TLit*)(sub3[i]);
        }
        delete (TSubj*)start[pos];
        void* p1 = start + pos + 1;
        void* p2 = start + pos;
        memmove(p2, p1, (cnt-pos)*4);
        ((int*)start)[POS_CNT]--;
    }
    if (lvl==3){
        findEl(start,key,pos,ok,compLit);
        if((cnt==0)||(ok==false)||(pos<0)){return;}
        delete((TLit*)start[pos]);
        start[pos]=NULL;
        void* p1=start+pos+1;
        void* p2=start+pos;
        memmove(p2,p1,(cnt-pos)*4);
        ((int*)start)[POS_CNT]--;
    }
}

void delList(void** &start){
    if (start==NULL) return;
    int cnt1,cnt2,cnt3;
    void** sub2;
    void** sub3;
    cnt1=((int*)start)[POS_CNT];
    if (cnt1==0){return;}
    for(int i=0;i<cnt1;i++){
        sub2=((TSpec*)start[i])->Sub;
        cnt2=((int*)sub2)[POS_CNT];
        for(int j=0;j<cnt2;j++){
            sub3=((TSubj*)sub2[j])->Sub;
            cnt3=((int*)sub3)[POS_CNT];
            for(int k=0;k<cnt3;k++){
                delete((TLit*)sub3[k]);
            }
            free(sub3);
            delete((TSubj*)sub2[j]);
        }
        free(sub2);
        delete((TSpec*)start[i]);
    }
  ((int*)start)[POS_CNT]=0;
  free(start);
}

void findEl(void** start, QString key, int &posFndEl, bool &findOk, TPFunc pCompare){
    int hi,low,mid;
    findOk=false;
    if (((int*)start)[POS_CNT]==0){posFndEl=0; return;}
    low=0;
    hi=((int*)start)[POS_CNT]-1;
    do{
        mid=(hi+low)/2;
        switch(pCompare(start[mid],key)){
        case 0: posFndEl=mid;
                findOk=true;
                return;
        case 1: hi=mid-1;
                break;
        case -1: low=mid+1;
                break;
        }
    } while (low<=hi);
posFndEl=low;
}

void showInTree(void** start, QTreeWidget* treeW){
    treeW->clear();
    QStringList lst;
    lst << "Список";
    QTreeWidgetItem* pItem = new QTreeWidgetItem(treeW,lst,0);
    QVariant lvl(0);//Объект класса QVariant, уровень - корень
    pItem->setData(0, Qt::UserRole, lvl);
    treeW->setCurrentItem(pItem);
    int cnt1 = ((int*)start)[POS_CNT];
    if (cnt1==0) {return;};
    int cnt2, cnt3;
    QString lvl1Name; // 1 уровень
    QString lvl2Name; // 2 уровень
    QString lvl3Name; // 3 уровень
    void** sublvl2; //Указатели на подуровни
    void** sublvl3;
    QTreeWidgetItem* pItem1; //Указатели на новые элементы дерева
    QTreeWidgetItem* pItem2;
    QTreeWidgetItem* pItem3;
    QVariant lvl_1(1); //Объект класса QVariant, уровень - 1
    QVariant lvl_2(2); //Объект класса QVariant, уровень - 2
    QVariant lvl_3(3); //Объект класса QVariant, уровень - 3

    for (int i=0; i<cnt1; i++){ //Вывод первого уровня
        lvl1Name = ((TSpec*)start[i])->name;
        pItem1 = new QTreeWidgetItem(pItem);
        pItem1->setData(0, Qt::UserRole, lvl_1);
        pItem1->setText(0, lvl1Name);
        sublvl2 = ((TSpec*)start[i])->Sub;
        cnt2 = ((int*)sublvl2)[POS_CNT];
        /*QMessageBox ms;
        QString c = QString("CNT %1; SIZE %2").arg((((int*)sublvl2)[POS_CNT])).arg((((int*)sublvl2)[POS_SIZE]));
        ms.setText(c);ms.exec();*/
        if (cnt2 > 0){ //Вывод второго уровня
            for (int j=0; j<cnt2; j++){
                lvl2Name = ((TSubj*)sublvl2[j])->name;
                pItem2 = new QTreeWidgetItem(pItem1);
                pItem2->setData(0, Qt::UserRole, lvl_2);
                pItem2->setText(0, lvl2Name);
                sublvl3 = ((TSubj*)sublvl2[j])->Sub;
                cnt3 = ((int*)sublvl3)[POS_CNT];
                if (cnt3 > 0){ //Вывод третьего уровня
                    for (int k=0; k<cnt3; k++){
                        lvl3Name = ((TLit*)sublvl3[k])->name;
                        pItem3 = new QTreeWidgetItem(pItem2);
                        pItem3->setData(0, Qt::UserRole, lvl_3);
                        pItem3->setText(0, lvl3Name);
                    }
                }
                }
        }
    }
treeW->expandAll();
}

void* TakeEl(void** &start,int pos){
    void* p=start[pos];
    int cnt = ((int*)start)[POS_CNT];
    void** p1=start+pos;
    void** p2=start+pos+1;
    memmove(p1,p2,4*(cnt-pos));
    ((int*)start)[POS_CNT]--;
    return p;
}

void showInTable(void** &start, QTableWidget* tbwg, QTreeWidgetItem* current, int lvl){
if (start == NULL) return;
int cnt = ((int*)start)[POS_CNT];
int pos;
QString inName;
bool ok;
QTableWidgetItem* p;
if (lvl == 0){
    if (cnt == 0){
        tbwg->clear();
        tbwg->setColumnCount(0);
        tbwg->setRowCount(0);
        return;
    }
    tbwg->setColumnCount(3);
    tbwg->setRowCount(cnt);
    tbwg->setColumnWidth(0, tbwg->width()/2.052);
    tbwg->setColumnWidth(1, tbwg->width()/7);
    tbwg->setColumnWidth(2, tbwg->width()/3);
    QStringList lablist;
    lablist << "Название" << "Код" << "Длительность обучения";
    tbwg->setHorizontalHeaderLabels(lablist);
    for (int i=0; i<cnt; i++){
        inName = ((TSpec*)start[i])->name;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 0, p);
        inName = ((TSpec*)start[i])->code;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 1, p);
        inName = ((TSpec*)start[i])->time;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 2, p);
    }
}
if (lvl == 1){
    findEl(start, current->text(0), pos, ok, compSpec);
    void** sublvl2 = ((TSpec*)start[pos])->Sub;
    if (sublvl2 == NULL) return;
    cnt = ((int*)sublvl2)[POS_CNT];
    if (cnt == 0){
        tbwg->clear();
        tbwg->setColumnCount(0);
        tbwg->setRowCount(0);
        return;
    }
    tbwg->setColumnCount(4);
    tbwg->setRowCount(cnt);
    tbwg->setColumnWidth(0, tbwg->width()/2.5);
    tbwg->setColumnWidth(1, tbwg->width()/2.5);
    tbwg->setColumnWidth(2, tbwg->width()/2.5);
    tbwg->setColumnWidth(2, tbwg->width()/2.5);
    QStringList lablist;
    lablist << "Название" << "Длительность обучения" << "ФИО преподавателя" << "Семестр";
    tbwg->setHorizontalHeaderLabels(lablist);
    for (int i=0; i<cnt; i++){
        inName = ((TSubj*)sublvl2[i])->name;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 0, p);
        inName = ((TSubj*)sublvl2[i])->dur;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 1, p);
        inName = ((TSubj*)sublvl2[i])->fio;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 2, p);
        inName = ((TSubj*)sublvl2[i])->semestr;
        p = new QTableWidgetItem(inName);
        tbwg->setItem(i, 3, p);
    }
}

if (lvl == 2){
    findEl(start, current->parent()->text(0), pos, ok, compSpec);
    void** sublvl2 = ((TSpec*)start[pos])->Sub;
    if (sublvl2 == NULL) return;
    findEl(sublvl2, current->text(0), pos, ok,compSubj);
    void** sublvl3 = ((TSubj*)sublvl2[pos])->Sub;
    if (sublvl3 == NULL) return;
    cnt = ((int*)sublvl3)[POS_CNT];
    if (cnt == 0){
        tbwg->clear();
        tbwg->setColumnCount(0);
        tbwg->setRowCount(0);
        return;
    }
    tbwg->setColumnCount(3);
    tbwg->setRowCount(cnt);
    tbwg->setColumnWidth(0, tbwg->width()/2.5);
    tbwg->setColumnWidth(1, tbwg->width()/2.5);
    tbwg->setColumnWidth(2, tbwg->width()/6.1);
    QStringList lablist;
    lablist <<"Имя" << "ФИО автора" << "Год издания";
    tbwg->setHorizontalHeaderLabels(lablist);
    for (int i=0; i<cnt; i++){
    inName = ((TLit*)sublvl3[i])->name;
    p = new QTableWidgetItem(inName);
    tbwg->setItem(i, 0, p);
    inName = ((TLit*)sublvl3[i])->aut;
    p = new QTableWidgetItem(inName);
    tbwg->setItem(i, 1, p);
    inName = ((TLit*)sublvl3[i])->year;
    p = new QTableWidgetItem(inName);
    tbwg->setItem(i, 2, p);
    }
}

if (lvl == 3){
    findEl(start, current->parent()->parent()->text(0), pos, ok, compSpec);
    void** sublvl2 = ((TSpec*)start[pos])->Sub;
    if (sublvl2 == NULL) return;
    findEl(sublvl2, current->parent()->text(0), pos, ok,compSubj);
    void** sublvl3 = ((TSubj*)sublvl2[pos])->Sub;
    if (sublvl3 == NULL) return;
    findEl(sublvl3, current->text(0), pos, ok, compLit);
    tbwg->setColumnCount(3);
    tbwg->setRowCount(1);
    tbwg->setColumnWidth(0, tbwg->width()/2.5);
    tbwg->setColumnWidth(1, tbwg->width()/2.5);
    tbwg->setColumnWidth(2, tbwg->width()/6.1);
    QStringList lablist;
    lablist <<"Имя" << "ФИО автора" << "Год издания";
    tbwg->setHorizontalHeaderLabels(lablist);
    inName = ((TLit*)sublvl3[pos])->name;
    p = new QTableWidgetItem(inName);
    tbwg->setItem(0, 0, p);
    inName = ((TLit*)sublvl3[pos])->aut;
    p = new QTableWidgetItem(inName);
    tbwg->setItem(0, 1, p);
    inName = ((TLit*)sublvl3[pos])->year;
    p = new QTableWidgetItem(inName);
    tbwg->setItem(0, 2, p);
}
}
/////////////////
/* Save & Load */
/////////////////

bool saveArray(void** start, char* fname){
    FILE* f;
    if ((f = fopen(fname, "w+b")) == NULL ) return false;

    FTSpec lvl1;
    FTSub lvl2;
    FTLit lvl3;
    int cnt2, cnt3;
    void** sub2;
    void** sub3;
    int cnt1=((int*)start)[POS_CNT];
    if (cnt1 == 0) {fclose(f); return true;};

    fwrite(&cnt1, sizeof(cnt1), 1, f);
    for (int i=0; i<cnt1; i++){
        strcpy(lvl1.name, ((TSpec*)start[i])->name.toLocal8Bit().data());
        strcpy(lvl1.code, ((TSpec*)start[i])->code.toLocal8Bit().data());
        strcpy(lvl1.time, ((TSpec*)start[i])->time.toLocal8Bit().data());
        cnt2 = (int)((((TSpec*)start[i])->Sub)[POS_CNT]);
        lvl1.subnum = cnt2;
        fwrite(&lvl1, sizeof(FTSpec), 1, f);
        sub2 = ((TSpec*)start[i])->Sub;
        for (int j=0; j<cnt2; j++){
            strcpy(lvl2.name, ((TSubj*)sub2[j])->name.toLocal8Bit().data());
            strcpy(lvl2.fio, ((TSubj*)sub2[j])->fio.toLocal8Bit().data());
            strcpy(lvl2.dur, ((TSubj*)sub2[j])->dur.toLocal8Bit().data());
            strcpy(lvl2.semestr, ((TSubj*)sub2[j])->semestr.toLocal8Bit().data());
            cnt3 = (int)((((TSubj*)sub2[j])->Sub)[POS_CNT]);
            lvl2.subnum = cnt3;
            fwrite (&lvl2,sizeof(FTSub), 1, f);
            sub3 = ((TSubj*)sub2[j])->Sub;
            for (int k=0; k<cnt3; k++){
                strcpy(lvl3.name, ((TLit*)sub3[k])->name.toLocal8Bit().data());
                strcpy(lvl3.aut, ((TLit*)sub3[k])->aut.toLocal8Bit().data());
                strcpy(lvl3.year, (((TLit*)sub3[k])->year.toLocal8Bit().data()));
                fwrite (&lvl3,sizeof(FTLit), 1, f);
            };
        };
    };
    fclose(f);
    return true;
}

bool loadArray(void** &start, char* fname){
    FILE* f;
    if ((f = fopen(fname, "r+b")) == NULL ) return false;
    delList(start);

    FTSpec lvl1;
    FTSub lvl2;
    FTLit lvl3;
    int cnt1, cnt2, cnt3;
    void** sub2;
    void** sub3;
    void* pnew;

    fread(&cnt1, sizeof(cnt1), 1, f);
    if (cnt1 == 0){fclose(f); return false;};

    start = InitArray();
    for (int i=0; i<cnt1; i++){
        fread(&lvl1, sizeof(FTSpec), 1, f);
        pnew = new TSpec;
        ((TSpec*)pnew)->name = lvl1.name;
        ((TSpec*)pnew)->code= lvl1.code;
        ((TSpec*)pnew)->time= lvl1.time;
        ((TSpec*)pnew)->Sub = InitArray();
        addEl(start, pnew, i);
        cnt2 = lvl1.subnum;
        sub2 = ((TSpec*)start[i])->Sub;
        for (int j=0; j<cnt2; j++){
            fread(&lvl2, sizeof(FTSub), 1, f);
            pnew = new TSubj;
            ((TSubj*)pnew)->name= lvl2.name;
            ((TSubj*)pnew)->fio= lvl2.fio;
            ((TSubj*)pnew)->dur= lvl2.dur;
            ((TSubj*)pnew)->semestr= lvl2.semestr;
            ((TSubj*)pnew)->Sub = InitArray();
            addEl(sub2, pnew, j);
            cnt3 = lvl2.subnum;
            sub3 = ((TSubj*)sub2[j])->Sub;
            for (int k=0; k<cnt3; k++){
                fread(&lvl3, sizeof(FTLit), 1, f);
                pnew = new TLit;
                ((TLit*)pnew)->name= lvl3.name;
                ((TLit*)pnew)->aut= lvl3.aut;
                ((TLit*)pnew)->year= lvl3.year;
                addEl(sub3, pnew, k);
            };
            ((TSubj*)sub2[j])->Sub=sub3;
        };
        ((TSpec*)start[i])->Sub=sub2;
    };
    fclose(f);
    return true;
}
