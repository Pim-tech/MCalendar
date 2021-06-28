#ifndef MCALENDAR_H
#define MCALENDAR_H

#include <QTableWidget>
#include <QDebug>
#include <QDateTime>
#include <QMouseEvent>
#include "MDate.h"


class MCalendar : public QTableWidget
{
    Q_OBJECT

    QList<MDate> mdtlst;
    int index;
    int month;
    int year;
    int day;
    QDate currentDate;
    QDate selectedDate;
    int daysInMonth;
    int lastRowClicked;
    int lastColumnClicked;
    int lastLineClicked;

    MCalendar *prec;
    MCalendar *next;
    QObject *parentObj = 0;
    QVector<QPalette> savedPrecedentPalette;
    QVector<QFont> savedPrecedentFont;
    //Pleine lune	2 mars 2018	01:52:10
    const QFont *globalFont = 0;


    QLabel *createFearLabel(MDate &md);
    QLabel *createNumDayLabel(MDate &md);
    QLabel *createDayLabel(MDate &md);
    QLabel *createMoonLabel(MDate &md);
    QLabel *createDtMoonLabel(MDate &md);

    void init();
    void initTable();
    int loadDays();

    void autoresize();
    QLabel * searchLabel(int row,int column);

public:
    explicit MCalendar();
    explicit MCalendar(int year, int month);
    explicit MCalendar(const QDate &d);
    QList<MDate> * getData();
    void setParentObj(QObject *parent);
    QObject * getParentObj();
    void setPrec(MCalendar *prev);
    void setNext(MCalendar *next);
    void reDrawTable();    
    void storeMoon(double decimalDay,int moonType);
    int selectCell(int row, int col, bool unselect=true);
    int unselectCell(int row,int col);
    int selectLine(int row);
    int unselectLine(int row);
    void setIndex(int i);
    void reset(int year, int month);
    int getMonth();
    int getYear();
    int getDay();
    int getIndex();

    QDate getDate();
    QDate getSelectedDate();

private slots:

protected:
void mousePressEvent(QMouseEvent *event);
void mouseDoubleClickEvent(QMouseEvent *event);

signals:
 void cocorico(int row,int col,MCalendar *cal);
 void cocorico2(int row,int col,MCalendar *cal);
};

#endif
