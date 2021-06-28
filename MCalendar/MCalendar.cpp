#include "MCalendar.h"
#include "MGroupCalendar.h"
#include <QHeaderView>
#include <QDebug>
#include <QHBoxLayout>
#include <math.h>
#include "window.h"

MCalendar:: MCalendar()
{
    QDate qdt(QDate::currentDate());
    month = qdt.month();
    year  = qdt.year();
    day  = qdt.day();
    currentDate = qdt;
    lastRowClicked = -1;
    lastColumnClicked=-1;
    lastLineClicked = -1;
    init();
}

MCalendar::MCalendar(int year,int month)
{
    this->month = month;
    this->year = year;
    currentDate = QDate(month,year,1);
    init();
}

MCalendar::MCalendar(const QDate &d)
{
    month = d.month();
    year =  d.year();
    day  =  d.day();
    currentDate =d;
    init();
}

void MCalendar::init()
{
    lastRowClicked = -1;
    lastColumnClicked=-1;
    lastLineClicked = -1;
    loadDays();
    initTable();
}

int MCalendar::loadDays()
{
    QDate startDate(year,month,1);
    daysInMonth = startDate.daysInMonth();
    MDate md(startDate);

    mdtlst.clear();
    int k;
    for ( k=0;k<daysInMonth;k++)
    {
        QDate dt(md.addDays(k));
        MDate mdt(dt);

        mdt.setIndex(k);
        mdtlst << mdt;
    }
    return k;
}

QList<MDate> * MCalendar::getData()
{
    return &mdtlst;
}

void MCalendar::initTable()
{

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setColumnCount(3);
    setRowCount(daysInMonth);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setAutoScroll(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMaximumWidth(50);
}

void MCalendar::setPrec(MCalendar *prev)
{
    this->prec =prev;
}

void MCalendar::setNext(MCalendar *next)
{
    this->next = next;
}

void MCalendar::setParentObj(QObject *parent)
{
    this->parentObj = parent;
    MGroupCalendar *par = qobject_cast<MGroupCalendar *>(parent);
    if (par == 0)
        return;
    //QObject * parpar = par->getParent();
    //if (parpar == 0)
    //    return;

    connect(this,SIGNAL(cocorico(int,int,MCalendar*)),parent,SLOT(cellIsClicked(int,int,MCalendar*)));
    connect(this,SIGNAL(cocorico2(int,int,MCalendar*)),parent,SLOT(cellIsDoubleClicked(int,int,MCalendar*)));
}
QObject * MCalendar::getParentObj()
{
    return this->parentObj;
}

void MCalendar::setIndex(int i)
{
    this->index = i;
}

void MCalendar::reset(int year, int month)
{
    this->month = month;
    this->year = year;
    this->day = 1;
    lastLineClicked = -1;
    currentDate = QDate(year,month,1);
    loadDays();
    for (int i = rowCount();i<daysInMonth;i++)
        insertRow(i);
    for(int i = rowCount() - 1;i>=daysInMonth;i--)
        removeRow(i);
}

void MCalendar::reDrawTable()
{
    setShowGrid(globalNamespace::gridVisible);
    int n = 2;
    if (globalNamespace::showFear == true)
        n++;
    if (globalNamespace::showMoon == true)
        n++;
    if (globalNamespace::showMoon == true && globalNamespace::showMoonTime == true)
        n++;

    setColumnCount(n);
    //setVisible(false);
    int inc_t = 0;
    for (QList<MDate>::iterator i=mdtlst.begin();i!=mdtlst.end();++i)
    {
        MDate md(*i);
        int col=0;

        QLabel *lblNum = searchLabel(inc_t,0);
        if ( lblNum == 0)
        {
            QWidget *wNday = new QWidget;

            wNday->setAttribute(Qt::WA_WindowPropagation,false);
            QHBoxLayout *qhblNday = new QHBoxLayout;
            lblNum =  createNumDayLabel(md);
            qhblNday->setSpacing(0);
            qhblNday->setMargin(0);
            qhblNday->addWidget(lblNum);
            wNday->setLayout(qhblNday);
            setCellWidget(inc_t,col++,wNday);
        }
        else
        {
            lblNum->setFont(md.genFont(0));
            lblNum->setText(md.getNumDay());
            lblNum->setPalette(md.genPalette(0));
            col++;
        }
        QLabel *lblName = searchLabel(inc_t,1);
        if (lblName == 0)
        {
            QWidget *wday = new QWidget;
            QHBoxLayout *qhblDay = new QHBoxLayout;
            qhblDay->setSpacing(0);
            qhblDay->setMargin(0);
            QLabel *lblName = createDayLabel(md);
            qhblDay->addWidget(lblName);
            wday->setLayout(qhblDay);
            setCellWidget(inc_t,col++,wday);
        }
        else
        {
            lblName->setFont(md.genFont(1));
            lblName->setText(md.getDay());
            lblName->setPalette(md.genPalette(1));
            col++;
        }

        if (globalNamespace::showFear == true)
        {
            QLabel * qlblFear = searchLabel(inc_t,col);
            if (qlblFear == 0)
            {
                QWidget *wFear = new QWidget;
                QHBoxLayout *qhbFear = new QHBoxLayout;
                QLabel * qlblFear = createFearLabel(md);
                qhbFear->setSpacing(0);
                qhbFear->setMargin(0);
                qhbFear->addWidget(qlblFear);
                wFear->setLayout(qhbFear);
                setCellWidget(inc_t,col++,wFear);
            }
            else
            {
                qlblFear->setFont(md.genFont(col));
                qlblFear->setText(md.getFear());
                qlblFear->setPalette(md.genPalette(col));
                col++;
            }
        }

        if (globalNamespace::showMoon == false) { inc_t++;continue;}
        QLabel *moonLabel = searchLabel(inc_t,col);
        if (moonLabel == 0)
        {
            QWidget *wMoon = new QWidget;
            QHBoxLayout *qhbxMoon =  new QHBoxLayout;
            moonLabel = createMoonLabel(md);
            qhbxMoon->addWidget(moonLabel);
            qhbxMoon->setSpacing(0);
            qhbxMoon->setMargin(0);
            wMoon->setLayout(qhbxMoon);
            setCellWidget(inc_t,col++,wMoon);
        }
        else
        {
            QPixmap *pix = md.genMoonPixmap();

            if (pix == 0)
            {
                pix = new QPixmap;
            }

            moonLabel->setPixmap((*pix).scaled(12,12,Qt::KeepAspectRatio));
            moonLabel->setPalette(md.genPalette(col));
            col++;
        }
        if (globalNamespace::showMoonTime == false) { inc_t++; continue;}
        QLabel *moonTime = searchLabel(inc_t,col);
        if (moonTime == 0)
        {
            QWidget *wMoonTm = new QWidget;
            QHBoxLayout *qhbxMoonTm = new QHBoxLayout;
            qhbxMoonTm->setSpacing(0);
            qhbxMoonTm->setMargin(0);
            QLabel *lblMoonTime = createDtMoonLabel(md);
            qhbxMoonTm->addWidget(lblMoonTime);
            wMoonTm->setLayout(qhbxMoonTm);
            setCellWidget(inc_t,col++,wMoonTm);
        }
        else
        {
            if (md.hasmoon() == true)
                moonTime->setText(md.getMoonTextTime());
            else
                moonTime->setText("");

            moonTime->setPalette(md.genPalette(col));
            moonTime->setFont(md.genFont(col));
            col++;
        }
        inc_t++;
    }
    autoresize();
    setVisible(true);

    savedPrecedentPalette.resize(columnCount());
    savedPrecedentFont.resize(columnCount());
}



void MCalendar::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        emit cocorico(currentRow(),currentColumn(),this);
}
void MCalendar::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        emit cocorico2(currentRow(),currentColumn(),this);
}

QLabel * MCalendar::searchLabel(int row,int column)
{
    QLayoutItem *item;
    QWidget *wgtParent = cellWidget(row,column);
    if (wgtParent == 0)
        return 0;
    QLayout *lyt = wgtParent->layout();
    for (int i=0;i<lyt->count();i++)
    {
        item = lyt->itemAt(i);
        if (QWidget * widget = item->widget())
            if (QLabel *lbl = dynamic_cast<QLabel *>(widget))
            {
                return lbl;
            }
    }
    return 0;
}
int MCalendar::selectCell(int row,int col,bool unselect)
{
    QLayoutItem *item;
    QWidget *wgtParent = cellWidget(row,col);
    if (wgtParent == 0)
        return -1;
    QLayout *lyt = wgtParent->layout();
    for (int i=0;i<lyt->count();i++)
    {
        item = lyt->itemAt(i);
        if (QWidget * widget = item->widget())
            if (QLabel *lbl = dynamic_cast<QLabel *>(widget))
            {
                if (lastRowClicked != -1 && lastColumnClicked != -1 && unselect == true)
                    unselectCell(lastRowClicked,lastColumnClicked);

                savedPrecedentPalette[col]  = lbl->palette();
                savedPrecedentFont[col]     = lbl->font();
                lbl->setAutoFillBackground(true);
                lbl->setPalette(globalNamespace::selectionPalette[col]);
                lbl->setFont(globalNamespace::selectionFont[col]);
                lastRowClicked = row;
                lastColumnClicked = col;
                return 0;
            }
    }
    return -1;
}

int MCalendar::unselectCell(int row,int col)
{
    QLayoutItem *item;

    QWidget *wgtParent = cellWidget(row,col);
    if (wgtParent == 0)
        return -1;
    QLayout *lyt = wgtParent->layout();
    for (int i=0;i<lyt->count();i++)
    {
        item = lyt->itemAt(i);
        if (QWidget * widget = item->widget())
            if (QLabel *lbl = dynamic_cast<QLabel *>(widget))
            {

                lbl->setPalette(savedPrecedentPalette[col]);
                lbl->setFont(savedPrecedentFont[col]);

                return 0;
            }
    }
    return -1;
}

int MCalendar::selectLine(int row)
{
    if (lastLineClicked != -1)
        unselectLine(lastLineClicked);
    for (int n = 0;n < columnCount();n++)
    {
        int t = selectCell(row,n,false);
        if  (t == -1)
            return -1;
    }
    lastLineClicked = row;
    selectedDate = QDate(year,month,row+1);
    return 0;
}

QDate MCalendar::getSelectedDate()
{
    return selectedDate;
}

int MCalendar::unselectLine(int row)
{
    for (int n = 0;n < columnCount();n++)
    {
        int t = unselectCell(row,n);
        if  (t == -1)
            return -1;
    }
    return 0;
}

void MCalendar::autoresize()
{
    QRect vporig = viewport()->geometry();
    QRect vpnew = vporig;
    vpnew.setWidth(std::numeric_limits<int>::max());
    viewport()->setGeometry(vpnew);
    resizeColumnsToContents();
    resizeRowsToContents();
    viewport()->setGeometry((vporig));

    int width=0;
    for (int i=0;i<columnCount();i++)
        width += columnWidth(i);

    if (width > 800)
        width = 800;
    setMinimumWidth(width);

    int minHeigt = 0;

    for (int i = 0;i<rowCount();i++)
        minHeigt += rowHeight(i);

    minHeigt = (minHeigt > 1200) ? 1200 : minHeigt;
    setMinimumHeight(minHeigt);
}
QLabel *MCalendar::createNumDayLabel(MDate &md)
{
    QLabel *qlbl = new QLabel(md.getNumDay());
    qlbl->setFont(md.genFont(0));
    qlbl->setAutoFillBackground(true);

    qlbl->setPalette(md.genPalette(0));
    qlbl->update();
    return qlbl;
}
QLabel * MCalendar::createDayLabel(MDate &md)
{
    QLabel *qlbl = new QLabel(md.getDay());
    qlbl->setAutoFillBackground(true);
    qlbl->setPalette(md.genPalette(1));
    qlbl->setFont(md.genFont(1));

    return qlbl;
}

QLabel  *MCalendar::createFearLabel(MDate &md)
{
    QLabel *lblFear = new QLabel(md.getFear());
    lblFear->setAutoFillBackground(true);

    lblFear->setPalette(md.genPalette(2));
    lblFear->setFont(md.genFont(2));
    return lblFear;
}

QLabel * MCalendar::createMoonLabel(MDate &md)
{
    QLabel *moonLabel = new QLabel;
    moonLabel->setAutoFillBackground(true);
    moonLabel->setPalette(md.genPalette(3));
    moonLabel->setFont(md.genFont(3));

    QPixmap  *pixmap = md.genMoonPixmap();
    if (pixmap == 0) {
        // TODO printf("Pixmap is NULL.\n");
        }

    moonLabel->setPixmap((*pixmap).scaled(12,12,Qt::KeepAspectRatio));

    return moonLabel;
}

QLabel * MCalendar::createDtMoonLabel(MDate &md)
{
    QLabel *dtMoonLabel = new QLabel("");
    if (md.hasmoon() == true)
        dtMoonLabel->setText(md.getMoonTime().toString(md.getMoonTimeDefaultFormat()));

    dtMoonLabel->setAutoFillBackground(true);
    dtMoonLabel->setPalette(md.genPalette(4));
    dtMoonLabel->setFont(md.genFont(4));

    return dtMoonLabel;
}

int MCalendar::getMonth()
{
    return month;
}

int MCalendar::getYear()
{
    return year;
}

int MCalendar::getDay()
{
    return day;
}

QDate MCalendar::getDate()
{
    return currentDate;
}

int MCalendar::getIndex()
{
    return this->index;
}

