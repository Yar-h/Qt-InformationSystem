#include <QString>
#ifndef FUNC_H
#define FUNC_H
#include <types.h>
#include <QTreeWidget>
#include <QTableWidget>


int compSpec(void* p, QString key);
int compSubj(void* p, QString key);
int compLit(void* p, QString key);

void** InitArray ();
void expandArr (void**  &start);
void addEl(void** &start,  void* pnew, int posAdd);
void delEl(void** &start, QString key, int lvl);
void delList(void** &start);
void findEl(void** start, QString key, int &posFndEl, bool &findOk, TPFunc pCompare);
void showInTree(void** start, QTreeWidget* twg);
void* TakeEl(void** &start,int pos);
void showInTable(void** &start, QTableWidget* tbwg, QTreeWidgetItem* current, int lvl);
bool loadArray(void** &start, char* fname);
bool saveArray(void** start, char* fname);

#endif // FUNC_H
