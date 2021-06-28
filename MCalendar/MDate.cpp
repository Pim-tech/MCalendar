#include <QDebug>
#include "MDate.h"
#include "fears.h"
#include <math.h>
#include "MoonPhases.h"

namespace globalNamespace
{
  int firstHolly = 7;
  int secondHolly = 6;
  int defaultDayFormat = DAY_LONG_NAME;
}

MDate::MDate()
{
    initColumnStyleSheet();
}

MDate::MDate(const QDate &d):QDate(d)
{

    initColumnStyleSheet();
}
MDate::MDate(int y,int m , int d):QDate(y,m,d)
{
    initColumnStyleSheet();
}

void MDate::initColumnStyleSheet()
{
    index =0;
    moontype = 0;
    hasMoon =false;
    paques = false;
    lundiPaques = false;
    ascension = false;
    pentecote = false;
    rameaux   = false;
    lundiPentecote = false;
    vendrediSaint = false;

    moonTimeDefaultFormat = "HH:mm";
}

QString MDate::getDay()
{
    QString str;
    switch(globalNamespace::defaultDayFormat)
    {
    case 0:
        str = toString("dddd");
        return QString(str.toUpper().at(0));
        break;
    case 1:
        str = toString("dddd").left(3);
        str[0]= str[0].toTitleCase();
        return str;
        break;
    case 2:
        return toString("dddd");
        break;
    default:
        break;
    }
    return "";
}

void MDate::setEaster(bool easter)
{
    this->paques = easter;
}

bool MDate::isFearDay()
{
    if (fearDay() == "")
        return false;
    return true;
}

bool MDate::isPaques()
{
    return false;
}
QString MDate::fearDay()
{
    if (month() == 1 && day() == 1)
        return "Jour de l'An";
    if (month() == 5 && day() == 1)
        return "Fête du travail";
    if (month() == 5 && day() == 8)
        return "8 Mai 1945";
    if (month() == 7 && day() == 14 )
        return "Fête Nat.";
    if (month() == 8 && day() == 15)
        return "ASSOMPTION";
    if (month() == 11 && day() == 1)
        return "TOUSSAINT";
    if (month() == 11 && day() == 11)
        return "ARMISTICE 11 Nov";
    if (month() == 12 && day() == 25)
        return "NOËL";
    if (paques == true)
        return "PAQUES";
    if (lundiPaques == true)
        return "Lundi de Paques";
    if (rameaux == true)
        return "RAMEAUX";
    if (ascension == true)
        return "ASCENSION";
    if (pentecote == true)
        return "PENTECÔTE";
    if (lundiPentecote == true)
        return "Lun. PENTECÔTE";
    if (vendrediSaint == true)
        return "Vendredi Saint";
    return "";
}

void MDate::setMondayEaster(bool measter)
{
    this->lundiPaques = measter;
}
void MDate::setRameaux(bool ram)
{
    this->rameaux = ram;
}
void MDate::setLundiPent(bool lpent)
{
    this->lundiPentecote = lpent;
}

void MDate::setAscension(bool asc)
{
    this->ascension = asc;
}
void MDate::setPentecote(bool pent)
{
    this->pentecote =pent;
}
void MDate::setVendrediSaint(bool vendredi)
{
    this->vendrediSaint = vendredi;
}

void MDate::setIndex(int i)
{
    this->index = i;
}
int MDate::getIndex()
{
    return index;
}

QString MDate::getMoonTimeDefaultFormat()
{
    return moonTimeDefaultFormat;
}

QPalette MDate::genPalette(int column)
{
    QPalette qp;
    qp.setColor(QPalette::Text,QColor("#FFFFFF"));
    qp.setColor(QPalette::Base,QColor("#0000FF"));
    if (column == 4)
    {
        if (isFearDay())
            return globalNamespace::fearPalette[column];
        if (dayOfWeek() == globalNamespace::secondHolly)
            return globalNamespace::secondPublicDayPalette[column];
        if (dayOfWeek() == globalNamespace::firstHolly)
            return  globalNamespace::firstPublicDayPalette[column];
    }
    else
    {
        if (dayOfWeek() == globalNamespace::firstHolly)
            return globalNamespace::firstPublicDayPalette[column];
        if (isFearDay())
            return globalNamespace::fearPalette[column];
        if (dayOfWeek() == globalNamespace::secondHolly )
            return globalNamespace::secondPublicDayPalette[column];
    }
    return globalNamespace::NormalDayPalette[column];
}

QFont MDate::genFont(int column)
{
    if (column == 4)
    {
        if (isFearDay())
            return globalNamespace::fearFont[column];
        if (dayOfWeek() == globalNamespace::secondHolly)
            return globalNamespace::secondPublicDayFont[column];
        if (dayOfWeek() == globalNamespace::firstHolly)
            return  globalNamespace::firstPublidayFont[column];
    }
    else
    {
        if (dayOfWeek() == globalNamespace::firstHolly)
            return globalNamespace::firstPublidayFont[column];
        if (isFearDay())
            return globalNamespace::fearFont[column];
        if (dayOfWeek() == globalNamespace::secondHolly )
            return globalNamespace::secondPublicDayFont[column];
    }
    return globalNamespace::NormalDayFont[column];
}

QPixmap * MDate::genMoonPixmap()
{
    QPixmap  *pixmap = new QPixmap;
    if (hasmoon() == true)
    {
        if (moontype == FULLMOON )
            pixmap->load(":/icons/pleine_lune");
        else if (moontype == NEWMOON)
            pixmap->load(":/icons/nouvelle_lune");
        else if (moontype == FIRSTQUATER)
            pixmap->load(":/icons/premier_quartier");
        else if (moontype == LASTQUATER)
            pixmap->load(":/icons/dernier_quartier");
    }

    return pixmap;
}
QString MDate::getMoonTextTime()
{
    if (hasmoon() == true)
        return moonTime.toString(moonTimeDefaultFormat);
    return "";
}
void MDate::setMoonTimeDefaultFormat(const QString &fmt)
{
    this->moonTimeDefaultFormat = fmt;
}

bool MDate::hasmoon()
{
    return hasMoon;
}

QTime MDate::getMoonTime()
{
    return moonTime;
}

int MDate::getMoonType()
{
    return moontype;
}

void MDate::setMoon(int type,const QTime &dt)
{
    this->moonTime = dt;
    this->moontype = type;
    this->hasMoon = true;
}

QString MDate::getNumDay()
{
    return QString::number((index+1));
}

QString MDate::getFear()
{
    MFear *mf = new MFear;
    QString str = fearDay();
    if (fearDay() != "")
        return str;
    return mf->getFear(month(),day());
}

bool MDate::operator!=(const QDate &d) const
{
    QDate *dt = (QDate *) this;
    return ( *dt != d);
}
bool MDate::operator<(const QDate &d) const
{
    QDate *dt = (QDate *) this;
    return ( *dt < d);
}
bool MDate::operator<=(const QDate &d) const
{
    QDate *dt = (QDate *) this;
    return ( *dt <= d);
}
bool MDate::operator==(const QDate &d) const
{
    QDate *dt = (QDate *) this;
    return (*dt == d);
}
bool MDate::operator>(const QDate &d) const
{
    QDate *dt = (QDate *) this;
    return ( *dt > d);
}
bool MDate::operator>=(const QDate &d) const
{
    QDate *dt = (QDate *) this;
    return ( *dt >= d);
}

bool MDate::operator!=(const MDate &d) const
{
    QDate *dt = (QDate *) this;
    QDate *date = (QDate *) &d;
    return ( *dt != *date);
}
bool MDate::operator<(const MDate &d) const
{
    QDate *dt = (QDate *) this;
    QDate *date = (QDate *) &d;
    return ( *dt < *date);
}
bool MDate::operator<=(const MDate &d) const
{
    QDate *dt = (QDate *) this;
    QDate *date = (QDate *) &d;
    return ( *dt <= *date);
}
bool MDate::operator==(const MDate &d) const
{
    QDate *dt = (QDate *) this;
    QDate *date = (QDate *) &d;
    return (*dt == *date);
}
bool MDate::operator>(const MDate &d) const
{
    QDate *dt = (QDate *) this;
    QDate *date = (QDate *) &d;
    return ( *dt > *date);
}
bool MDate::operator>=(const MDate &d) const
{
    QDate *dt = (QDate *) this;
    QDate *date = (QDate *) &d;
    return ( *dt >= *date);
}

