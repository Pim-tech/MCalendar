#ifndef VIEWERCONSOLE_H
#define VIEWERCONSOLE_H
#include "conscolor.h"
#include <QDateTime>
#include <QMultiMap>
#include "../recorder.h"
#include "datefilter.h"



class viewerConsole
{
 const int seek_display = 2;
 QMultiMap<QDateTime,dt_per_msg> *disp;
 recorder *record;
 conscolor *cprinter;
 int daystoDisplayBeforeToday;
 int daystoDisplayAfterToday;
 int offsetToIndex(qint64 offsetDays);
 QTextStream *out;
public:
 void display();
    viewerConsole();
};

#endif // VIEWERCONSOLE_H
