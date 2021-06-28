
#include "MGroupCalendar.h"
#include <QPixmap>
#include <QIcon>
#include <QKeyEvent>
#include <math.h>
#include "window.h"

namespace  globalNamespace
{
QList<QFont> NormalDayFont;
QList<QFont> firstPublidayFont;
QList<QFont> secondPublicDayFont;
QList<QFont> fearFont;
QList<QFont> selectionFont;

QList<QPalette> NormalDayPalette;
QList<QPalette> firstPublicDayPalette;
QList<QPalette> secondPublicDayPalette;
QList<QPalette> fearPalette;
QList<QPalette> selectionPalette;
QPalette headersPalette;
QFont headersFont;
QString headerLabelFormat;
bool gridVisible;
bool showMoon;
bool showMoonTime;
bool showFear;
}

MGroupCalendar::MGroupCalendar()
{
    init();
    doCalendars(QDate::currentDate(),defaultMonthsAfter,defaultMonthBefore);
    createCommandsLayout();
    mainlayout->addLayout(calendarLayout);
    mainlayout->addLayout(commandsLayout);
    setLayout(mainlayout);
    lastCalSelected = -1;
}

MGroupCalendar::MGroupCalendar(int year,int curMonth,int mthAfter,int mthBefore)
{
    defaultMonthsAfter = mthAfter;
    defaultMonthBefore = mthBefore;
    init();
    doCalendars(year,curMonth,  mthAfter,  mthBefore);
    createCommandsLayout();
    mainlayout->addLayout(calendarLayout);
    mainlayout->addLayout(commandsLayout);
    setLayout(mainlayout);
}

MGroupCalendar::MGroupCalendar(const QDate &date, int mthAfter, int mthBefore)
{
    defaultMonthsAfter = mthAfter;
    defaultMonthBefore = mthBefore;
    init();
    doCalendars(date, mthAfter, mthBefore);
    createCommandsLayout();
    mainlayout->addLayout(calendarLayout);
    mainlayout->addLayout(commandsLayout);
    setLayout(mainlayout);
}

void MGroupCalendar::init()
{
  lastCalSelected = -1;
  nbMonth = defaultMonthsAfter -  -(defaultMonthBefore);
  currentSeletedDate = QDate::currentDate();
  if (globalNamespace::headerLabelFormat == "")
      globalNamespace::headerLabelFormat = "MMMM yyyy";

  mainlayout     = new QVBoxLayout;
  calendarLayout = new QHBoxLayout;
  commandsLayout  = new QHBoxLayout;
  mph = new MoonPhases;

}


void MGroupCalendar::setHeadersBgColor(const QColor &color)
{
    for(int i=0;i<labels.count();i++)
    {
        labels[i]->setAutoFillBackground(true);
        globalNamespace::headersPalette.setColor(labels[i]->backgroundRole(),color);
        labels[i]->setPalette(globalNamespace::headersPalette);
    }
}

void MGroupCalendar::setHeadersFgColor(const QColor &color)
{
    for(int i=0;i<labels.count();i++)
    {
        labels[i]->setAutoFillBackground(true);
        globalNamespace::headersPalette.setColor(labels[i]->foregroundRole(),color);
        labels[i]->setPalette(globalNamespace::headersPalette);
    }
}


void MGroupCalendar::setHeadersFont(QFont &font)
{
    globalNamespace::headersFont=font;
    for(int i=0;i<labels.count();i++)
        labels[i]->setFont(globalNamespace::headersFont);
}


void MGroupCalendar::doCalendars(int year,int curMonth,int mthAfter,int mthBefore)
{
    currentYear = year;
    currentMonth = curMonth;
    QDate curDate(year,curMonth,1);
    firstPanelDate = curDate.addMonths(-mthBefore);

    defaultMonthsAfter = mthAfter;
    defaultMonthBefore = mthBefore;
    lastPanelDate = firstPanelDate.addMonths(nbMonth - 1);
    create();
}


void MGroupCalendar::doCalendars(const QDate &date, int mthAfter, int mthBefore)
{
   currentYear = date.year();
   currentMonth = date.month();
   firstPanelDate = date.addMonths(-mthBefore);
   defaultMonthsAfter = mthAfter;
   defaultMonthBefore = mthBefore;
   lastPanelDate = firstPanelDate.addMonths(nbMonth - 1);
   create();
}

QVBoxLayout * MGroupCalendar::makeVBox(int year, int month)
{
    QVBoxLayout *qvbl = new QVBoxLayout;
    QLabel *label = makeHeaderLabel(year,month);
    labels << label;
    label->setAlignment(Qt::AlignHCenter);
    qvbl->addWidget(label);
    MCalendar *cal = new MCalendar(year,month);
    cal->setShowGrid(globalNamespace::gridVisible);

    QList<MDate> *lCalData = cal->getData();
    for (QList<MDate>::iterator i = lCalData->begin();i != lCalData->end() ; ++i)
    {
        MDate *md = &*i;
        QDate qd(md->year(),md->month(),md->day());
        mMdate[qd] = md;
    }
    cal->setParentObj(this);
    cal->setIndex(index++);

    qvbl->addWidget(cal);
    cals << cal;
    return qvbl;
}

QVBoxLayout * MGroupCalendar::makeVBox(const QDate &d)
{
    QVBoxLayout *qvbl = new QVBoxLayout;
    QLabel *label = makeHeaderLabel(d);
    labels << label;
    label->setAlignment(Qt::AlignHCenter);
    qvbl->addWidget(label);
    MCalendar *cal = new MCalendar(d);
    cal->setShowGrid(globalNamespace::gridVisible);

    QList<MDate> *lCalData = cal->getData();
    for (QList<MDate>::iterator i = lCalData->begin();i != lCalData->end() ; ++i)
    {
        MDate *md = &*i;
        QDate qd(md->year(),md->month(),md->day());
        mMdate[qd] = md;
    }
    cal->setParentObj(this);
    cal->setIndex(index++);
    qvbl->addWidget(cal);

    cals << cal;
    return qvbl;
}

void MGroupCalendar::setHeaderLabelFormat(const QString &fmt)
{
    globalNamespace::headerLabelFormat = fmt;
}

void MGroupCalendar::create()
{
   QDate qd;
    for(qd = firstPanelDate;qd <= lastPanelDate ;)
    {
        QVBoxLayout *vbl = makeVBox(qd);
        calendarLayout->addLayout(vbl);
        qd = qd.addMonths(1);
    }

    setMoonPhases();
    pullEasterDate();
    redrawAfterDataChanged();
}
int MGroupCalendar::getNbMonth()
{
    return this->nbMonth;
}

void MGroupCalendar::addPanel()
{
   lastPanelDate = lastPanelDate.addMonths(1);
   QVBoxLayout * vbl = makeVBox(lastPanelDate);
   calendarLayout->addLayout(vbl);
   nbMonth++;
   defaultMonthsAfter++;
   setMoonPhases();
   pullEasterDate();
   redrawAfterDataChanged();

}
void MGroupCalendar::refreshHeaderLabel()
{
    QDate mDate(firstPanelDate);
    int i = 0;
    do
    {
        labels[i]->setText(mDate.toString(globalNamespace::headerLabelFormat));
        mDate =mDate.addMonths(1);
        i++;
    } while(i<nbMonth);
}

void MGroupCalendar::removePanel()
{
   int last = cals.count() - 1;
   lastPanelDate   =   lastPanelDate.addMonths(-1);
   QLayout *lastLayout = searchLayout(calendarLayout,last);
   MCalendar *lastcal  =   cals.last();
   lastcal->setVisible(false);
   lastLayout->removeWidget(lastcal);
   QLabel    *lastLabel = labels.last();
   lastLabel->setVisible(false);
   lastLayout->removeWidget(lastLabel);
   // Deselects all selected items
   lastcal->clearSelection();
   // Disconnect all signals from table widget ! important !
   lastcal->disconnect();
   // Remove all items
   lastcal->clearContents();
   // Set row count to 0 (remove rows)
   lastcal->setRowCount(0);
   cals.removeAt(last);
   labels.removeAt(last);
   showLayoutWidgets(lastLayout,false);
   removeLayoutWidgets(lastLayout,true);
   nbMonth --;
   defaultMonthsAfter--;
   redrawAfterDataChanged();
}
QLayout * MGroupCalendar::searchLayout(QLayout *layout,int n)
{
    QLayoutItem *item =layout->itemAt(n);
    if (QLayout* childLayout = item->layout())
        return childLayout;
    return 0;
}
void MGroupCalendar::setFirstHolyDay(int n)
{
    globalNamespace::firstHolly = n;
}

void MGroupCalendar::setSecondHolyDay(int n)
{
    globalNamespace::secondHolly = n;
}

void MGroupCalendar::showLayoutWidgets(QLayout* layout,bool show)
{
    QLayoutItem *item;
    for (int i=0;i<layout->count();i++)
    {
        item = layout->itemAt(i);
        if (QWidget* widget = item->widget())
            widget->setVisible(show);
        else if (QLayout* childLayout = item->layout())
            showLayoutWidgets(childLayout,show);
    }
}
void MGroupCalendar::removeLayoutWidgets(QLayout* layout, bool doIt)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (doIt)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            removeLayoutWidgets(childLayout, doIt);
        delete item;
    }
}
QLabel * MGroupCalendar::makeHeaderLabel(int year, int month)
{
    QDate qd(year,month,1);
    return makeHeaderLabel(qd);
}

QLabel * MGroupCalendar::makeHeaderLabel(const QDate &qd)
{
    QLabel *ql = new QLabel(qd.toString(globalNamespace::headerLabelFormat));
    ql->setAutoFillBackground(true);
    ql->setPalette(globalNamespace::headersPalette);
    ql->setFont(globalNamespace::headersFont);
    return ql;
}
int MGroupCalendar::getFirstHolyDay()
{
  return globalNamespace::firstHolly;
}
int MGroupCalendar::getSecondHolyDay()
{
  return globalNamespace::secondHolly;
}
bool MGroupCalendar::getHasMoon()
{
  return globalNamespace::showMoon;
}
bool MGroupCalendar::getHasMoonHours()
{
    return globalNamespace::showMoonTime;
}

void MGroupCalendar::storeOnePhase(int moontype)
{
     ln_dt *moon;
     if (moontype == NEWMOON)
          moon = mph->getnewMoon();
      else if(moontype == FULLMOON)
         moon = mph->getfullMoon();
      else if( moontype == FIRSTQUATER)
         moon = mph->getfirstQuater();
      else if (moontype == LASTQUATER)
         moon = mph->getlastQuater();

     QDateTime qdt_moon(QDate(moon->year,moon->month,moon->day),QTime(moon->hour,moon->min,moon->secs));
             qdt_moon = qdt_moon.addSecs(qdt_moon.offsetFromUtc());

    QDate dt_moon(qdt_moon.date());

     if (mMdate.contains(dt_moon) == true)
     {
         if (mMdate[dt_moon]->hasmoon() == false)
             mMdate[dt_moon]->setMoon(moontype,qdt_moon.time());
     }
}

void MGroupCalendar::setMoonPhases()
{
    QDate stDate(firstPanelDate.addMonths(-2));
    QDate endDate(lastPanelDate.addMonths(2));
    QDate mDate=stDate;

    while (mDate < endDate)
    {
        int year,month,d;
        mDate.getDate(&year,&month,&d);
        mph->computeMoonPhases(year,month);
        storeOnePhase(NEWMOON);
        storeOnePhase(FULLMOON);
        storeOnePhase(FIRSTQUATER);
        storeOnePhase(LASTQUATER);

        mph->computeMoonPhases(year,month,1);
        storeOnePhase(NEWMOON);
        storeOnePhase(FULLMOON);
        storeOnePhase(FIRSTQUATER);
        storeOnePhase(LASTQUATER);

        mph->computeMoonPhases(year,month,15);
        storeOnePhase(NEWMOON);
        storeOnePhase(FULLMOON);
        storeOnePhase(FIRSTQUATER);
        storeOnePhase(LASTQUATER);
        mDate = mDate.addMonths(1);
    }
}

void MGroupCalendar::cellIsClicked(int row, int col, MCalendar *cal)
{
   cal->selectLine(row);

  const QDate &selectedDate = cal->getSelectedDate();
  selectDate(selectedDate);

}

QPushButton * MGroupCalendar::makeButton(const QString &name, const QString &icon)
{
    QPixmap pixmap(icon);
    QIcon ButtonIcon(pixmap);
    QPushButton *button = new QPushButton(name);
    button->setIcon(ButtonIcon);
    return button;
}
void MGroupCalendar::redrawAfterDataChanged()
{
    for (int i = 0;i<cals.count();i++)
        cals[i]->reDrawTable();
}

void MGroupCalendar::setMonthBefore(int n)
{
  if (defaultMonthBefore == n)
      return;
  QDate qdInitial(currentYear,currentMonth,1);
  QDate to = qdInitial.addMonths(-n);
  firstPanelDate = to;
  defaultMonthBefore = n;
  jumpTo(to);
}

void MGroupCalendar::setNbMonth(int n)
{
    if (n < 1 || n>12)
        return;
    while (nbMonth < n)
        addPanel();
    while(nbMonth> n)
        removePanel();
}

int MGroupCalendar::getCurrentMonth()
{
    return this->currentMonth;
}
int MGroupCalendar::getCurrentYear()
{
    return this->currentYear;
}

void MGroupCalendar::keyPressEvent(QKeyEvent *event)
{

}
void MGroupCalendar::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
        monthPlus();
    else if (event->key() == Qt::Key_Left)
        monthMinus();
    else if (event->key() == Qt::Key_Up)
        dayMinus();
    else if (event->key() == Qt::Key_Down)
        dayPlus();
    else if (event->key() == Qt::Key_PageUp)
        pageMinus();
    else if (event->key() == Qt::Key_PageDown)
        pagePlus();
    else if (event->key() == Qt::Key_End)
    {
     QDate newDate =  firstPanelDate.addMonths(nbMonth-1);
        newDate.setDate(newDate.year(),newDate.month(),newDate.daysInMonth());
     selectDate(newDate);
    }
    else if (event->key() == Qt::Key_Home)
    {
        QDate newDate(firstPanelDate.year(),firstPanelDate.month(),1);
        selectDate(newDate);
    }
}
int MGroupCalendar::getMonthBefore()
{
    return defaultMonthBefore;
}

void MGroupCalendar::createCommandsLayout()
{
    QHBoxLayout *hbxlyt1 = new QHBoxLayout;
    QPushButton *leftArrowYears = makeButton(tr("Year"),":/icons/arrow_left");
    QPushButton *rightArrowYears = makeButton(tr("Year"),":/icons/arrow_right");
    QPushButton *leftArrowMonth = makeButton(tr("Month"),":/icons/arrow_left");
    QPushButton *rightArrowMonth = makeButton(tr("Month"),":/icons/arrow_right");
    QPushButton *leftArrowDay = makeButton(tr("Days"),":/icons/arrow_left");
    QPushButton *rightArrowDay = makeButton(tr("Days"),":/icons/arrow_right");
    QPushButton *leftArrowPages = makeButton(tr("Prev. Page"),":/icons/arrow_left");
    QPushButton *rightArrowPages = makeButton(tr("Next Page"),":/icons/arrow_right");
    QPushButton *btnToday = new QPushButton(tr("Today"));
    btnQuit   =  new QPushButton(tr("Exit"));
    btnSettings= new QPushButton(tr("Show/Hide Settings"));



    hbxlyt1->addWidget(leftArrowPages);
    hbxlyt1->setAlignment(leftArrowPages,Qt::AlignLeft);
    hbxlyt1->addWidget(leftArrowYears);
    hbxlyt1->setAlignment(leftArrowYears,Qt::AlignLeft);
    hbxlyt1->addWidget(leftArrowMonth);
    hbxlyt1->setAlignment(leftArrowMonth,Qt::AlignLeft);
    hbxlyt1->addWidget(leftArrowDay);
    hbxlyt1->setAlignment(leftArrowDay,Qt::AlignLeft);
    hbxlyt1->addWidget(btnToday);
    hbxlyt1->setAlignment(btnToday,Qt::AlignCenter);
    hbxlyt1->addWidget(rightArrowDay);
    hbxlyt1->setAlignment(rightArrowDay,Qt::AlignRight);
    hbxlyt1->addWidget(rightArrowMonth);
    hbxlyt1->setAlignment(rightArrowMonth,Qt::AlignRight);
    hbxlyt1->addWidget(rightArrowYears);
    hbxlyt1->setAlignment(rightArrowYears,Qt::AlignRight);
    hbxlyt1->addWidget(rightArrowPages);
    hbxlyt1->setAlignment(rightArrowPages,Qt::AlignRight);
    hbxlyt1->addWidget(btnSettings,Qt::AlignRight);
    btnSettings->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    hbxlyt1->addWidget(btnQuit,Qt::AlignRight);
    btnQuit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    commandsLayout->addLayout(hbxlyt1);
    connect(leftArrowPages,SIGNAL(clicked()),this,SLOT(pageMinus()));
    connect(rightArrowPages,SIGNAL(clicked()),this,SLOT(pagePlus()));
    connect(leftArrowYears,SIGNAL(clicked()),this,SLOT(yearMinus()));
    connect(rightArrowYears,SIGNAL(clicked()),this,SLOT(yearPlus()));
    connect(leftArrowMonth,SIGNAL(clicked()),this,SLOT(monthMinus()));
    connect(rightArrowMonth,SIGNAL(clicked()),this,SLOT(monthPlus()));
    connect(leftArrowDay,SIGNAL(clicked()),this,SLOT(dayMinus()));
    connect(rightArrowDay,SIGNAL(clicked()),this,SLOT(dayPlus()));
    connect(btnToday,SIGNAL(clicked()),this,SLOT(toDay()));

}

QPushButton * MGroupCalendar::getBtnSettings()
{
    return this->btnSettings;
}
QPushButton * MGroupCalendar::getBtnQuit()
{
    return this->btnQuit;
}

void MGroupCalendar::pageMinus()
{
   if (lastLineSelected != -1)
       lastLineSelected = -1;
   firstPanelDate = firstPanelDate.addMonths(-nbMonth);
   jumpTo(firstPanelDate);
}
void MGroupCalendar::pagePlus()
{
    if (lastLineSelected != -1)
        lastLineSelected = -1;
    firstPanelDate = firstPanelDate.addMonths(nbMonth);
    jumpTo(firstPanelDate);
}
void MGroupCalendar::yearMinus()
{
    firstPanelDate = firstPanelDate.addYears(-1);
    jumpTo(firstPanelDate);
}
void MGroupCalendar::yearPlus()
{
    firstPanelDate = firstPanelDate.addYears(1);
    jumpTo(firstPanelDate);
}
void MGroupCalendar::monthMinus()
{
 monthSeek(-1);
}
void MGroupCalendar::monthSeek(int n)
{
    QDate date2check = (n>0) ? (firstPanelDate.addMonths(nbMonth)) : firstPanelDate;
    if (currentSeletedDate.year() == date2check.year() && currentSeletedDate.month() == date2check.month())
    {
        if (n > 0)
            firstPanelDate =firstPanelDate.addMonths(nbMonth);
        else if (n < 0)
            firstPanelDate =firstPanelDate.addMonths(-nbMonth);
        jumpTo(firstPanelDate);
    }

    int currentNbDay = currentSeletedDate.day();
    currentSeletedDate = currentSeletedDate.addMonths(n);
    int nextNbDay    =  currentSeletedDate.day();
    if (remaining > 0)
        currentSeletedDate =  currentSeletedDate.addDays(remaining);
    if (currentNbDay > nextNbDay)
        remaining =  currentNbDay - nextNbDay;
    else
        remaining = 0;

    selectDate(currentSeletedDate);
}

void MGroupCalendar::monthPlus()
{
  monthSeek(+1);
}

void MGroupCalendar::dayMinus()
{
   QDate minimumDate(firstPanelDate.year(),firstPanelDate.month(),1);

    if (currentSeletedDate == minimumDate)
    {
        firstPanelDate = firstPanelDate.addMonths(-nbMonth);
        jumpTo(firstPanelDate);
    }
    currentSeletedDate = currentSeletedDate.addDays(-1);
    selectDate(currentSeletedDate);
}

void MGroupCalendar::dayPlus()
{
    currentSeletedDate = currentSeletedDate.addDays(1);
    selectDate(currentSeletedDate);
}

void MGroupCalendar::toDay()
{
    selectDate(QDate::currentDate());
}

void MGroupCalendar::jumpTo(const QDate &dt)
{

   jumpTo(dt.year(),dt.month());
}

void MGroupCalendar::jumpTo(int year,int month)
{
    mMdate.clear();
    currentMonth=month;
    currentYear=year;
    firstPanelDate = QDate(year,month,1);
    QDate qd(firstPanelDate);
    lastPanelDate=firstPanelDate.addMonths(nbMonth - 1);
    for (int n=0;qd <= lastPanelDate;n++)
    {

        labels[n]->setText(qd.toString(globalNamespace::headerLabelFormat));
        cals[n]->reset(qd.year(),qd.month());
        QList<MDate> *lCalData = cals[n]->getData();
        for (QList<MDate>::iterator i = lCalData->begin();i != lCalData->end() ; ++i)
        {
            MDate *md = &*i;
            QDate qd(md->year(),md->month(),md->day());
            mMdate[qd] = md;
        }
        qd = qd.addMonths(1);
    }
    setMoonPhases();
    pullEasterDate();
    redrawAfterDataChanged();
}

void MGroupCalendar::pullEasterDate()
{
    QDate tm = firstPanelDate;
    QDate var;
    while(tm <= lastPanelDate)
    {
        int year = tm.year();
        if (tm.month() >= 4 && tm.month() <= 6)
        {
            QDate paques = getEasterDate(year);
            if (mMdate.contains(paques))
                mMdate[paques]->setEaster(true);
            var = paques.addDays(1);
            if (mMdate.contains(var))
                mMdate[var]->setMondayEaster(true);
            var = paques.addDays(-1);
            while (var.dayOfWeek() != 7)
                var = var.addDays(-1);
            if (mMdate.contains(var))
                mMdate[var]->setRameaux(true);
            var = paques.addDays(39);
            if (mMdate.contains(var))
                mMdate[var]->setAscension(true);
            var = paques.addDays(49);
            if (mMdate.contains(var))
                mMdate[var]->setPentecote(true);
            var = paques.addDays(50);
             if (mMdate.contains(var))
                 mMdate[var]->setLundiPent(true);
             var = paques.addDays(-2);
             if (mMdate.contains(var))
                mMdate[var]->setVendrediSaint(true);
        }
        tm = tm.addMonths(1);
    }
}

QDate MGroupCalendar::getEasterDate(int year)
{
    int s,t,a,p;
    int jps,jp;
    int g,G;
    int b,r;
    int C;
    int d,h,e,f;
    int R;
    QDate qd;

    /*
     *
     * D'après la méthode de Conway.
     * Valable seulement pour les années postérieures à 1583
     *
     */
    s = year/100;
    t = year % 100;
    a = t/4;
    p = s%4;
    jps = (9-2*p) % 7;
    jp = (jps + t + a) % 7;
    g =  year % 19;
    G = g + 1; /*Cycle de méthon*/

    b = s/4; //Métemptose
    r =  8 * ( s + 11 ) / 25; //Proemptose
    C =  -s + b + r; //Correction séculaire
    d = (11 * G + C) % 30;
    d = (d + 30) % 30; //Pleine lune pascale
    h = (551 - 19 * d + G) / 554 ; //Correction d'épactes
    e = (50 - d - h) % 7; //Ecart Lune pascale au jour pivot
    f = (e + jp) % 7;

    R = 57 - d - f - h; //Dimanche de paques.

    if (R <= 31)
        qd = QDate(year,3,R);
    else
    {
        R -= 31;
        qd = QDate(year,4,R);
    }
    return qd;
}

void MGroupCalendar::selectDate(const QDate &dt)
{
    //[TODO] ne pas déselectionner les lignes hors champs.
    int whichMonthPanel;
    int into =false;
    //dans le champ ?
    for (int i=0;i<nbMonth;i++)
        if (cals[i]->getYear() == dt.year() && cals[i]->getMonth() == dt.month())
        {
            into = true;
            break;
        }

  if (into == false)
      jumpTo(dt);

    for(int i=0;i<nbMonth;i++)
        if (cals[i]->getYear() == dt.year() && cals[i]->getMonth() == dt.month())
        {
            whichMonthPanel = i;
            break;
        }

        if (lastCalSelected != -1)
            if (whichMonthPanel != lastCalSelected )
                cals[lastCalSelected]->unselectLine(lastLineSelected);

            lastCalSelected = whichMonthPanel;
            lastLineSelected = dt.day() - 1;


            cals[whichMonthPanel]->selectLine(dt.day() - 1);

            currentSeletedDate = dt;
}
void MGroupCalendar::cellIsDoubleClicked(int row, int col,MCalendar *cal)
{
  //qDebug() << "Row :  " << row << ", col : " << col << "is double Clicked.";
  Window *pwin = qobject_cast<Window *>(mParent);
  if (pwin == 0)
  {
      qFatal("Could not cast parent Window object from MGroupCalendar\n");
      return;
  }
  pwin->showRdvWin(row,col,cal);
}
void MGroupCalendar::setParent(QObject *parent)
{
    this->mParent = parent;
}

QObject * MGroupCalendar::getParent()
{
    return this->mParent;
}

void MGroupCalendar::setDayformat(int n)
{
    globalNamespace::defaultDayFormat = n;
}

int MGroupCalendar::getDayFormat()
{
    return globalNamespace::defaultDayFormat;
}

void MGroupCalendar::setFirstHolyPalette(QPalette &pal,int col,bool redraw)
{
  globalNamespace::firstPublicDayPalette[col] = pal;
  if (redraw == true)
      redrawAfterDataChanged();
}

void MGroupCalendar::setSecondHolyPalette(QPalette &pal,int col,bool redraw)
{
  globalNamespace::secondPublicDayPalette[col] = pal;
  if (redraw == true)
      redrawAfterDataChanged();
}

void MGroupCalendar::setNormalDayPalette(QPalette &pal,int col,bool redraw)
{
   globalNamespace::NormalDayPalette[col] = pal;
   if (redraw == true)
       redrawAfterDataChanged();
}

void MGroupCalendar::setSelectionPalette(QPalette &pal,int col,bool redraw)
{
   globalNamespace::selectionPalette[col] = pal;
   if (redraw == true)
       redrawAfterDataChanged();
}

void MGroupCalendar::setFearDayPalette(QPalette &pal,int col,bool redraw)
{
  globalNamespace::fearPalette[col] = pal;
  if (redraw == true)
      redrawAfterDataChanged();
}

QPalette * MGroupCalendar::getFirstHolyPalette(int col)
{
  return &globalNamespace::firstPublicDayPalette[col];
}

QPalette * MGroupCalendar::getSecondHolyPalette(int col)
{
  return &globalNamespace::secondPublicDayPalette[col];
}

QPalette * MGroupCalendar::getNormalDayPalette(int col)
{
  return &globalNamespace::NormalDayPalette[col];
}

QPalette * MGroupCalendar::getSelectionPalette(int col)
{
  return &globalNamespace::selectionPalette[col];
}

QPalette * MGroupCalendar::getFearDayPalette(int col)
{
  return &globalNamespace::fearPalette[col];
}

QFont * MGroupCalendar::getNormalDayFont(int col)
{
    return &(globalNamespace::NormalDayFont[col]);
}

QFont * MGroupCalendar::getFirstHolyFont(int col)
{
    return &(globalNamespace::firstPublidayFont[col]);
}
QFont * MGroupCalendar::getSecondHolyFont(int col)
{
    return &(globalNamespace::secondPublicDayFont[col]);
}
QFont * MGroupCalendar::getSelectionFont(int col)
{
    return &(globalNamespace::selectionFont[col]);
}
QFont * MGroupCalendar::getFearDayFont(int col)
{
    return &(globalNamespace::fearFont[col]);
}

void MGroupCalendar::setFirstHolyFont(QFont &fnt,int col,bool redraw)
{
  globalNamespace::firstPublidayFont[col] = fnt;
  if (redraw == true)
      redrawAfterDataChanged();
}

void MGroupCalendar::setSecondHolyFont(QFont &fnt,int col,bool redraw)
{
   globalNamespace::secondPublicDayFont[col] = fnt;
   if (redraw == true)
       redrawAfterDataChanged();
}

void MGroupCalendar::setNormalDayFont(QFont &fnt,int col,bool redraw)
{
    globalNamespace::NormalDayFont[col] = fnt;
    if (redraw == true)
        redrawAfterDataChanged();
}

void MGroupCalendar::setSelectionFont(QFont &fnt,int col,bool redraw)
{
    globalNamespace::selectionFont[col] = fnt;
    if (redraw == true)
        redrawAfterDataChanged();
}

void MGroupCalendar::setFearDayFont(QFont &fnt,int col,bool redraw)
{
   globalNamespace::fearFont[col] = fnt;
   if (redraw == true)
       redrawAfterDataChanged();
}
QString MGroupCalendar::getHeaderLabelFormat()
{
    return globalNamespace::headerLabelFormat;
}

QFont *MGroupCalendar::getHeaderFont()
{
  return &( globalNamespace::headersFont) ;
}

QPalette * MGroupCalendar::getHeaderPalette()
{
  return  & (globalNamespace::headersPalette);
}

