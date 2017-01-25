#include <QString>
#include <QTextCodec>
#ifndef TYPES_H
#define TYPES_H

#define SIZE_ARR 5  // первоначальный размер
#define DELTA 5     // количество, на которое  расширяется  массив
#define POS_SIZE -2 // индекс для служебного элемента size
#define POS_CNT -1  // индекс для служебного элемента cnt

typedef int (*TPFunc)(void* , QString);

typedef struct Spec{
    QString  name;
    QString  code;
    QString  time;
    void**   Sub;
}TSpec;

typedef struct Subj{
    QString  name;
    QString  dur;
    QString  fio;
    QString  semestr;
    void**   Sub;
}TSubj;

typedef struct Lit{
    QString    name;
    QString    aut;
    QString    year;
}TLit;


typedef struct FSpec{
    char  name[50];
    char  code[20];
    char  time[50];
    int subnum;
}FTSpec;

typedef struct FSub{
    char  name[50];
    char  dur[20];
    char  fio[50];
    char  semestr[20];
    int subnum;
}FTSub;

typedef struct FLit{
    char    name[50];
    char    aut[50];
    char    year[15];
}FTLit;

#endif // TYPES_H
